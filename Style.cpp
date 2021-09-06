#include "Style.h"

Style::Style()
{

}

QObject* Style::getQMLInstance([[maybe_unused]] QQmlEngine* engine, [[maybe_unused]] QJSEngine* scriptEngine)
{
	if (!_instance) _instance = new Style();
	return _instance;
}

int Style::getIndex()
{
	QSettings settings;
	const int index = settings.value(DATA_STYLE, 0).toInt();
	return index;
}

void Style::setIndex(const int index)
{
	QSettings settings;
	settings.setValue(DATA_STYLE, index);
	settings.sync();
}

const QColor Style::getSecondaryColor()
{
	auto savedStyle = static_cast<Style::style>(getIndex());
	return _secondaryColors[savedStyle];
}

const QColor Style::getSecondaryLightColor()
{
	auto savedStyle = static_cast<Style::style>(getIndex());
	return _secondaryLightColors[savedStyle];
}

const QColor Style::getSecondaryDarkColor()
{
	auto savedStyle = static_cast<Style::style>(getIndex());
	return _secondaryDarkColors[savedStyle];
}
