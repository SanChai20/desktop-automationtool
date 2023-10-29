#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QThreadPool>
#include <QFontDatabase>
//#include <QtWebEngine>

#include "gameapplicationenginewrapper.h"
#include "covertransparentwindow.h"
#include "covershowwindow.h"
#include "gamescriptruninstance.h"
#include "globalsettingsprofile.h"
#include "gamescriptclipboard.h"
 
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QThreadPool::globalInstance()->setMaxThreadCount(6);
    qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Material"));
    qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", QByteArray("Dark"));

    QGuiApplication app(argc, argv);
    //QtWebEngine::initialize();

#if QT_DEVELOPMENT
    const int font_id = QFontDatabase::addApplicationFont(":/font/Font Awesome 6 Free-Solid-900.otf");
    const int font_id2 = QFontDatabase::addApplicationFont(":/font/Font Awesome 6 Free-Regular-400.otf");
    const int font_id3 = QFontDatabase::addApplicationFont(":/font/Font Awesome 6 Brands-Regular-400.otf");
#else
	const int font_id = QFontDatabase::addApplicationFont("font/Font Awesome 6 Free-Solid-900.otf");
	const int font_id2 = QFontDatabase::addApplicationFont("font/Font Awesome 6 Free-Regular-400.otf");
	const int font_id3 = QFontDatabase::addApplicationFont("font/Font Awesome 6 Brands-Regular-400.otf");
#endif
    qDebug() << QFontDatabase::applicationFontFamilies(font_id);
    qDebug() << QFontDatabase::applicationFontFamilies(font_id2);
    qDebug() << QFontDatabase::applicationFontFamilies(font_id3);

    qmlRegisterType<GameScriptRunInstance>("GameScriptRunInstance", 1, 0, "GameScriptRunInstance");
    qmlRegisterType<Covertransparentwindow>("CoverTransparentWindow", 1, 0, "Covertransparentwindow");
    qmlRegisterType<CoverShowWindow>("CoverShowWindow", 1, 0, "CoverShowWindow");
    qmlRegisterType<GlobalSettingsProfile>("GlobalSettingsProfile", 1, 0, "GlobalSettingsProfile");
    qmlRegisterType<GameScriptClipBoard>("GameScriptClipBoard", 1, 0, "GameScriptClipBoard");

    qRegisterMetaType<QList<ObjectImageFeature>>("QList<ObjectImageFeature>");
    GameApplicationEngineWrapper wrapper(app);
    return app.exec();
}
