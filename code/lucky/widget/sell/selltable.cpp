#include "selltable.h"
#include "widget/mytablemodel.h"
#include "widget/sell/sellmodel.h"

#include <QHeaderView>
//买表
SellTable::SellTable(QWidget* parent) :
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
    SellModel* model = new SellModel;
    //列
    for(int column = 0; column != 3; ++column)
    {
        //行
        switch (column) {
        case 0:
            //编号列
            for(int rows = 0; rows != 5; ++rows) model->setItem(rows, column, new QStandardItem(QString::number(5 - rows)));
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
