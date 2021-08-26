#pragma once

#include <QObject>
#include <QSettings>
#include <QDateTime>

#if defined(Q_OS_IOS)
#include "IOSUtils.h"
#elif defined(Q_OS_ANDROID)
#include "AndroidUtils.h"
#endif

#define DATA_COUNT "count"
#define DATA_ARRAY "counterData"
#define DATA_NAME "name"
#define DATA_TIME_DATA "timeData"
#define DATA_TIMESTAMP "timestamp"

class Counter : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int count READ getCount WRITE setCount NOTIFY countChanged)

public:
	explicit Counter(QObject* parent = nullptr);

	Q_INVOKABLE void setName(const QString& name);

	void reload();
	void saveData();

	int getCount();
	void setCount(const int& input);

	Q_INVOKABLE void incrementCount();
	Q_INVOKABLE void decrementCount(bool single = false);
    
    Q_INVOKABLE void displayDeleteCounterMessageBox(const QString& displayName, const QString& name);
    
private:
	int _count = 0;
	QString _name;
	QSettings _settings = QSettings("WhenInDoubtC4", "ApologyCounter");
    
#ifdef Q_OS_IOS
    bool _dialogOpen = false;
#endif

public slots:
    Q_INVOKABLE void deleteCounter();
    
signals:
	void countChanged();
    void displayDeleteCounterDialog();
    void deleteQmlCounter(const QString& name);
};

