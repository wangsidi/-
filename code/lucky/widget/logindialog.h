#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}
class TradeAccount;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    //获取信息
    const TradeAccount* loadInformation();
    //是不是按钮
    bool isClicked = false;
Q_SIGNALS:
    void closedDialog(LoginDialog*);
    void clickedDialog();
protected:
    void closeEvent(QCloseEvent *) override;
private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
