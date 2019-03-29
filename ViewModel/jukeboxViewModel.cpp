#include "jukeboxViewModel.h"
#include <QtDebug>
#include <QFileInfo>
#include <QMediaPlayer>

// --------------------------------------------------------------------- JukeBoxViewModel

JukeBoxViewModel::JukeBoxViewModel(QObject *parent) : QObject(parent)
{
    m_soundsProperty = QQmlListProperty<SoundData>(this, this,
                                                &JukeBoxViewModel::soundCount,
                                                &JukeBoxViewModel::sound);
}

QQmlListProperty<SoundData> JukeBoxViewModel::soundsProperty() const
{
    return m_soundsProperty;
}

QUrl JukeBoxViewModel::folder() const
{
    return m_folderPath;
}

void JukeBoxViewModel::setFolder(const QUrl &folder)
{
    m_folderPath = folder;
    m_folder = QDir(m_folderPath.toLocalFile());
    /*m_player.setMedia(QUrl::fromLocalFile("D:/sound/kern/combat/porte.mp3"));
    m_player.setVolume(50);
    m_player.play();
    connect(&m_player, &QMediaPlayer::mediaStatusChanged, this, [this](){
        qDebug() << m_player.mediaStatus();
    });*/
    _loadSoundData();
}

void JukeBoxViewModel::_loadSoundData()
{
    m_sounds.clear();
    QStringList filters;
    filters << "*.wav" << "*.mp3" << "*.flac";
    m_folder.setNameFilters(filters);
    auto files = m_folder.entryInfoList();
    int id = 0;
    for(auto& file : files){
        auto soundElem = std::make_shared<SoundData>(file, id, this);
        m_sounds.append(soundElem);
        id++;
    }
    soundsChanged();
}

int JukeBoxViewModel::soundCount(QQmlListProperty<SoundData> *listProperty)
{
    auto jukeBox = reinterpret_cast<JukeBoxViewModel*>(listProperty->data);
    return jukeBox->m_sounds.size();
}

SoundData *JukeBoxViewModel::sound(QQmlListProperty<SoundData> *listProperty, int index)
{
    auto jukeBox = reinterpret_cast<JukeBoxViewModel*>(listProperty->data);
    return jukeBox->m_sounds[index].get();
}

// --------------------------------------------------------------------- SoundData

SoundData::SoundData(QObject *parent):
    QObject(parent),
    m_path(""),
    m_id(-1),
    m_name("")
{

}

SoundData::SoundData(QFileInfo fileInfo, int id, QObject *parent):
    QObject(parent),
    m_path(fileInfo.filePath()),
    m_name(fileInfo.baseName()),
    m_id(id)
{

}

QUrl SoundData::path() const
{
    return m_path;
}

void SoundData::setPath(const QUrl &path)
{
    m_path = path;
    pathChanged();
}

QString SoundData::name() const
{
    return m_name;
}

int SoundData::id() const
{
    return m_id;
}

void SoundData::setId(int id)
{
    m_id = id;
}
