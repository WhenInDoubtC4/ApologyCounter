#include "CounterManager.h"

#include <QDebug>

CounterManager::CounterManager(QObject* parent) : QObject(parent)
{
#if defined(Q_OS_IOS)
    QObject::connect(IOSUtils::getInstance(), &IOSUtils::createNewCounter, this, &CounterManager::createNewCounter);
#elif defined(Q_OS_ANDROID)
	QObject::connect(AndroidUtils::getInstance(), &AndroidUtils::createNewCounter, this, &CounterManager::createNewCounter);
#endif

	//Load from settings
	//Load names
	const int counterCount = _settings.beginReadArray(DATA_ARRAY);
	QList<QString> names;
	for (int i = 0; i < counterCount; i++)
	{
		_settings.setArrayIndex(i);
		names << _settings.value(DATA_NAME).toString();
	}
	_settings.endArray();
	//Load all data
	for (const QString& name : qAsConst(names))
	{
		_settings.beginGroup(name);
		counterData data;
		data.name = name;
		data.displayName = _settings.value(DATA_DISPLAY_NAME).toString();
		data.count = _settings.value(DATA_COUNT).toInt();
		_counters << data;
		_settings.endGroup();
	}
}

QVariant CounterManager::getCounters()
{
	QVariantList list;
	for (const counterData& data : qAsConst(_counters))
	{
		QVariantMap variant;
		variant.insert("count", data.count);
		variant.insert("displayName", data.displayName);
		variant.insert("name", data.name);
		list << variant;
	}
	return list;
}

void CounterManager::displayNewCounterMessageBox()
{
#if	defined(Q_OS_IOS)
    IOSUtils::getInstance()->displayNewCounterMessageBox();
#elif defined(Q_OS_ANDROID)
	AndroidUtils::getInstance()->displayNewCounterMessageBox();
#else
	emit displayNewCounterDialog();
#endif
}

void CounterManager::updateSettings()
{
	//Add names to array
	_settings.beginWriteArray(DATA_ARRAY);
	for (int i = 0; i < _counters.size(); i++)
	{
		_settings.setArrayIndex(i);
		_settings.setValue(DATA_NAME, _counters[i].name);
	}
	_settings.endArray();
	_settings.sync();

	//Write data for all new counters
	for (const counterData& data : qAsConst(_counters))
	{
        //Skip existing entries
        _settings.beginGroup(data.name);
        if (_settings.contains(DATA_COUNT))
        {
            _settings.endGroup();
            continue;
        }
		_settings.setValue(DATA_DISPLAY_NAME, data.displayName);
		_settings.setValue(DATA_COUNT, data.count);
		_settings.endGroup();
	}

	_settings.sync();
}

void CounterManager::createNewCounter(QString name)
{
	if (name.isEmpty()) return;

	QString safeName = name.normalized(QString::NormalizationForm::NormalizationForm_KC).toHtmlEscaped();
	counterData newCounter;
	newCounter.displayName = name;
	newCounter.name = safeName;
	newCounter.count = 0;
	_counters << newCounter;

	updateSettings();
	emit addNewCounter(name, safeName);
}

void CounterManager::clearSettingsForCounter(const QString &name)
{
    int index = -1;
    for (int i = 0; i < _counters.size(); i++)
    {
        if (_counters[i].name == name)
        {
            index = i;
            break;
        }
    }
    if (index == -1) throw std::runtime_error("Counter deletion out of range");
    _counters.removeAt(index);
    
    //Update array
    updateSettings();
    
    //Remove name
    _settings.beginGroup(name);
    _settings.remove("");
    _settings.endGroup();
}

void CounterManager::swap(const int index1, const int index2)
{
	int arraySize = _settings.beginReadArray(DATA_ARRAY);
    QList<QString> names;
    for (int i = 0; i < arraySize; i++)
    {
        _settings.setArrayIndex(i);
        names << _settings.value(DATA_NAME).toString();
    }
	_settings.endArray();
    
	_settings.beginWriteArray(DATA_ARRAY);
    for (int i = 0; i < names.size(); i++)
    {
        _settings.setArrayIndex(i);
        _settings.setValue(DATA_NAME, names[i]);
    }
	_settings.endArray();

	_settings.sync();
}
