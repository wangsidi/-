#include "positiontable.h"
#include "widget/mytablemodel.h"

#include <QHeaderView>

PositionTable::PositionTable(QWidget *parent) :
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
    header << "交易类别" << "合约代码" << "买卖方向" << "期初数量" << "可用数量" << "当日开仓可用数量" << "投机/套保类型" << "成交数量" <<  "今总持仓" << "老仓持仓数量";
    //加入表头
    model->setHorizontalHeaderLabels(header);
    //设置模型
    this->setModel(model);
}

