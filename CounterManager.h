#pragma once

#include <QObject>
#include <QSettings>

#if	defined(Q_OS_IOS)
#include "IOSUtils.h"
#elif defined(Q_OS_ANDROID)
#include "AndroidUtils.h"
#endif

#define DATA_ARRAY "counterData"
#define DATA_DISPLAY_NAME "displayName"
#define DATA_NAME "name"
#define DATA_COUNT "count"

class CounterManager : public QObject
{
	Q_OBJECT
public:
	explicit CounterManager(QObject* parent = nullptr);

	struct counterData
	{
		QString displayName;
		QString name;
		int count;
	};
	typedef QList<counterData> counterDataList;

    Q_INVOKABLE void displayNewCounterMessageBox();

	Q_INVOKABLE QVariant getCounters();

	void updateSettings();
    
    Q_INVOKABLE void clearSettingsForCounter(const QString& name);
    
    Q_INVOKABLE void swap(const int index1, const int index2);

private:
	QSettings _settings = QSettings("WhenInDoubtC4", "ApologyCounter");
	QList<counterData> _counters;

public slots:
	Q_INVOKABLE void createNewCounter(QString name);

signals:
	void displayNewCounterDialog();
	void addNewCounter(QString displayName, QString name);
};
