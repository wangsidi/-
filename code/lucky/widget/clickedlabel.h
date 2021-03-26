#ifndef DETAILLABEL_H
#define DETAILLABEL_H
#include <QLabel>
#include <QModelIndex>

class ClickedLabel : public QLabel
{
    Q_OBJECT
Q_SIGNALS:
    void Clicked(ClickedLabel*);
public:
    ClickedLabel(QString title = "<u>添加</u>" , QWidget* parent = nullptr);
public:
    void mousePressEvent(QMouseEvent *ev) override;
};

#endif // DETAILLABEL_H
