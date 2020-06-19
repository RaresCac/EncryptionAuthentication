#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "userfilecreator.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    //Add instance of userFileCreator as global context property
    UserFileCreator* ufc = new UserFileCreator();
    engine.rootContext()->setContextProperty("ufc", ufc);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
