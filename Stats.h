#pragma once

#include <QObject>
#include <QtQuick>
#include <QtWidgets>
#include <QtCharts>
#include <QSettings>

#define STYLE_TEXT 0xfafafa
#define STYLE_PRIMARY_LIGHT 0x4f5b62
#define STYLE_SECONDARY 0x03a9f4
#define STYLE_SECONDARY_LIGHT 0x67daff
#define STYLE_SECONDARY_DARK 0x007ac1

#define DATA_TIME_DATA "timeData"
#define DATA_TIMESTAMP "timestamp"

class Stats : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(chartRange rangeSelected READ getRangeSelected WRITE setRangeSelected NOTIFY rangeSelectedChanged)
public:
	explicit Stats(QObject* parent = nullptr);
    
    enum chartRange
    {
        CHART_RANGE_WEEK,
        CHART_RANGE_MONTH,
        CHART_RANGE_YEAR
    };
    Q_ENUM(chartRange)
    
	Q_INVOKABLE void updateChart(QQuickItem* chartView);

	QString getName() { return _name; };
    void setName(const QString& name) { _name = name; };
    
    chartRange getRangeSelected() { return _chartRangeSelected; };
    void setRangeSelected(chartRange range) { _chartRangeSelected = range; };
    
    Q_INVOKABLE bool isRangeAllowed(chartRange range) { return _chartRangeAllowed[range]; };
    
    Q_INVOKABLE QVariant getRangePoints();
    Q_INVOKABLE void setRangePointIndex(const int index) { _rangePointIndex = index; };
    
private:
    QSettings _settings = QSettings("WhenInDoubtC4", "ApologyCounter");
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
