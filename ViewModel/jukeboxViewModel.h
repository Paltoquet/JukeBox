#ifndef JUKEBOXVIEWMODEL_H
#define JUKEBOXVIEWMODEL_H

#include <QObject>
#include <QUrl>
#include <QDir>
#include <QQmlListProperty>

#include <memory>

class SoundData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl         path
        READ                path
        WRITE               setPath
        NOTIFY              pathChanged
    )

    Q_PROPERTY(QString      name
        READ                name
        NOTIFY              nameChanged
    )

    Q_PROPERTY(int          id
        READ                id
        WRITE               setId
    )

public:
    SoundData(QObject *parent = nullptr);
    SoundData(QFileInfo fileInfo, int id, QObject *parent = nullptr);

public:
    QUrl path() const;
    void setPath(const QUrl &path);

    QString name() const;

    int id() const;
    void setId(int id);

signals:
    void pathChanged();
    void nameChanged();

private:
    QUrl m_path;
    QString m_name;
    int m_id;
};

class JukeBoxViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl                         folder
        READ                                folder
        WRITE                               setFolder
    )

    Q_PROPERTY(QQmlListProperty<SoundData>  sounds
        READ                                soundsProperty
        NOTIFY                              soundsChanged
    )

public:
    explicit JukeBoxViewModel(QObject *parent = nullptr);

    QQmlListProperty<SoundData> soundsProperty() const;
    QUrl folder() const;
    void setFolder(const QUrl &folder);

private:
    static int soundCount(QQmlListProperty<SoundData>* listProperty);
    static SoundData* sound(QQmlListProperty<SoundData>* listProperty, int index);

    void _loadSoundData();

private:
    QQmlListProperty<SoundData> m_soundsProperty;
    QList<std::shared_ptr<SoundData>> m_sounds;
    QUrl m_folderPath;
    QDir m_folder;

signals:
    void soundsChanged();

public slots:
};

#endif // JUKEBOXVIEWMODEL_H
