#include "sellmodel.h"

SellModel::SellModel():
    QStandardItemModel()
{

}

QVariant SellModel::data(const QModelIndex &index, int role) const
{
    {
        //获取处理role
        QVariant value = QStandardItemModel::data(index, role);
        //判断role
        switch (role) {

        case Qt::TextAlignmentRole:
        {
            //设置居中
            value = Qt::AlignVCenter;
            //返回
            return value;
        }
        case Qt::ForegroundRole:
            switch(index.column() % 3)
            {
            case 0:
                return QVariant(QColor(245,245,245));
            case 1:
                return QVariant(QColor(40,200,240));
            case 2:
                return QVariant(QColor(40,200,240));
            default://only to disable warning
                return QVariant(QColor(Qt::white));
            }
        }
        //如果是文字布局处理项
        //其他直接返回
        return value;
    }
}
