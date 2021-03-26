#ifndef QUOTAHEADERTABLE_H
#define QUOTAHEADERTABLE_H
#include <QTableView>
//行情表头表格
class QuotaHeaderTable : public QTableView
{
    Q_OBJECT
public:
    QuotaHeaderTable(QWidget* parent = nullptr);

};

#endif // QUOTAHEADERTABLE_H
