#include <QStandardItemModel>
#include <QHeaderView>
#include <QScrollBar>

#include "quotatable.h"
//行情表格
QuotaTable::QuotaTable(QWidget *parent) :
    QTableView(parent)
{
    //只读
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    //自适应列宽
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    //自适应列高
//    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //隐藏垂直表头
    this->verticalHeader()->setVisible(false);
    //创建初始化模型
    QStandardItemModel* model = new QStandardItemModel;
    //创建表头
    QStringList header;
    //赋值
    header <<"合约代码          " << "合约名称          " << " 开盘                            " << " 最高                            " << " 最低                            " << " 最新                            " << " 涨停                            " << " 跌停                            "/*<<"测试次数"*/;
    //加入表头
    model->setHorizontalHeaderLabels(header);
    //设置模型
    this->setModel(model);
    //设置滚动条
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                             "{"
                                             "width:8px;"
                                             "background:rgba(0,0,0,0%);"
                                             "margin:0px,0px,0px,0px;"
                                             "padding-top:9px;"
                                             "padding-bottom:9px;"
                                             "}"
                                             "QScrollBar::handle:vertical"
                                             "{"
                                             "width:8px;"
                                             "background:rgba(0,0,0,25%);"
                                             " border-radius:4px;"
                                             "min-height:20;"
                                             "}"
                                             "QScrollBar::handle:vertical:hover"
                                             "{"
                                             "width:8px;"
                                             "background:rgba(0,0,0,50%);"
                                             " border-radius:4px;"
                                             "min-height:20;"
                                             "}"
                                             "QScrollBar::add-line:vertical"
                                             "{"
                                             "height:9px;width:8px;"
                                             "border-image:url(:/images/a/3.png);"
                                             "subcontrol-position:bottom;"
                                             "}"
                                             "QScrollBar::sub-line:vertical"
                                             "{"
                                             "height:9px;width:8px;"
                                             "border-image:url(:/images/a/1.png);"
                                             "subcontrol-position:top;"
                                             "}"
                                             "QScrollBar::add-line:vertical:hover"
                                             "{"
                                             "height:9px;width:8px;"
                                             "border-image:url(:/images/a/4.png);"
                                             "subcontrol-position:bottom;"
                                             "}"
                                             "QScrollBar::sub-line:vertical:hover"
                                             "{"
                                             "height:9px;width:8px;"
                                             "border-image:url(:/images/a/2.png);"
                                             "subcontrol-position:top;"
                                             "}"
                                             "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                             "{"
                                             "background:rgba(0,0,0,10%);"
                                             "border-radius:4px;"
                                             "}"
                                             );
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}
