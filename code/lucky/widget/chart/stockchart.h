#ifndef STOCKCHART_H
#define STOCKCHART_H
#include <QMainWindow>
#include <QtCharts>
class UnifiedQuotaData;
QT_CHARTS_USE_NAMESPACE
//委托图表
class StockChart : public QChart
{
    Q_OBJECT

public:
    explicit StockChart(const UnifiedQuotaData* data);
    virtual ~StockChart();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    int valueTimerId;
    QLineSeries* m_LeftSeries = NULL;
private:
    const UnifiedQuotaData* cData;

    double minY;
    double maxY;
};
#endif // STOCKCHART_H
