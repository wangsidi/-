#include "clickedlabel.h"
#include <QMouseEvent>

ClickedLabel::ClickedLabel(QString title, QWidget* parent):
    QLabel(title, parent)
{  
    this->setStyleSheet(QString("color:rgb(210, 210, 120);	font: 11pt  \"新宋体\";"));
    this->setCursor(Qt::CursorShape::PointingHandCursor);
    this->setVisible(false);
}

void ClickedLabel::mousePressEvent(QMouseEvent *ev){
    //如果为左键
    if(ev->button() == Qt::MouseButton::LeftButton)
    {
        emit Clicked(this);
        return;
    }
    return QLabel::mousePressEvent(ev);
}
