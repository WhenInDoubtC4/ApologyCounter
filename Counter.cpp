#include "Counter.h"

#include <QDebug>

Counter::Counter(QObject* parent) : QObject(parent)
{
#if defined(Q_OS_IOS)
    QObject::connect(IOSUtils::getInstance(), &IOSUtils::deleteCounter, this, &Counter::deleteCounter);
#elif defined(Q_OS_ANDROID)
	QObject::connect(AndroidUtils::getInstance(), &AndroidUtils::deleteCounter, this, &Counter::deleteCounter);
#endif
}

void Counter::setName(const QString& name)
{
	_name = name;
	reload();
}

void Counter::reload()
{
	_settings.beginGroup(_name);
	_count = _settings.value(DATA_COUNT).toInt();
	_settings.endGroup();

	emit countChanged();
}

void Counter::saveData()
{
	_settings.beginGroup(_name);
	_settings.setValue(DATA_COUNT, _count);
	_settings.endGroup();
}

int Counter::getCount()
{
	return _count;
}

void Counter::setCount(const int& input)
{
	_count = input;
	emit countChanged();
}

void Counter::incrementCount()
{
	_count++;
	emit countChanged();
	saveData();

    _settings.beginGroup(_name);
    const int size = _settings.value(DATA_TIME_DATA "/size").toInt();
    _settings.beginWriteArray(DATA_TIME_DATA);
    _settings.setArrayIndex(size);
    _settings.setValue(DATA_TIMESTAMP, QDateTime::currentDateTime().toString(Qt::ISODate));
    _settings.endArray();
    _settings.endGroup();
    
#ifdef Q_OS_IOS
	IOSUtils::playImpactHapticFeedback(IOSUtils::ImpactFeedbackStyle::MEDIUM);
#endif
}

void Counter::decrementCount(bool single)
{
	if (_count == 1) return;

    const int decrement = single ? 1 : 2;
    _count -= decrement;
	emit countChanged();
	saveData();
    
    _settings.beginGroup(_name);
    const int size = _settings.value(DATA_TIME_DATA "/size").toInt();
    qDebug() << size;
    _settings.setValue(DATA_TIME_DATA "/size", size - decrement);
    _settings.endGroup();

#ifdef Q_OS_IOS
	IOSUtils::playNotificationHapticFeedback(IOSUtils::NotificationFeedbackType::WARNING);
#endif
}

void Counter::displayDeleteCounterMessageBox(const QString &displayName, const QString &name)
{
#if defined(Q_OS_IOS)
    IOSUtils::getInstance()->displayDeleteCounterMessageBox(displayName, name);
    _dialogOpen = true;
#elif defined(Q_OS_ANDROID)
	AndroidUtils::getInstance()->displayDeleteCounterMessageBox(displayName);
#else
    emit displayDeleteCounterDialog();
#endif
}

void Counter::deleteCounter()
{
#ifdef Q_OS_IOS
    if (!_dialogOpen) return;
    _dialogOpen = false;
#endif
    
    emit deleteQmlCounter(_name);
}
