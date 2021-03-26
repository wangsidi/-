#include <QChartView>

#include "widget/entrustchart.h"
#include "stockdialog.h"
#include "ui_stockdialog.h"
//命名空间
using namespace QtCharts;
//股票会话
StockDialog::StockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StockDialog)
{
    ui->setupUi(this);
    EntrustChart* chart = new EntrustChart;
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(chartView, 0, 0);
    ui->widget->setLayout(baseLayout);
}

StockDialog::~StockDialog()
{
    delete ui;
}
