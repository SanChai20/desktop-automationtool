#include "gameapplicationenginewrapper.h"
#include "colorimageprovider.h"
GameApplicationEngineWrapper::GameApplicationEngineWrapper(QGuiApplication& app, QObject *parent) : QObject(parent)
{
    m_rEngine.addImageProvider(QLatin1String("captures"), new ColorImageProvider);
#if QT_DEVELOPMENT
    const QUrl url(QStringLiteral("qrc:/main.qml"));
#else
    const QUrl url(QStringLiteral("main.qml"));
#endif
    QObject::connect(&m_rEngine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    m_rEngine.load(url);
}
