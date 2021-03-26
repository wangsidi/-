#ifndef CROSSWIDGET_H
#define CROSSWIDGET_H
#include <QWidget>

class CrossWidget : public QWidget
{
public:
    CrossWidget(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
public:
    int top;
    int buttom;
};

#endif // CROSSWIDGET_H
