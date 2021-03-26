#ifndef POSITIONTABLE_H
#define POSITIONTABLE_H
#include <QTableView>
//持仓列表
class PositionTable: public QTableView
{
    Q_OBJECT
public:
    PositionTable(QWidget* parent = nullptr);
};

#endif // POSITIONTABLE_H
