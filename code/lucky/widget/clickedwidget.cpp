#include "clickedwidget.h"

ClickedWidget::ClickedWidget(QWidget* parent):
       QWidget(parent)
{

}

void ClickedWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
    emit doubleClicked(this);
}
