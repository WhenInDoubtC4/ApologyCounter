#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>

#include "Global.h"

#include "Counter.h"
#include "CounterManager.h"
#include "Stats.h"
#include "Settings.h"
#include "Style.h"

int main(int argc, char* argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QApplication app(argc, argv);

	QApplication::setApplicationName(SETTINGS_APP_NAME);
	QApplication::setOrganizationName(SETTINGS_ORGANIZATION_NAME);

#ifdef Q_OS_ANDROID
	QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
#endif

	qmlRegisterSingletonType(QUrl("qrc:/Style.qml"), "style", 1, 0, "Style");

	qmlRegisterType<Counter>("counter", 1, 0, "CounterBackend");
	qmlRegisterType<CounterManager>("counterManager", 1, 0, "CounterManager");
	qmlRegisterType<Stats>("stats", 1, 0, "StatsBackend");
	qmlRegisterType<Settings>("settings", 1, 0, "SettingsBacked");
	qmlRegisterSingletonType<Style>("styleBackend", 1, 0, "StyleBackend", &Style::getQMLInstance);

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);

#ifdef Q_OS_ANDROID
	Stats widgetStats(nullptr, true);
#endif

	return app.exec();
}
