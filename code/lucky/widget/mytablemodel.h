#ifndef MYTABLEMODLE_H
#define MYTABLEMODLE_H
#include <QStandardItemModel>

class MyTableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    MyTableModel();
    //设置属性
    QVariant data(const QModelIndex& index, int role)const override;
};

#endif // MYTABLEMODLE_H
