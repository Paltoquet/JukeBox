#ifndef AUDIOCLIPVIEWMODEL_H
#define AUDIOCLIPVIEWMODEL_H

#include <QMediaPlayer>
#include <QTimer>
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

    Q_PROPERTY(bool      isRandomizing
        READ             isRandomizing
        WRITE            setIsRandomizing
        NOTIFY           isRandomizingChanged
    )

    Q_PROPERTY(bool      isPlaying
        READ             isPlaying
        WRITE            setIsPlaying
        NOTIFY           isPlayingChanged
    )

    Q_PROPERTY(bool      isWaiting
        READ             isWaiting
        NOTIFY           isWaitingChanged
    )

public:
    AudioClipViewModel(QObject* parent = nullptr);
    ~AudioClipViewModel();

private:
    QUrl m_source;
    int m_volume;
    bool m_isRepeating;
    bool m_isRandomizing;

    bool m_isPlaying;
    bool m_isWaiting;
    bool m_isInitialized;
    QMediaPlayer m_player;
    QTimer m_randomTimer;
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

    bool isRandomizing() const;
    void setIsRandomizing(bool isRandomizing);

    bool isPlaying() const;
    void setIsPlaying(bool isPlaying);

    bool isWaiting() const;

signals:
    void sourceChanged();
    void volumeChanged();
    void isRepeatingChanged();
    void isRandomizingChanged();
    void isPlayingChanged();
    void isWaitingChanged();

private slots:
    void _statusChanged();
    void _stateChanged();
    void _startClip();

private:
    void _initialize();
    bool _skipDataIfRequired(QFile& file);
    void _setIsWaiting(bool isWaiting);
    void _stopTimer();
    float _generateRandomInterval();
};

#endif // AUDIOCLIPVIEWMODEL_H
