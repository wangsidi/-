#include <QStandardItemModel>
#include <QHeaderView>

#include "quotaheadertable.h"
//行情表头表格
QuotaHeaderTable::QuotaHeaderTable(QWidget *parent) :
    QTableView(parent)
{
    //只读
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);

    //自适应列宽
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //隐藏垂直表头
    this->verticalHeader()->setVisible(false);
    //创建初始化模型
    QStandardItemModel* model = new QStandardItemModel;
    //创建表头
    QStringList header;
    //赋值
    header << "合约代码" << "合约名称";
    //加入表头
    model->setHorizontalHeaderLabels(header);
    //设置模型
    this->setModel(model);
}
