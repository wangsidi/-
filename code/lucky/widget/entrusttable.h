#ifndef ENTRUSTTABLE_H
#define ENTRUSTTABLE_H
#include <QTableView>
//委托列表
class EntrustTable : public QTableView
{
    Q_OBJECT
public:
    EntrustTable(QWidget* parent = nullptr);
};

#endif // ENTRUSTTABLE_H
