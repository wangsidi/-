/*******************************************************************************************************/
/*
/*    1、此C++ Demo使用异步发送接收模式；
/*    2、Demo连接的是恒生仿真测试环境，如要对接券商环境，需要修改t2sdk.ini文件中的IP和许可证文件等；
/*    3、不同环境的验证信息可能不一样，更换连接环境时，包头字段设置需要确认；
/*    4、接口字段说明请参考接口文档"恒生统一接入平台_周边接口规范(期货，期权).xls"；
/*    5、T2函数技术说明参考开发文档“T2SDK 外部版开发指南.docx"；
/*    6、如有UFX接口技术疑问可联系大金融讨论组（261969915）；
/*    7、UFX技术支持网站 https://ufx.hscloud.cn/；
/*    8、demo仅供参考。
/*
/*******************************************************************************************************/
#include <QDebug>
#include <QPushButton>
#include <QString>

#include "package/FutuTrade.h"
#include "package/FutuMd.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->confirm, &QPushButton::clicked, this, &MainWindow::testFunc);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//交易前置地址
char TradeFrontAddr[255] = "121.41.126.194:9399";
//交易接入许可证书（相对路径）
char TradelicensePath[255] = "license.dat";
//行情前置地址
char MdFrontAddr[255] = "121.41.126.194:9399";
//行情接入许可证书（相对路径）
char MdlicensePath[255] = "license.dat";
//App信息
char UserApplicationInfo[255] = "Demo";
//看穿式AppID
char AppId[255] = "Hs_Strade_5.0";
//看穿式认证串
char AuthCode[64] = "Hs_Strade_5.0";

//资金账号（请联系环境负责人获取资金账号）
char FundAccount[32] = "";
//交易密码（请联系环境负责人获取对应密码）
char Password[32] = "";
//委托方式
char EntrustWay = '7';

void ShowDir()
{
    qDebug()<<"-------------------------------------------------";
    qDebug()<<" [ 1]  331480 请求穿透式认证                     ";
    qDebug()<<" [ 2]  331481 请求客户端认证                     ";
    qDebug()<<" [ 3]  331482 请求客户端系统信息上报             ";
    qDebug()<<" [ 4]  331100 请求登录                           ";
    qDebug()<<" [ 5]  338202 期货委托确认                       ";
    qDebug()<<" [ 6]  338217 期货撤单                           ";
    qDebug()<<" [ 7]  338301 期货委托查询                       ";
    qDebug()<<" [ 8]  338302 期货成交查询                       ";
    qDebug()<<" [ 9]  338303 期货持仓查询                       ";
    qDebug()<<" [10]  620001 期货委托成交回报订阅               ";
    qDebug()<<" [20]  331100 行情登录                           ";
    qDebug()<<" [21]  620001 单腿行情订阅                       ";
    qDebug()<<" [99]  ShowDir                                   ";
    qDebug()<<" [ 0]  EXIT                                      ";
    qDebug()<<"-------------------------------------------------";
}

void MainWindow::testFunc()
{
    //交易实例
    CTrade_Impl* lpTrade = NULL;
    //行情实例
    CMd_Impl* lpMd = NULL;

    char exchange_type[5];
    char futu_code[8];
    char position_str[100];
    char request_num[10];
    int entrust_amount = 0;
    int entrust_no = 0;
    double entrust_price = 0.0;
    char entrust_bs = '\0';
    char futures_direction = '\0';
    int issue_type = 0;
    memset(exchange_type,0,sizeof(exchange_type));
    memset(futu_code,0,sizeof(futu_code));
    memset(position_str,0,sizeof(position_str));
    memset(request_num,0,sizeof(request_num));

    lpTrade = new CTrade_Impl();
    lpTrade->RegisterFront(TradeFrontAddr);
    lpTrade->InitConn(TradelicensePath);

    lpMd = new CMd_Impl();
    lpMd->RegisterFront(MdFrontAddr);
    lpMd->InitConn(MdlicensePath);

    ShowDir();

    while(1)
    {
        switch(ui->choose->text().toUInt()){
            case 1:
                lpTrade->ReqFunction331480(FundAccount, Password);
                break;
            case 2:
                lpTrade->ReqFunction331481(FundAccount, AppId, AuthCode);
                break;
            case 3:
                lpTrade->ReqFunction331482(FundAccount, AppId);
                break;
            case 4:
                lpTrade->ReqFunction331100(FundAccount, Password, EntrustWay);
                break;
            case 5:
                qDebug()<<"请输入交易类别（exchange_type）:";cin>>exchange_type;
                qDebug()<<"请输入合约代码（futu_code）:"; cin>>futu_code;
                qDebug()<<"请输入买卖方向（entrust_bs）:";cin>>entrust_bs;
                qDebug()<<"请输入开平仓方向（futures_direction）:";cin>>futures_direction;
                qDebug()<<"请输入委托数量（entrust_amount）:";cin>>entrust_amount;
                qDebug()<<"请输入委托价格（entrust_price）:";cin>>entrust_price;
                lpTrade->ReqFunction338202(exchange_type,futu_code,entrust_bs,futures_direction,entrust_amount,entrust_price);
                break;
            case 6:
                qDebug()<<"请输入撤单编号（entrust_no）：";cin>>entrust_no;
                lpTrade->ReqFunction338217(entrust_no);
                break;
            case 7:
                lpTrade->ReqFunction338301("", 50);
                break;
            case 8:
                lpTrade->ReqFunction338302();
                break;
            case 9:
                lpTrade->ReqFunction338303();
                break;
            case 10:
                lpTrade->SubFunction33101(ISSUE_TYPE_REALTIME);
                break;

            case 20:
                lpMd->ReqFunction331100(FundAccount, Password, EntrustWay);
                break;
            case 21:
                lpMd->SubFunction33100(ISSUE_TYPE_MERKET);
                break;

            case 99:
                ShowDir();
                break;
            case 0:exit(0);
        }
    }

    delete lpTrade;
    delete lpMd;
}
