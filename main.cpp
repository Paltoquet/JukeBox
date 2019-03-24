#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QtDebug>
#include "ViewModel/jukeboxViewModel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    app.setOrganizationName("WestFolk");
    app.setOrganizationDomain("WestFolk.com");
    app.setApplicationName("JukeBox");

    qmlRegisterType<JukeBoxViewModel>("JukeBox", 1, 0, "JukeBoxViewModel");
    qmlRegisterType<SoundData>("JukeBox", 1, 0, "SoundData");

    QFontDatabase fontDatabase;
    if (fontDatabase.addApplicationFont(":/jukebox.ttf") == -1) {
        qWarning() << "Failed to load fontello.ttf";
    }
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
