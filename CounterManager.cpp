#include "CounterManager.h"

#include <QDebug>

CounterManager::CounterManager(QObject* parent) : QObject(parent)
{
#if defined(Q_OS_IOS)
    QObject::connect(IOSUtils::getInstance(), &IOSUtils::createNewCounter, this, &CounterManager::createNewCounter);
#elif defined(Q_OS_ANDROID)
	QObject::connect(AndroidUtils::getInstance(), &AndroidUtils::createNewCounter, this, &CounterManager::createNewCounter);
#endif

	QList<QString> crispyTimestapmps = QList<QString>{}
		<< "2021-08-19T17:48:00"
		<< "2021-08-19T17:39:00"
		<< "2021-08-19T16:41:00"
		<< "2021-08-19T13:47:00"
		<< "2021-08-14T12:46:00"
		<< "2021-08-13T21:54:00"
		<< "2021-08-13T21:51:00"
		<< "2021-08-11T12:23:00"
		<< "2021-08-10T17:38:00"
		<< "2021-08-10T01:22:00"
		<< "2021-08-06T22:36:00"
		<< "2021-08-06T20:36:00"
		<< "2021-08-05T19:36:00"
		<< "2021-08-05T12:52:00"
		<< "2021-08-04T01:14:00"
		<< "2021-08-04T01:13:00"
		<< "2021-08-03T21:37:00"
		<< "2021-08-03T13:06:00"
		<< "2021-08-02T19:01:00"
		<< "2021-08-02T18:02:00"
		<< "2021-08-01T20:36:00"
		<< "2021-08-01T01:03:00"
		<< "2021-08-01T00:17:00"
		<< "2021-07-27T15:23:00"
		<< "2021-07-27T15:17:00"
		<< "2021-07-26T20:28:00"
		<< "2021-07-26T14:15:00"
		<< "2021-07-26T13:18:00"
		<< "2021-07-25:19:12:00"
		<< "2021-07-25T15:58:00"
		<< "2021-07-25T15:47:00"
		<< "2021-07-25T15:10:00"
		<< "2021-07-25T12:18:00"
		<< "2021-07-25T01:40:00"
		<< "2021-07-24T22:49:00"
		<< "2021-07-24T19:39:00"
		<< "2021-07-24T17:00:00"
		<< "2021-07-23T01:45:00"
		<< "2021-07-22T17:31:00"
		<< "2021-07-22T16:09:00"
		<< "2021-07-20T13:44:00"
		<< "2021-07-20T13:43:00"
		<< "2021-07-20T12:41:00"
		<< "2021-07-20T12:20:00"
		<< "2021-07-19T20:50:00"
		<< "2021-07-19T13:44:00"
		<< "2021-07-18T19:00:00"
		<< "2021-07-18T13:30:00"
		<< "2021-07-14T21:12:00"
		<< "2021-07-14T13:18:00"
		<< "2021-07-14T13:14:00"
		<< "2021-07-13T21:42:00"
		<< "2021-07-11T22:16:00"
		<< "2021-07-09T22:50:00"
		<< "2021-07-08T01:01:00"
		<< "2021-07-08T00:59:00"
		<< "2021-07-07T20:57:00"
		<< "2021-07-07T20:04:00"
		<< "2021-07-07T18:52:00"
		<< "2021-07-07T18:50:00"
		<< "2021-07-07T18:35:00"
		<< "2021-07-04T05:55:00"
		<< "2021-07-02T20:41:00"
		<< "2021-06-25T18:14:00"
		<< "2021-06-25T17:14:00"
		<< "2021-06-25T13:38:00"
		<< "2021-06-25T10:28:00"
		<< "2021-06-24T22:58:00"
		<< "2021-06-24T22:57:00";
		_settings.beginGroup("Crispy");
		_settings.setValue(DATA_COUNT, 69);
		_settings.setValue(DATA_DISPLAY_NAME, "Crispy");
		_settings.beginWriteArray(DATA_TIME_DATA);
		for (int i = 0; i < 69; i++)
		{
			_settings.setArrayIndex(i);
			_settings.setValue(DATA_TIMESTAMP, crispyTimestapmps[crispyTimestapmps.size() - 1 - i]);
		}
		_settings.endArray();
		_settings.endGroup();

		auto miloData = QList<QString>{}
							<< "2021-08-27T12:00:00"
							<< "2021-08-26T12:00:00"
							<< "2021-08-26T12:00:00"
							<< "2021-08-26T12:00:00"
							<< "2021-08-26T12:00:00"
							<< "2021-08-25T12:00:00"
							<< "2021-08-25T12:00:01"
							<< "2021-08-25T12:00:02"
							<< "2021-08-25T12:00:03"
							<< "2021-08-24T12:00:00"
							<< "2021-08-24T12:00:01"
							<< "2021-08-23T12:00:00"
							<< "2021-08-23T12:00:01"
							<< "2021-08-23T12:00:02"
							<< "2021-08-22T12:00:00"
							<< "2021-08-22T12:00:01"
							<< "2021-08-20T12:00:00"
							<< "2021-08-19T12:00:00"
							<< "2021-08-19T12:00:01"
							<< "2021-08-19T12:00:02"
							<< "2021-08-18T12:00:00"
							<< "2021-08-18T12:00:01"
							<< "2021-08-18T12:00:02"
							<< "2021-08-18T12:00:03"
							<< "2021-08-18T12:00:04"
							<< "2021-08-18T12:00:05"
							<< "2021-08-17T12:00:00"
							<< "2021-08-17T12:00:01"
							<< "2021-08-17T12:00:02"
							<< "2021-08-17T12:00:03"
							<< "2021-08-17T12:00:04"
							<< "2021-08-17T12:00:05"
							<< "2021-08-17T12:00:06"
							<< "2021-08-17T12:00:07"
							<< "2021-08-17T12:00:08"
							<< "2021-08-17T12:00:09"
							<< "2021-08-16T12:00:00"
							<< "2021-08-16T12:00:01"
							<< "2021-08-16T12:00:02"
							<< "2021-08-16T12:00:03"
							<< "2021-08-15T12:00:00"
							<< "2021-08-15T12:00:01"
							<< "2021-08-14T12:00:00"
							<< "2021-08-14T12:00:01"
							<< "2021-08-14T12:00:02"
							<< "2021-08-14T12:00:03"
							<< "2021-08-13T12:00:00"
							<< "2021-08-13T12:00:01"
							<< "2021-08-13T12:00:02"
							<< "2021-08-13T12:00:03"
							<< "2021-08-13T12:00:04"
							<< "2021-08-13T12:00:05"
							<< "2021-08-12T12:00:00"
							<< "2021-08-12T12:00:01"
							<< "2021-08-11T12:00:00"
							<< "2021-08-11T12:00:01"
							<< "2021-08-11T12:00:02"
							<< "2021-08-11T12:00:03"
							<< "2021-08-11T12:00:04"
							<< "2021-08-11T12:00:05"
							<< "2021-08-10T12:00:00"
							<< "2021-08-10T12:00:01"
							<< "2021-08-10T12:00:02"
							<< "2021-08-10T12:00:03"
							<< "2021-08-10T12:00:04"
							<< "2021-08-10T12:00:05"
							<< "2021-08-10T12:00:06"
							<< "2021-08-08T12:00:00"
							<< "2021-08-08T12:00:01"
							<< "2021-08-08T12:00:02"
							<< "2021-08-08T12:00:03"
							<< "2021-08-08T12:00:04"
							<< "2021-08-08T12:00:05"
							<< "2021-08-08T12:00:06"
							<< "2021-08-08T12:00:07"
							<< "2021-08-08T12:00:08"
							<< "2021-08-07T12:00:00"
							<< "2021-08-07T12:00:01"
							<< "2021-08-07T12:00:02";

			_settings.beginGroup("Milo");
			_settings.beginWriteArray(DATA_TIME_DATA);
			for (int i = 0; i < 79; i++)
			{
				_settings.setArrayIndex(i);
				_settings.setValue(DATA_TIMESTAMP, miloData[miloData.size() - i - 1]);
			}
			_settings.endArray();
			_settings.setValue(DATA_COUNT, 79);
			_settings.setValue(DATA_DISPLAY_NAME, "Milo");
			_settings.endGroup();



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
