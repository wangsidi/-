#include "public/QuotaStruct.h"
#include "stockchart.h"
#include <QDebug>
#include <QtCharts>
#include <QLineSeries>
#include <random>
//每次刷新数据相隔毫秒数
const int C_FlushDatamSec = 15000;
//测试代码
const std::string searchCOde = "";
//随机数
std::uniform_int_distribution<int> un(0, 30);
std::default_random_engine e(static_cast<unsigned int>(time(nullptr)));
//创建图表
StockChart::StockChart(const UnifiedQuotaData* data) : QChart() , cData(data){
    //设置图表元素
    m_LeftSeries = new QLineSeries();
    QDateTimeAxis* m_AxisX_Time = new QDateTimeAxis();
    QValueAxis* m_AxisY_Left = new QValueAxis();
    //设置背景
    this->setBackgroundBrush(QBrush(QColor(38,38,40)));
    //设置曲线
    m_LeftSeries->setPen(QPen(Qt::GlobalColor::red, 0.6, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    this->addSeries(m_LeftSeries);//添加曲线到chart中
    //设置x轴
    QDateTime temp_StartTime(QDateTime::currentDateTime());
    QDateTime temp_EndTime;
    if(QTime::currentTime().secsTo(QTime(11,30,0)) > 0)
        temp_EndTime = QDateTime(QDateTime::currentDateTime().date(), QTime(11, 30, 0));
    else
        temp_EndTime = QDateTime(QDateTime::currentDateTime().date(), QTime(15, 0, 0));
    m_AxisX_Time->setTickCount(9); // 网格数
    m_AxisX_Time->setRange(temp_StartTime, temp_EndTime);//时间显示范围
    QColor color = QColor(Qt::GlobalColor::black);
    m_AxisX_Time->setTitleFont(QFont("黑体", 8, QFont::Medium, false));
    m_AxisX_Time->setTitleBrush(color);
    m_AxisX_Time->setLinePenColor(color);
    m_AxisX_Time->setFormat("hh:mm");//坐标轴显示方式: 时:分
    m_AxisX_Time->setLabelsColor(QColor(245,245,245));
    //设置y轴
    //设置范围
    minY = (static_cast<double>(cData->m_nLastPrice)/(double)100)*0.99;
    maxY = (static_cast<double>(cData->m_nLastPrice)/(double)100)*1.01;
    m_AxisY_Left->setRange(/*cData->m_nDownLimitPrice, cData->m_nUpLimitPrice*/minY,maxY);
    m_AxisY_Left->setTitleFont(QFont("黑体", 8, QFont::Medium, false));
    m_AxisY_Left->setTitleBrush(color);
    m_AxisY_Left->setLinePenColor(color);
    m_AxisY_Left->setLabelsColor(QColor(245,245,245));
    //把坐标轴添加到chart中
    this->addAxis(m_AxisX_Time, Qt::AlignBottom);
    this->addAxis(m_AxisY_Left, Qt::AlignLeft);
    //把曲线关联到坐标轴
    m_LeftSeries->attachAxis(m_AxisX_Time);
    m_LeftSeries->attachAxis(m_AxisY_Left);
    //将图例关闭
    this->legend()->hide();
    //初始点
    const double priceData = static_cast<double>(cData->m_nLastPrice)/(double)100;
    m_LeftSeries->append(QDateTime::currentDateTime().toMSecsSinceEpoch(), priceData);
    //启动程序定时器
    valueTimerId = startTimer(C_FlushDatamSec);
}
//析构图表
StockChart::~StockChart() {
}
//计时器
void StockChart::timerEvent(QTimerEvent *event) {
    // 产生一个数据，模拟不停的接收到新数据
    if (event->timerId() == valueTimerId) {
        const double data = static_cast<double>(cData->m_nLastPrice)/(double)100;
        if(data <= minY) { this->axisY()->setRange(data * 0.99, maxY); minY = data * 0.99;}
        if(data >= maxY) { this->axisY()->setRange(minY, data * 1.01); maxY = data * 1.01;}
        m_LeftSeries->append(QDateTime::currentDateTime().toMSecsSinceEpoch(), data);
        return;
    }
    else{
        return QChart::timerEvent(event);
    }
}
