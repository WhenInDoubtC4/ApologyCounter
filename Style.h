#pragma once

#include <QObject>
#include <QSettings>
#include <QColor>
#include <QQmlEngine>
#include <QJSEngine>

#include "Global.h"

class Style : public QObject
{
	Q_OBJECT
public:
	enum style
	{
		BLUE,
		GREEN,
		PURPLE,
		PINK,
		RED,
		YELLOW,
		ORANGE,
		GREY
	};
	Q_ENUM(style)

	static QObject* getQMLInstance(QQmlEngine* engine, QJSEngine* scriptEngine);

	Q_INVOKABLE static int getIndex();
	Q_INVOKABLE static void setIndex(const int index);

	Q_INVOKABLE static const QColor getSecondaryColor();
	Q_INVOKABLE static const QColor getSecondaryLightColor();
	Q_INVOKABLE static const QColor getSecondaryDarkColor();

private:
	Style();

	inline static Style* _instance = nullptr;

	inline static const QMap<style, QColor> _secondaryColors
	{
		{style::BLUE, QColor(STYLE_BLUE_SECONDARY)},
		{style::GREEN, QColor(STYLE_GREEN_SECONDARY)},
		{style::PURPLE, QColor(STYLE_PURPLE_SECONDARY)},
		{style::PINK, QColor(STYLE_PINK_SECONDARY)},
		{style::RED, QColor(STYLE_RED_SECONDARY)},
		{style::YELLOW, QColor(STYLE_YELLOW_SECONDARY)},
		{style::ORANGE, QColor(STYLE_ORANGE_SECONDARY)},
		{style::GREY, QColor(STYLE_GREY_SECONDARY)}
	};

	inline static const QMap<style, QColor> _secondaryLightColors
	{
		{style::BLUE, QColor(STYLE_BLUE_SECONDARY_LIGHT)},
		{style::GREEN, QColor(STYLE_GREEN_SECONDARY_LIGHT)},
		{style::PURPLE, QColor(STYLE_PURPLE_SECONDARy_LIGHT)},
		{style::PINK, QColor(STYLE_PINK_SECONDARY_LIGHT)},
		{style::RED, QColor(STYLE_RED_SECONDARY_LIGHT)},
		{style::YELLOW, QColor(STYLE_YELLOW_SECONDARY_LIGHT)},
		{style::ORANGE, QColor(STYLE_ORANGE_SECONDARY_LIGHT)},
		{style::GREY, QColor(STYLE_GREY_SECONDARY_LIGHT)}
	};

	inline static const QMap<style, QColor> _secondaryDarkColors
	{
		{style::BLUE, QColor(STYLE_BLUE_SECONDARY_DARK)},
		{style::GREEN, QColor(STYLE_GREEN_SECONDARY_DARK)},
		{style::PURPLE, QColor(STYLE_PURPLE_SECONDARY_DARK)},
		{style::PINK, QColor(STYLE_PINK_SECONDARY_DARK)},
		{style::RED, QColor(STYLE_RED_SECONDARY_DARK)},
		{style::YELLOW, QColor(STYLE_YELLOW_SECONDARY_DARK)},
		{style::ORANGE, QColor(STYLE_ORANGE_SECONDARY_DARK)},
		{style::GREY, QColor(STYLE_GREY_SECONDARY_DARK)}
	};
};

