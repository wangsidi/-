#ifndef BUYMODEL_H
#define BUYMODEL_H
#include <QStandardItemModel>

class BuyModel: public QStandardItemModel
{
    Q_OBJECT
public:
    BuyModel();
    //设置属性
    QVariant data(const QModelIndex& index, int role)const override;
};

#endif // BUYMODEL_H
