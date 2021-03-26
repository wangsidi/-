#include <QMouseEvent>

#include "detaillabel.h"

DetailLabel::DetailLabel(QString title, QWidget* parent):
    QLabel(title, parent)
{  
    this->show();
}

void DetailLabel::mousePressEvent(QMouseEvent *ev){
    //如果为左键
    if(ev->button() == Qt::MouseButton::LeftButton)
    {
        emit Clicked(this);
        return;
    }
    return QLabel::mousePressEvent(ev);
}
