#include <QChartView>
#include <QTime>
#include "stockwindow.h"
#include "ui_stockwindow.h"
#include "public/QuotaStruct.h"
#include "widget/chart/stockchart.h"

//命名空间
using namespace QtCharts;
//股票
StockWindow::StockWindow(const UnifiedQuotaData* data, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StockWindow),
    chart(nullptr)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);

    this->setWindowTitle(QString::fromUtf8(data->m_sContractCode));

    this->setStyleSheet(QString("background-color: rgb(38,38,40);"));

    //周一 - 周五 9:30 - 11:30 13:00 - 15:00
    if(((QDateTime::currentDateTime().toString("ddd") != "周六" && QDateTime::currentDateTime().toString("ddd") != "周日") && ((QTime::currentTime().secsTo(QTime(9,30,0))<=0 &&  QTime::currentTime().secsTo(QTime(11,0,0))>0)|| (QTime::currentTime().secsTo(QTime(13,0,0))<=0 && QTime::currentTime().secsTo(QTime(15,0,0))>0)))){
        chart = new StockChart(data);

        ui->widget->setChart(chart);
        ui->widget->setRenderHint(QPainter::Antialiasing);
        this->showMaximized();
    }
    else{
        QMessageBox::warning(nullptr, "警告", "不在时间段之内", QMessageBox::Ok);
        this->close();
    }
}

StockWindow::~StockWindow()
{
    if(chart) delete chart;
    delete ui;
}
