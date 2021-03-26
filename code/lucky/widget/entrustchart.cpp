#include "entrustchart.h"
#include <QDebug>
#include <QtCharts>
#include <QLineSeries>
using namespace QtCharts;
//每次刷新数据相隔毫秒数
const int C_FlushDataSec = 2000;

EntrustChart::EntrustChart() : QChart() {

    m_AxisX_Time = new QDateTimeAxis();
    m_AxisY_Left = new QValueAxis();

    m_LeftSeries = new QLineSeries();
    //设置曲线
    m_LeftSeries->setPen(QPen(Qt::GlobalColor::red, 0.6, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    this->addSeries(m_LeftSeries);//添加曲线到chart中
    //设置x轴
    QDateTime temp_StartTime(m_BaseDate, QTime(9, 30, 0));
    m_AxisX_Time->setTickCount(9); // 网格数
    m_AxisX_Time->setRange(temp_StartTime, temp_StartTime.addSecs(60 * 60 * 8));//时间显示范围
    QColor color = QColor(Qt::GlobalColor::black);
    m_AxisX_Time->setTitleFont(QFont("黑体", 8, QFont::Medium, false));
    m_AxisX_Time->setTitleBrush(color);
    m_AxisX_Time->setLinePenColor(color);
    m_AxisX_Time->setFormat("hh:mm");//坐标轴显示方式: 时:分
    //设置y轴
    m_AxisY_Left->setRange(0, 30);
    m_AxisY_Left->setTitleFont(QFont("黑体", 8, QFont::Medium, false));
    m_AxisY_Left->setTitleBrush(color);
    m_AxisY_Left->setLinePenColor(color);
    //把坐标轴添加到chart中
    this->addAxis(m_AxisX_Time, Qt::AlignBottom);
    this->addAxis(m_AxisY_Left, Qt::AlignLeft);
    //把曲线关联到坐标轴
    m_LeftSeries->attachAxis(m_AxisX_Time);
    m_LeftSeries->attachAxis(m_AxisY_Left);
    //将图例关闭
    this->legend()->hide();
    //开启时间事件 (每C_FlushDataSec毫秒)
    timerId = startTimer(C_FlushDataSec);
}

EntrustChart::~EntrustChart() {
}

void EntrustChart::timerEvent(QTimerEvent *event) {
    // 产生一个数据，模拟不停的接收到新数据
    if (event->timerId() == timerId) {
        //添加数据点
        m_LeftSeries->append(QDateTime::currentDateTime().addSecs(60 * 60).toMSecsSinceEpoch(), 10);
    }
}

void EntrustChart::dataReceived(int value) {
    data << value;

    // 数据个数超过了最大数量，则删除最先接收到的数据，实现曲线向前移动
    while (data.size() > maxSize) {
        data.removeFirst();
    }

    // 界面被隐藏后就没有必要绘制数据的曲线了
    if (isVisible()) {
//        splineSeries->clear();
//        scatterSeries->clear();
//        int dx = maxX / (maxSize-1);
//        int less = maxSize - data.size();

//        for (int i = 0; i < data.size(); ++i) {
//            splineSeries->append(less*dx+i*dx, data.at(i));
//            scatterSeries->append(less*dx+i*dx, data.at(i));
//        }
    }
}
