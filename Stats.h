#pragma once

#include <QObject>
#include <QtQuick>
#include <QtWidgets>
#include <QtCharts>
#include <QSettings>

#include "Global.h"
#include "Style.h"

#ifdef Q_OS_ANDROID
#include "AndroidUtils.h"
#endif

class Stats : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(chartRange rangeSelected READ getRangeSelected WRITE setRangeSelected NOTIFY rangeSelectedChanged)
public:
	explicit Stats(QObject* parent = nullptr, bool widget = false);
    
    enum chartRange
    {
        CHART_RANGE_WEEK,
        CHART_RANGE_MONTH,
        CHART_RANGE_YEAR
    };
    Q_ENUM(chartRange)
    
	Q_INVOKABLE void updateChart(QQuickItem* chartView);

	QString getName() { return _name; };
    
    chartRange getRangeSelected() { return _chartRangeSelected; };
    void setRangeSelected(chartRange range) { _chartRangeSelected = range; };
    
    Q_INVOKABLE bool isRangeAllowed(chartRange range) { return _chartRangeAllowed[range]; };
    
    Q_INVOKABLE QVariant getRangePoints();
    Q_INVOKABLE void setRangePointIndex(const int index) { _rangePointIndex = index; };

public slots:
	void setName(const QString& name) { _name = name; };
	void saveWidgetChart(const QString& fileName);
    
private:
	QSettings _settings;
    QString _name;
	QMap<chartRange, bool> _chartRangeAllowed
    {
        {chartRange::CHART_RANGE_WEEK, false},
        {chartRange::CHART_RANGE_MONTH, false},
        {chartRange::CHART_RANGE_YEAR, false}
    };
    chartRange _chartRangeSelected = chartRange::CHART_RANGE_WEEK;
    
    QMap<chartRange, QList<QMap<QDate, int>>> _rangeData;
    int _rangePointIndex = 0;
    
    void updateChartRanges();
    
signals:
    void nameChanged();
    void rangeSelectedChanged();
};

