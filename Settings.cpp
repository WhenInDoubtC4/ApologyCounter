#include "Settings.h"

#include <QDebug>

Settings::Settings(QObject* parent) : QObject(parent)
{

}

void Settings::setWidgetName(const int index, const QString& name)
{
	_settings.beginGroup(DATA_SETTINGS);
	_settings.setValue(DATA_WIDGET "/" + QString::number(index), name);
	_settings.endGroup();
	_settings.sync();

#ifdef Q_OS_ANDROID
	AndroidUtils::getInstance()->requestUpdateWidget();
#endif
}

const QString Settings::getWidgetName(const int index)
{
	QSettings settings;
	settings.beginGroup(DATA_SETTINGS);
	const QString name = settings.value(DATA_WIDGET "/" + QString::number(index)).toString();
	settings.endGroup();
	return name;
}

const QString Settings::getWidgetDisplayName(const int index)
{
	QSettings settings;
	const QString name = getWidgetName(index);
	return settings.value(name + "/" DATA_DISPLAY_NAME).toString();
}

QVariant Settings::getWidgetSettings()
{
	QVariantList list;
	_settings.beginGroup(DATA_SETTINGS);
	list << _settings.value(DATA_WIDGET "/0").toString();
	list << _settings.value(DATA_WIDGET "/1").toString();
	_settings.endGroup();
	return list;
}
