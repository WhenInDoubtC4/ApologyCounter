#pragma once

#include <QObject>
#include <QSettings>

#include "Global.h"

#ifdef Q_OS_ANDROID
#include "AndroidUtils.h"
#endif

class Settings : public QObject
{
	Q_OBJECT
public:
	explicit Settings(QObject* parent = nullptr);

	Q_INVOKABLE void setWidgetName(const int index, const QString& name);
	static const QString getWidgetName(const int index);
	static const QString getWidgetDisplayName(const int index);

	Q_INVOKABLE QVariant getWidgetSettings();

private:
	QSettings _settings;

signals:
	void initWidget(int index, const QString displayName);
};

