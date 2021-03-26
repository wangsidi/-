#ifndef QUOTATABLE_H
#define QUOTATABLE_H
#include <QTableView>
class QStandardItemModel;
//行情表格
class QuotaTable : public QTableView
{
    Q_OBJECT
public:
    QuotaTable(QWidget* parent = nullptr);
};
#endif // QUOTATABLE_H
