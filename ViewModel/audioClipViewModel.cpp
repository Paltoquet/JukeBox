#include "audioClipViewModel.h"
#include <QMediaContent>

// --------------------------------------------------------------------- JukeBoxViewModel

AudioClipViewModel::AudioClipViewModel(QObject* parent):
    QObject(parent),
    m_source(QUrl("")),
    m_volume(50),
    m_isRepeating(false),
    m_isPlaying(false),
    m_isInitialized(false)
{
    QObject::connect(&m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioClipViewModel::_statusChanged);
    QObject::connect(&m_player, &QMediaPlayer::stateChanged, this, &AudioClipViewModel::_stateChanged);
}

void AudioClipViewModel::play()
{
    if(!m_isInitialized){
        _initialize();
    }
    m_player.play();
}

void AudioClipViewModel::pause()
{
    m_player.stop();
}

// --------------------------------------------------------------------- properties

bool AudioClipViewModel::isPlaying() const
{
    return m_isPlaying;
}

void AudioClipViewModel::setIsPlaying(bool isPlaying)
{
    if(m_isPlaying != isPlaying){
        m_isPlaying = isPlaying;
        isPlayingChanged();
    }
}

bool AudioClipViewModel::isRepeating() const
{
    return m_isRepeating;
}

void AudioClipViewModel::setIsRepeating(bool isRepeating)
{
    if(m_isRepeating != isRepeating){
        m_isRepeating = isRepeating;
        isRepeatingChanged();
    }
}

int AudioClipViewModel::volume() const
{
    return m_volume;
}

void AudioClipViewModel::setVolume(int volume)
{
    if(m_volume != volume){
        m_volume = volume;
        m_player.setVolume(volume);
        volumeChanged();
    }
}

QUrl AudioClipViewModel::source() const
{
    return m_source;
}

void AudioClipViewModel::setSource(const QUrl &source)
{
    if(m_source != source){
        m_source = source;
        sourceChanged();
    }
}

// --------------------------------------------------------------------- private methods

void AudioClipViewModel::_initialize()
{
    QFile destinationFile(m_source.toLocalFile());
    destinationFile.open(QIODevice::ReadOnly);
    destinationFile.seek(0);
    _skipDataIfRequired(destinationFile);

    QMediaContent audioContent = QMediaContent(m_source);
    m_data = std::make_shared<QByteArray>(destinationFile.readAll());
    m_buffer = std::make_shared<QBuffer>(m_data.get());
    m_buffer->open(QIODevice::ReadOnly);
    m_buffer->seek(0);

    m_player.setMedia(audioContent, m_buffer.get());
    m_isInitialized = true;
}

bool AudioClipViewModel::_skipDataIfRequired(QFile& file)
{
    auto data = file.read(3);
    Q_ASSERT(data.size() == 3);
    if (data.size() != 3)
        return false;

    if (memcmp(data.constData(), "ID3", 3))
    {
        file.seek(0);
        return true;
    }

    // ID3v2 tag is detected; skip it

    file.seek(3+2+1);
    data = file.read(4);
    if (data.size() != 4)
        return false;

    qint32 size = (data[0] << (8*3)) | (data[1] << (8*2)) |
            (data[2] << 8) | data[3];

    file.seek(3+2+1+4+size);

    return true;
}

void AudioClipViewModel::_statusChanged()
{
    if(m_player.mediaStatus() == QMediaPlayer::EndOfMedia){
        if(m_isRepeating){
            m_player.play();
        }
    }
}

void AudioClipViewModel::_stateChanged()
{
    bool playing = m_player.state() == QMediaPlayer::PlayingState;
    setIsPlaying(playing);
}
