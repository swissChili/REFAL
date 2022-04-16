#include "IdeMain.h"

#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QTranslator>

#include "CellModel.h"

int ideMain(Application *app)
{
    QQmlApplicationEngine engine;

    QQuickStyle::setStyle("Material");

    qRegisterMetaType<CellModel>();
    qRegisterMetaType<CellModel *>();

    QTranslator translator;
    qInfo() << "loading translations" << translator.load(QLocale(), "refal", "_", ":/ts/", ".qm");
    app->installTranslator(&translator);

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
