#include "entrusttable.h"
#include "widget/mytablemodel.h"

#include <QHeaderView>

EntrustTable::EntrustTable(QWidget* parent) :
    QTableView(parent)
{
    //只读
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    //去除垂直表头
    this->verticalHeader()->setVisible(false);
    //自适应列宽
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //创建初始化模型
    MyTableModel* model = new MyTableModel;
    //创建表头
    QStringList header;
    //赋值
    header << "委托编号" << "交易类别"<< "合约代码"<< "买卖方向"<< "开平仓方向"<< "投机/套保类型"<< "委托数量"<< "总成交数量"<< "委托价格"<< "委托状态"<< "委托时间"<< "详情" << "撤单" << "错误信息";
    //加入表头
    model->setHorizontalHeaderLabels(header);
    //设置模型
    this->setModel(model);
    //设置
    this->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
}
