#include "Stats.h"

#include <QDebug>

Stats::Stats(QObject* parent) : QObject(parent)
{
    
}
 
void Stats::updateChart(QQuickItem* chartView)
{
    //Extract qml chart
    QGraphicsScene* scene = chartView->findChild<QGraphicsScene*>();
    QChart* chart = nullptr;
    for (QGraphicsItem* item : scene->items())
    {
        chart = dynamic_cast<QChart*>(item);
        if (chart) break;
    }
    if (!chart) throw std::runtime_error("Can't find chart");
        
    //Set up charts
    updateChartRanges();
    
    //Clear previous data
    chart->removeAllSeries();
    for (QAbstractAxis* axis : chart->axes()) chart->removeAxis(axis);
    
    auto series = new QBarSeries();
    auto set = new QBarSet(_name);
    
    const int chartWidth = chart->plotArea().width();
    float lineRadius = 0.f;
    QString horAxisDateFormat;
    switch (_chartRangeSelected)
    {
        case Stats::chartRange::CHART_RANGE_WEEK:
            lineRadius = 10.f;
            horAxisDateFormat = "d";
            break;
        case Stats::chartRange::CHART_RANGE_MONTH:
            lineRadius = 2.f;
            horAxisDateFormat = "d";
            break;
        case Stats::chartRange::CHART_RANGE_YEAR:
            lineRadius = 6.f;
            horAxisDateFormat = chartWidth >= 600 ? "MMM" : "M";
            break;
    }
    const float lineThickness = lineRadius / (chart->plotArea().height() / 2.f);
    
    if (_rangeData[_chartRangeSelected].isEmpty()) return;
    const QList<int> frequency = _rangeData[_chartRangeSelected][_rangePointIndex].values();
    int max = 0;
    for (const int n : frequency)
    {
        *set << float(n) - lineThickness;
        if (n > max) max = n;
    }
    //Round up to even number (or a factor of 5)
    if (max % 2 != 0 && max % 5 != 0) max += 1;
    
    const QList<QDate> dates = _rangeData[_chartRangeSelected][_rangePointIndex].keys();
    QList<QString> categories;
    for (const QDate& date : dates) categories << date.toString(horAxisDateFormat);
    
    series->append(set);
    chart->addSeries(series);
    
    QLinearGradient gradient(0, 0, 0, 300);
    gradient.setColorAt(0.f, STYLE_SECONDARY_LIGHT);
    gradient.setColorAt(0.5f, STYLE_SECONDARY);
    gradient.setColorAt(1.f, STYLE_SECONDARY_DARK);
    QBrush barSetBrush(gradient);
    set->setBrush(barSetBrush);
    
    QPen pen(barSetBrush, lineRadius);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    set->setPen(pen);
    
    QPen horAxisGridLinePen(STYLE_PRIMARY_LIGHT);
    horAxisGridLinePen.setWidthF(1.2f);
    horAxisGridLinePen.setStyle(Qt::DashLine);
    
    QAbstractAxis* horAxis = nullptr;
    //Category axis is too dense for months (or years with small screens), use unattached date time axis
    if (_chartRangeSelected == Stats::chartRange::CHART_RANGE_MONTH || (_chartRangeSelected == Stats::chartRange::CHART_RANGE_YEAR && chartWidth < 400))
    {
        auto dateTimeAxis = new QDateTimeAxis();
        dateTimeAxis->setMin(dates.first().startOfDay());
        dateTimeAxis->setMax(dates.last().startOfDay());
        dateTimeAxis->setFormat(horAxisDateFormat);
        dateTimeAxis->setTickCount(6);
        horAxis = dateTimeAxis;
    }
    else
    {
        auto categoryAxis = new QBarCategoryAxis();
        categoryAxis->append(categories);
        series->attachAxis(categoryAxis);
        horAxis = categoryAxis;
    }
    horAxis->setLabelsColor(STYLE_TEXT);
    horAxis->setGridLinePen(horAxisGridLinePen);
    QFont horAxisFont = horAxis->labelsFont();
    horAxisFont.setPixelSize(13);
    horAxis->setLabelsFont(horAxisFont);
    chart->addAxis(horAxis, Qt::AlignBottom);
    
    QPen vertAxisGridLinePen(STYLE_PRIMARY_LIGHT);
    vertAxisGridLinePen.setWidthF(1.f);
    vertAxisGridLinePen.setStyle(Qt::DotLine);
        
    //TODO: Add better system to decide interval
    const int interval = qCeil(float(max) / 6.f);
    auto vertAxis = new QValueAxis();
    vertAxis->setLabelsColor(STYLE_TEXT);
    vertAxis->setMin(0);
    vertAxis->setMax(max);
    vertAxis->setLabelFormat("%i");
    vertAxis->setGridLinePen(vertAxisGridLinePen);
    vertAxis->setTickInterval(interval);
    vertAxis->setTickAnchor(0);
    vertAxis->setTickType(QValueAxis::TicksDynamic);
    QFont vertAxisLabelFont = vertAxis->labelsFont();
    vertAxisLabelFont.setPointSize(13);
    vertAxis->setLabelsFont(vertAxisLabelFont);
    chart->addAxis(vertAxis, Qt::AlignLeft);
    series->attachAxis(vertAxis);
    
    chart->legend()->hide();
}

void Stats::updateChartRanges()
{
    _settings.beginGroup(_name);
    const int size = _settings.beginReadArray(DATA_TIME_DATA);
    _settings.setArrayIndex(0);
    QDateTime start = QDateTime::fromString(_settings.value(DATA_TIMESTAMP).toString(), Qt::ISODate);
    _settings.setArrayIndex(size - 1);
    QDateTime end = QDateTime::fromString(_settings.value(DATA_TIMESTAMP).toString(), Qt::ISODate);
    _settings.endArray();
    _settings.endGroup();
    
    const quint64 diffrence = start.daysTo(end);
    
    _chartRangeAllowed[Stats::chartRange::CHART_RANGE_WEEK] = diffrence > 0;
    _chartRangeAllowed[Stats::chartRange::CHART_RANGE_MONTH] = diffrence > 7;
    _chartRangeAllowed[Stats::chartRange::CHART_RANGE_YEAR] = diffrence > 30;
    
    //Read all data
    QList<QDateTime> dates;
    _settings.beginGroup(_name);
    _settings.beginReadArray(DATA_TIME_DATA);
    for (int i = size - 1; i >= 0; i--)
    {
        _settings.setArrayIndex(i);
        dates << QDateTime::fromString(_settings.value(DATA_TIMESTAMP).toString(), Qt::ISODate);
    }
    _settings.endArray();
    _settings.endGroup();
    
    _rangeData[Stats::chartRange::CHART_RANGE_WEEK].clear();
    _rangeData[Stats::chartRange::CHART_RANGE_MONTH].clear();
    _rangeData[Stats::chartRange::CHART_RANGE_YEAR].clear();
    
    QMap<QDate, int> dailyData;
    QDate startDate = dates.size() > 0 ? dates[0].date() : QDate();
    int countInDay = 0;
    for (const QDateTime& timestamp : qAsConst(dates))
    {
        //If same day
        if (timestamp.date() == startDate)
        {
            countInDay++;
        }
        else
        {
            //Add skipped days
            for (qint64 i = timestamp.date().daysTo(startDate) - 1; i > 0; i--)
            {
                dailyData[timestamp.date().addDays(i)] = 0;
            }
            
            if (startDate.isValid()) dailyData[startDate] = countInDay;
            countInDay = 1;
            startDate = timestamp.date();
        }
    }
    
    //Weeks
    if (!_chartRangeAllowed[Stats::chartRange::CHART_RANGE_WEEK]) return;
    startDate = dailyData.lastKey();
    for (int week = 0; week < qCeil(float(dailyData.size()) / 7.f); week++)
    {
        QMap<QDate, int> currentWeek;
        for (int day = week * 7; day < (week + 1) * 7; day++)
        {
            const QDate date = startDate.addDays(-day);
            currentWeek[date] = dailyData.value(date);
        }
        _rangeData[Stats::chartRange::CHART_RANGE_WEEK] << currentWeek;
        currentWeek.clear();
    }
    
    //Months
    if (!_chartRangeAllowed[Stats::chartRange::CHART_RANGE_MONTH]) return;
	int difInMonths = (dailyData.lastKey().year() - dailyData.firstKey().year()) * 12 + dailyData.lastKey().month() - dailyData.firstKey().month();
	if (Q_UNLIKELY(difInMonths == 0)) difInMonths = 1;
	const qint64 difInDays = dailyData.lastKey().addMonths(-difInMonths).daysTo(dailyData.lastKey());
    startDate = dailyData.lastKey();
    QMap<QDate, int> totalCountInMonth;
    QMap<QDate, int> currentMonth;
    int countInMonth = 0;
    for (int day = 0; day <= difInDays; day++)
    {
        const QDate currentDate = dailyData.lastKey().addDays(-day);
        if (startDate.month() != currentDate.month() && startDate.day() == currentDate.day())
        {
            totalCountInMonth[QDate(startDate.year(), startDate.month(), 1)] = countInMonth;
            startDate = currentDate;
            _rangeData[Stats::chartRange::CHART_RANGE_MONTH] << currentMonth;
            currentMonth.clear();
            countInMonth = 0;
        }
        currentMonth[currentDate] = dailyData[currentDate];
        countInMonth++;
    }
        
    //Years
    if (!_chartRangeAllowed[Stats::chartRange::CHART_RANGE_YEAR]) return;
    startDate = dailyData.lastKey();
    QMap<QDate, int> currentYear;
    for (int month = 0; month <= qCeil(float(difInMonths) / 12.f) * 12; month++)
    {
        const QDate currentDate = QDate(dailyData.lastKey().year(), dailyData.lastKey().month(), 1).addMonths(-month);
        const QDate currentDateMonthOnly = QDate(currentDate.year(), currentDate.month(), 1);
        if (startDate.year() != currentDate.year() && startDate.month() == currentDate.month())
        {
            _rangeData[Stats::chartRange::CHART_RANGE_YEAR] << currentYear;
            currentYear.clear();
            startDate = currentDate;
        }
        currentYear[currentDateMonthOnly] = totalCountInMonth[currentDateMonthOnly];
    }
 }

QVariant Stats::getRangePoints()
{
    QVariantList rangePoints;
    for (const QMap<QDate, int>& periodData : qAsConst(_rangeData[_chartRangeSelected]))
    {
        QVariantMap point;
        point["start"] = periodData.firstKey().toString("yyyy-MM-dd");
        point["end"] = periodData.lastKey().toString("yyyy-MM-dd");
        rangePoints << point;
    }
    return rangePoints;
}
