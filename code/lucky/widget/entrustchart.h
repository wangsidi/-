#ifndef ENTRUSTCHART_H
#define ENTRUSTCHART_H
#include <QMainWindow>
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
//委托图表
class EntrustChart : public QChart
{
    Q_OBJECT

public:
    explicit EntrustChart();
    ~EntrustChart();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    /**
     * 接收到数据源发送来的数据，数据源可以下位机，采集卡，传感器等。
     */
    void dataReceived(int value);

    int timerId;
    int maxSize;  // data 最多存储 maxSize 个元素
    int maxX;
    int maxY;
    QList<double> data; // 存储业务数据的 list


    QLineSeries* m_LeftSeries = NULL;
    QDateTimeAxis* m_AxisX_Time = NULL;
    QValueAxis* m_AxisY_Left;
    QTimer* m_DrawTimer = NULL;
    const QDate m_BaseDate = QDate::currentDate();
    const QTime m_BaseTime = QTime(0, 0, 0);
    QStringList m_DataList;
    int second_count = 0;
};

#endif // ENTRUSTCHART_H
