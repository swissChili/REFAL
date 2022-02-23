#include "IdeMain.h"

#include <QQmlApplicationEngine>
#include <QQuickStyle>

int ideMain(QGuiApplication *app)
{
    QQmlApplicationEngine engine;

    // This is done implicitly now.
    // registerTypes(&engine);

    QQuickStyle::setStyle("Material");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     app, [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app->exec();
}