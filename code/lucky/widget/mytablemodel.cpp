#include "mytablemodel.h"

MyTableModel::MyTableModel():
    QStandardItemModel()
{

}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    //获取处理role
    QVariant value = QStandardItemModel::data(index, role);
    //判断role
    switch (role) {

    case Qt::TextAlignmentRole:
    {
        //设置居中
        value = Qt::AlignCenter;
        //返回
        return value;
    }
    }
    //如果是文字布局处理项
    //其他直接返回
    return value;
}
