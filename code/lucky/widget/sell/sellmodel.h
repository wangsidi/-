#ifndef SELLMODEL_H
#define SELLMODEL_H
#include <QStandardItemModel>

class SellModel: public QStandardItemModel
{
    Q_OBJECT
public:
    SellModel();
    //设置属性
    QVariant data(const QModelIndex& index, int role)const override;
};

#endif // SELLMODEL_H
