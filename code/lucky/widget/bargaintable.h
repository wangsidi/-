#ifndef BARGAINTABLE_H
#define BARGAINTABLE_H
#include <QTableView>
//成交列表
class BargainTable : public QTableView
{
    Q_OBJECT
public:
    BargainTable(QWidget* parent = nullptr);
};

#endif // BARGAINTABLE_H
