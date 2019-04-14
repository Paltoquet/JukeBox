#include "audioClipViewModel.h"
#include <QMediaContent>
#include <QDateTime>

#include <random>

// --------------------------------------------------------------------- JukeBoxViewModel

AudioClipViewModel::AudioClipViewModel(QObject* parent):
    QObject(parent),
    m_source(QUrl("")),
    m_volume(50),
    m_isRepeating(false),
    m_isPlaying(false),
    m_isWaiting(false),
    m_isInitialized(false)
{
    m_randomTimer.setSingleShot(true);
    QObject::connect(&m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioClipViewModel::_statusChanged);
    QObject::connect(&m_player, &QMediaPlayer::stateChanged, this, &AudioClipViewModel::_stateChanged);
    QObject::connect(&m_randomTimer, SIGNAL(timeout()), this, SLOT(_startClip()));
}

AudioClipViewModel::~AudioClipViewModel()
{
    m_player.stop();
    m_randomTimer.stop();
    QObject::disconnect(&m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioClipViewModel::_statusChanged);
    QObject::disconnect(&m_player, &QMediaPlayer::stateChanged, this, &AudioClipViewModel::_stateChanged);
    QObject::disconnect(&m_randomTimer, SIGNAL(timeout()), this, SLOT(_startClip()));
}

void AudioClipViewModel::play()
{
    if(!m_isInitialized){
        _initialize();
    }
    _startClip();
}

void AudioClipViewModel::pause()
{
    _setIsWaiting(false);
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

bool AudioClipViewModel::isWaiting() const
{
    return m_isWaiting;
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

bool AudioClipViewModel::isRandomizing() const
{
    return m_isRandomizing;
}

void AudioClipViewModel::setIsRandomizing(bool isRandomizing)
{
    if(m_isRandomizing != isRandomizing){
        m_isRandomizing = isRandomizing;
        _setIsWaiting(m_isRandomizing && m_isPlaying);
        isRandomizingChanged();
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

void AudioClipViewModel::_statusChanged()
{
    if(m_player.mediaStatus() == QMediaPlayer::EndOfMedia){
        if(m_isRepeating){
            _startClip();
        }
        else if(m_isRandomizing){
            auto interval = _generateRandomInterval();
            interval = interval * 1000.0f;
            _setIsWaiting(true);
            m_randomTimer.setInterval(int(interval));
            m_randomTimer.start();
        }
    }
}

void AudioClipViewModel::_stateChanged()
{
    bool playing = m_player.state() == QMediaPlayer::PlayingState;
    setIsPlaying(playing);
}

void AudioClipViewModel::_startClip()
{
    m_player.play();
}

void AudioClipViewModel::_stopTimer()
{
    if(m_randomTimer.isActive()){
        m_randomTimer.stop();
        setIsPlaying(false);
    }
}

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

    qint32 size = (data[0] << (7*3)) | (data[1] << (7*2)) |
            (data[2] << 7) | data[3];

    file.seek(3+2+1+4+size);

    return true;
}

void AudioClipViewModel::_setIsWaiting(bool isWaiting)
{
    if(m_isWaiting != isWaiting){
        m_isWaiting = isWaiting;
        if(!m_isWaiting && m_randomTimer.isActive()){
            m_randomTimer.stop();
        }
        isWaitingChanged();
    }
}

float AudioClipViewModel::_generateRandomInterval()
{
    auto mean = 12.0;
    auto variance = 6.0;
    auto seed = QDateTime::currentDateTime().toTime_t();
    std::default_random_engine randomGenerator(seed);
    std::normal_distribution<double> distribution(mean,sqrt(variance));
    auto result = distribution(randomGenerator);
    return float(result);
}
