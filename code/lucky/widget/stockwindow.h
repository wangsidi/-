#ifndef STOCKWINDOW_H
#define STOCKWINDOW_H

#include <QMainWindow>

namespace Ui {
class StockWindow;
}
class StockChart;
class UnifiedQuotaData;
//股票视图
class StockWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StockWindow(const UnifiedQuotaData* data, QWidget *parent = nullptr);
    ~StockWindow();

private:
    Ui::StockWindow *ui;

    StockChart* chart;
};

#endif // STOCKWINDOW_H
