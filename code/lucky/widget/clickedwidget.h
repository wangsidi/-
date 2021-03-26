#ifndef CLICKEDWIDGET_H
#define CLICKEDWIDGET_H
#include <QWidget>
//双击事件
class ClickedWidget : public QWidget
{
    Q_OBJECT
public:
    ClickedWidget(QWidget* parent = nullptr);
Q_SIGNALS:
    void doubleClicked(ClickedWidget*);
protected:
    void mouseDoubleClickEvent(QMouseEvent* ev) override;
};

#endif // CLICKEDWIDGET_H
