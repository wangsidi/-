#include "buytable.h"
#include "widget/mytablemodel.h"
#include "widget/buy/buymodel.h"

#include <QHeaderView>
//买表
BuyTable::BuyTable(QWidget* parent) :
    QTableView(parent)
{
    //只读
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    //隐藏表头
    this->horizontalHeader()->setVisible(false);
    this->verticalHeader()->setVisible(false);
    //隐藏网格
    this->setShowGrid(false);
    //自适应列宽
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //自适应列高
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //创建初始化模型
    BuyModel* model = new BuyModel;
    //列
    for(int column = 0; column != 3; ++column)
    {
        //行
        switch (column) {
        case 0:
            //编号列
            for(int rows = 0; rows != 5; ++rows) model->setItem(rows, column, new QStandardItem(QString::number(rows + 1)));
            break;
        case 1:
            //报价列
            for(int rows = 0; rows != 5; ++rows) model->setItem(rows, column, new QStandardItem(QString("0")));
            break;
        case 2:
            //量列
            for(int rows = 0; rows != 5; ++rows) model->setItem(rows, column, new QStandardItem(QString("0")));
            break;
        }
    }
    //设置模型
    this->setModel(model);
}
