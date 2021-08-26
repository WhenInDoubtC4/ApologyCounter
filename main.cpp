#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Counter.h"
#include "CounterManager.h"
#include "Stats.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QApplication app(argc, argv);

	QApplication::setApplicationName("ApologyCounter");
	QApplication::setOrganizationName("WhenInDoubtC4");

	qmlRegisterSingletonType(QUrl("qrc:/Style.qml"), "style", 1, 0, "Style");

	qmlRegisterType<Counter>("counter", 1, 0, "CounterBackend");
	qmlRegisterType<CounterManager>("counterManager", 1, 0, "CounterManager");
	qmlRegisterType<Stats>("stats", 1, 0, "StatsBackend");

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
