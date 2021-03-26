#ifndef DETAILLABEL_H
#define DETAILLABEL_H
#include <QLabel>
#include <QModelIndex>

class DetailLabel : public QLabel
{
    Q_OBJECT
Q_SIGNALS:
    void Clicked(DetailLabel*);
public:
    DetailLabel(QString title = "添加" , QWidget* parent = nullptr);
public:
    void mousePressEvent(QMouseEvent *ev) override;
};

#endif // DETAILLABEL_H
