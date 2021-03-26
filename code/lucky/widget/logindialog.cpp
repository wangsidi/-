#include "logindialog.h"
#include "ui_logindialog.h"
#include "public/Basic.h"


LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    //关闭析构
    this->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
    //按钮
    QObject::connect(ui->login, &QPushButton::clicked, this, [=](){
        //按钮
        this->isClicked = true;
        //发送
        emit clickedDialog();
    });
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

const TradeAccount *LoginDialog::loadInformation()
{
    TradeAccount* user = new TradeAccount;

    strcpy(user->FundAccount, ui->account->text().toUtf8());
    strcpy(user->Password, ui->password->text().toUtf8());
    user->EntrustWay = '7';
    strcpy(user->AppId, ui->appId->text().toUtf8());
    strcpy(user->AuthCode, ui->authCode->text().toUtf8());

    return user;
}

void LoginDialog::closeEvent(QCloseEvent *)
{
    emit closedDialog(this);
}
