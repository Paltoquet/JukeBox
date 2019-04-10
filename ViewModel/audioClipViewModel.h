#ifndef AUDIOCLIPVIEWMODEL_H
#define AUDIOCLIPVIEWMODEL_H

#include <QMediaPlayer>
#include <QBuffer>
#include <QFile>

#include <memory>

class AudioClipViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl     source
        READ            source
        WRITE           setSource
        NOTIFY          sourceChanged
    )

    Q_PROPERTY(int      volume
        READ            volume
        WRITE           setVolume
        NOTIFY          volumeChanged
    )

    Q_PROPERTY(bool      isRepeating
        READ             isRepeating
        WRITE            setIsRepeating
        NOTIFY           isRepeatingChanged
    )

    Q_PROPERTY(bool      isPlaying
        READ             isPlaying
        WRITE            setIsPlaying
        NOTIFY           isPlayingChanged
    )

public:
    AudioClipViewModel(QObject* parent = nullptr);

private:
    QUrl m_source;
    int m_volume;
    bool m_isRepeating;

    bool m_isPlaying;
    bool m_isInitialized;
    QMediaPlayer m_player;
    std::shared_ptr<QBuffer> m_buffer;
    std::shared_ptr<QByteArray> m_data;


public:
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();

    QUrl source() const;
    void setSource(const QUrl &source);

    int volume() const;
    void setVolume(int volume);

    bool isRepeating() const;
    void setIsRepeating(bool isRepeating);

    bool isPlaying() const;
    void setIsPlaying(bool isPlaying);

signals:
    void sourceChanged();
    void volumeChanged();
    void isRepeatingChanged();
    void isPlayingChanged();

private slots:
    void _statusChanged();
    void _stateChanged();

private:
    void _initialize();
    bool _skipDataIfRequired(QFile& file);
};

#endif // AUDIOCLIPVIEWMODEL_H
