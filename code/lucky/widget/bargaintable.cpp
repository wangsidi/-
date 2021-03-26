#include "bargaintable.h"
#include "widget/mytablemodel.h"

#include <QHeaderView>
BargainTable::BargainTable(QWidget* parent) :
    QTableView(parent)
{
    //只读
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    //自适应列宽
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //创建初始化模型
    MyTableModel* model = new MyTableModel;
    //创建表头
    QStringList header;
    //赋值
    header << "成交编号" << "合约代码" << "成交价格" << "成交数量" << "成交时间" ;
    //加入表头
    model->setHorizontalHeaderLabels(header);
    //设置模型
    this->setModel(model);
}
