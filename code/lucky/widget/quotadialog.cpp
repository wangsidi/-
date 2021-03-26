#include "quotadialog.h"
#include "ui_quotadialog.h"

#include <QKeyEvent>

QuotaDialog::QuotaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuotaDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->confirm, &QPushButton::clicked, this, [=](){
        emit confirmValue(ui->nIssueTypeText->text(), ui->exchangeTypeText->text(), ui->contractCodeText->text());
    });

    QObject::connect(ui->all, &QPushButton::clicked, this, [=](){
        emit allValue(ui->nIssueTypeText->text());
    });
}

QuotaDialog::~QuotaDialog()
{
    delete ui;
}

void QuotaDialog::showError(QString errorInfo){
    ui->error->setText(errorInfo);
}

void QuotaDialog::keyPressEvent(QKeyEvent *ev)
{
    //全局窗口回车 => 添加行情
    if(ev->key()==Qt::Key::Key_Enter || ev->key()==Qt::Key::Key_Return){
        emit confirmValue(ui->nIssueTypeText->text(), ui->exchangeTypeText->text(), ui->contractCodeText->text());
        return;
    }

    return QDialog::keyPressEvent(ev);
}
