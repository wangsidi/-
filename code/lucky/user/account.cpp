#include <QChartView>
#include <QDomDocument>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QTime>
#include <QTimerEvent>
#include <QPushButton>
#include <QDir>

#include "account.h"
#include "package/FutuQuota.h"
#include "package/FutuTrade.h"
#include "public/Basic.h"
#include "public/QuotaStruct.h"
#include "widget/buy/buymodel.h"
#include "widget/buy/buytable.h"
#include "widget/sell/sellmodel.h"
#include "widget/sell/selltable.h"
#include "widget/bargaintable.h"
#include "widget/clickedlabel.h"
#include "widget/logindialog.h"
#include "widget/mytablemodel.h"
#include "widget/quotadialog.h"
#include "widget/stockwindow.h"
#include "ui_account.h"
//命名空间
using namespace QtCharts;
//int 转 char
const char C_Int2Char = 49;
//每页显示委托数 1
const int C_EntrustPerPage = 14;
//每页显示持仓信息数 2
const int C_PositionPerPage = 20;
//每页显示行情信息数 3
const int C_QuotaPerPage = 35;
//目前页数 1
static int entrustCurrentPage = 0;
//目前页数 2
static int positionCurrentPage = 0;
//目前页数 3
static int quotaCurrentPage = 0;
//目前总页数 1
static int entrustTotalPage = 0;
//目前总页数 2
static int positionTotalPage = 0;
//目前总页数 3
static int quotaTotalPage = 0;
//目前详细信息的行数
static int detailInformationRows = 0;
//可用资金计时间隔
const int C_EnableBalance_TimeDis = 5000;
//行情数据刷新间隔
const int C_FlashQuotaData_TimeDis = 1000;
//自动发起行情jiange
const int C_AutoSend_TimeDis = 30000;
//配置信息 连接恒生
const QString C_TradeConnectPath = "trade.ini";
//配置信息 连接行情
const QString C_QuotaConnectPath = "quota.ini";
//构造用户类
Account::Account(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::Account),
    impl(new CTrade_Impl()),
    quotaImpl(new CQuota_Impl()),
    user(nullptr),
    pStrockMap(nullptr),
    keys(nullptr),
    stockView(nullptr)
{
    //初始化界面
    ui->setupUi(this);
    //输出信息只读
    ui->message->setReadOnly(true);
    //消息
    QObject::connect(impl, &CTrade_Impl::appendMsg, this, [=](QString msg){
        ui->message->append(QTime::currentTime().toString() + "  " + msg + "\n");
    });
    QObject::connect(quotaImpl, &CQuota_Impl::appendMsg, this, [=](QString msg){
        ui->message->append(QTime::currentTime().toString() + "  " + msg + "\n");
    });
    QObject::connect(quotaImpl, &CQuota_Impl::setSubStatus, this, [=](QString msg){
        ui->subStatus->setText(msg);
    });
    //隐藏消息
    QObject::connect(ui->hideMsg, &QPushButton::clicked, this, [=](){
        //隐藏
        if(ui->hideMsg->text() == "-") {
            ui->message->setVisible(false);
            ui->hideMsg->setText("+");
        }
        //显示
        else{
            ui->message->setVisible(true);
            ui->hideMsg->setText("-");
        }
    });
    //连接失败 => 加载连接
    QObject::connect(impl, &CTrade_Impl::connectClose, this, [=](){
        ui->message->append(QTime::currentTime().toString() + "  "  + "已断开连接, 正在自动尝试重连"+ "\n");
        //重载连接
        impl->InitConn(C_TradeConnectPath);/*-------------------------------------------------*/
        //连接失败
        connectStatus = false;

    });
    //点击重连
    QObject::connect(ui->reconnectBtn, &QPushButton::clicked, this, [=](){
        //如果连接状态断开
        if(connectStatus == false){
            ui->message->append(QTime::currentTime().toString() + "  "  + "已断开连接, 正在手动尝试重连"+ "\n");
            //重载连接
            impl->InitConn(C_TradeConnectPath);/*-------------------------------------------------*/
        }
        else
            ui->message->append(QTime::currentTime().toString() + "  "  + "处于已连接状态"+ "\n");
    });
}
Account::~Account()
{
    //析构ui
    delete ui;
    //析构实现类
    delete impl;
    //析构实现类
    delete quotaImpl;
    //析构账号
    if(user != nullptr) delete user;
    //析构委托
    for(EntrustInfo* entrust : entrustList) delete entrust;
    //析构持仓
    for(PositionInfo* position : positionList) delete  position;
}
//开始程序
void Account::start()
{
    //加载与服务器连接
    loadConnection();
    //根据xml直接登录
    loadLogin();
}
//时间事件
void Account::timerEvent(QTimerEvent *ev)
{
    //如果为可用资金
    if(ev->timerId() == enableBalance_TimerId)
    {
        impl->cTradeFundSearch();
        //返回
        return;
    }
    //如果为刷新行情数据
    else if(ev->timerId() == flashQuotaData_TimerId)
    {
        //刷新总页数 (PS: 最后一页可能没满)
        quotaTotalPage = keys->size() / C_QuotaPerPage + (keys->size() % C_QuotaPerPage == 0 ? 0 : 1) ;
        //显示行情
        showQuota(quotaCurrentPage);
        //如果信息不为空
        if(!keys->empty())
        {
            //显示详情 -- 初始值为0
            showQuotaDetailInfo(detailInformationRows);
            //显示买卖手
            showBuyTable(detailInformationRows);
            showSellTable(detailInformationRows);
        }
    }
    else if(ev->timerId() == autoSendQuota_TimerId)
    {
        //如果是已连接状态
        if(quotaImpl->connectionStatus() == true)
            //发起委托
            quotaImpl->sendSubscription(nIssueType.toInt(), exchangeType.toUtf8(), contractCode.toUtf8());
    }
    //返回其他
    return QMainWindow::timerEvent(ev);
}
//加载账号信息
const TradeAccount* Account::loadInformation()
{
    //获取
    TradeAccount* user = new TradeAccount;
    //获取Account信息
    QFile* file = new QFile;
    //设置文件名
    file->setFileName("data.xml");
    //打不开 == 没有该文档
    if(!file->open(QIODevice::ReadOnly))
    {
        //创建文件
        file->open(QIODevice::WriteOnly | QIODevice::Truncate);
        //创建xml文档
        QDomDocument doc;
        //写入xml头部
        QDomProcessingInstruction instruction; //添加处理命令
        instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        doc.appendChild(instruction);
        //加入节点
        QDomElement user = doc.createElement("user");
        //加入账号属性
        user.setAttribute("account", "");
        //加入密码属性
        user.setAttribute("password", "");
        //加入委托方式属性
        user.setAttribute("entrustWay", "");
        //加入xml
        doc.appendChild(user);
        //文本流
        QTextStream out(file);
        //保存xml
        doc.save(out, 4, QDomNode::EncodingFromTextStream);
    }
    //如果有
    else
    {
        //关闭
        file->close();
        //创建xml文档
        QDomDocument doc;
        //加载读取文件
        file->open(QIODevice::ReadOnly | QIODevice::Text);
        //读入内容
        doc.setContent(file);
        //获取节点
        QDomElement u = doc.firstChildElement();
        //获取账号值
        memcpy(user->FundAccount, u.attribute("account").toUtf8(), u.attribute("account").length());
        //获取密码值
        memcpy(user->Password, u.attribute("password").toUtf8(), u.attribute("account").length());
        //获取委托方式
        user->EntrustWay = u.attribute("entrustWay").toStdString()[0];
    }
    //关闭文件
    file->close();

    return user;
}
//加载连接
void Account::loadConnection()
{
    //初始化账号连接
    if(impl->InitConn(C_TradeConnectPath) == -1)
    {
        //连接失败
        QMessageBox* box = new QMessageBox(QMessageBox::Warning, "错误", QString("连接服务器失败"));
        //加载提示框
        box->exec();
        //析构
        this->~Account();
        //退出
        exit(-1);
    }
    //连接成功
    connectStatus = true;
}
//加载交易连接
void Account::loadTradeConnection()
{
    //点击上一页 委托查询 1
    QObject::connect(ui->last, &QPushButton::clicked, this, [=](){
        //如果不是首页
        if(entrustCurrentPage != 0){
            //显示列表上一页
            showEntrust(--entrustCurrentPage);
        }
    });
    //点击下一页 委托查询 1
    QObject::connect(ui->next, &QPushButton::clicked, this, [=](){
        //如果不是末尾
        if(entrustCurrentPage < entrustTotalPage - 1){
            //显示列表上一页
            showEntrust(++entrustCurrentPage);
        }
    });
    //点击上一页 持仓查询 2
    QObject::connect(ui->last2, &QPushButton::clicked, this, [=](){
        //如果不是首页
        if(positionCurrentPage != 0){
            //显示列表上一页
            showPosition(--positionCurrentPage);
        }
    });
    //点击下一页 持仓查询 2
    QObject::connect(ui->next2, &QPushButton::clicked, this, [=](){
        //如果不是末尾
        if(positionCurrentPage < positionTotalPage - 1){
            //显示列表上一页
            showPosition(++positionCurrentPage);
        }
    });
    //点击菜单 => 跳转相应界面
    QObject::connect(ui->menu, &QListWidget::clicked, this, [=](const QModelIndex& index){
        //跳转到对应界面
        ui->optionView->setCurrentIndex(index.row());
        //如果是查询界面 => 显示委托信息 1
        if(index.row() == 1)
        {
            //回档目前页数
            entrustCurrentPage = 0;
            //初始化总页数 (PS: 最后一页可能没满)
            entrustTotalPage = entrustList.size() / C_EntrustPerPage + (entrustList.size() % C_EntrustPerPage == 0 ? 0 : 1) ;
            //显示列表
            showEntrust(entrustCurrentPage);
        }
        //如果是2查询界面 => 显示持仓信息 2
        if(index.row() == 2)
        {
            //清空先前的
            positionList.clear();
            //回档目前页数
            positionCurrentPage = 0;
            //显示列表
            showPosition(positionCurrentPage);
            //发起请求： 请求获取持仓信息
            impl->cTradePositionSearch(nullptr, GET_SIZE);
        }
        //如果是4行情查询
        if(index.row() == 4)
        {
            quotaCurrentPage = 0;
        }
    });
    //获取改变数据
    QObject::connect(impl, &CTrade_Impl::changeEntrustNo, this, [=](int no, int newEntrustNo){
        //找到对应成交
        auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
            return info->entrust_no == no;
        });
        if(thisEntrustIter != entrustList.end())
            //刷新值
            (*thisEntrustIter)->entrust_no = newEntrustNo;
        //刷新页面
        showEntrust(entrustCurrentPage);
    });
    QObject::connect(impl, &CTrade_Impl::changeBarginInfo, this, [=](int no, BargainInfo* newBargain){
        //找到对应成交
        auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
            return info->entrust_no == no;
        });
        if(thisEntrustIter != entrustList.end())
        {
            //加入
            (*thisEntrustIter)->bargainList.push_back(newBargain);
            //成交量变化
            (*thisEntrustIter)->totalBusinessAmount += newBargain->businessAmount;
            //如果成交等于下单
            if((*thisEntrustIter)->totalBusinessAmount == (*thisEntrustIter)->entrustAmount)
            {
                //已成
                (*thisEntrustIter)->entrustStatus = '8';
            }
        }
            //刷新页面
        showEntrust(entrustCurrentPage);

    });
    QObject::connect(impl, &CTrade_Impl::changeTodayBarginInfo, this, [=](int no, BargainInfo* newBargain){
        //找到对应成交
        auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
            return info->entrust_no == no;
        });
        if(thisEntrustIter != entrustList.end())
            //加入
            (*thisEntrustIter)->bargainList.push_back(newBargain);
            //刷新页面
        showEntrust(entrustCurrentPage);

    });
    QObject::connect(impl, &CTrade_Impl::changeEntrustStatus, this, [=](int no, char newEntrustStatus){
        //找到对应成交
        auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
            return info->entrust_no == no;
        });
        if(thisEntrustIter != entrustList.end())
            //刷新值
            (*thisEntrustIter)->entrustStatus = newEntrustStatus;
        //刷新页面
        showEntrust(entrustCurrentPage);
    });
    QObject::connect(impl, &CTrade_Impl::changeEntrustDate, this, [=](int no, int newEntrustDate){
        //找到对应成交
        auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
            return info->entrust_no == no;
        });
        if(thisEntrustIter != entrustList.end())
            //刷新值
            (*thisEntrustIter)->entrustDate = newEntrustDate;
        //刷新页面
        showEntrust(entrustCurrentPage);
    });
    QObject::connect(impl, &CTrade_Impl::changeErrorInfo, this, [=](int no, QString newErrorInfo){
        //找到对应成交
        auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
            return info->entrust_no == no;
        });
        if(thisEntrustIter != entrustList.end())
            //刷新值
            strcpy((*thisEntrustIter)->errorInfo, newErrorInfo.toUtf8());
        //刷新页面
        showEntrust(entrustCurrentPage);
    });
    QObject::connect(impl, &CTrade_Impl::changeEntrustNoByRet, this, [=](int ret,int sessionId, int newEntrustNo){
        //找到对应成交
        auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
            return (info->ret == ret && info->sessionNum == sessionId);
        });
        if(thisEntrustIter != entrustList.end())
            //刷新值
            (*thisEntrustIter)->entrust_no = newEntrustNo;
        //刷新页面
        showEntrust(entrustCurrentPage);
    });
    QObject::connect(impl, &CTrade_Impl::changeEntrustStatusByRet, this, [=](int ret, int sessionId, char newEntrustStatus){
        //找到对应成交
        auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
            return (info->ret == ret && info->sessionNum == sessionId);
        });
        if(thisEntrustIter != entrustList.end())
            //刷新值
            (*thisEntrustIter)->entrustStatus = newEntrustStatus;
        //刷新页面
        showEntrust(entrustCurrentPage);
    });
    QObject::connect(impl, &CTrade_Impl::changeEntrustDateByRet, this, [=](int ret, int sessionId, int newEntrustDate){
        //找到对应成交
        auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
            return (info->ret == ret && info->sessionNum == sessionId);
        });
        if(thisEntrustIter != entrustList.end())
            //刷新值
            (*thisEntrustIter)->entrustDate = newEntrustDate;
        //刷新页面
        showEntrust(entrustCurrentPage);
    });
    QObject::connect(impl, &CTrade_Impl::changeErrorInfoByRet, this, [=](int ret,int sessionId, QString newErrorInfo){
        //找到对应成交
        auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
            return (info->ret == ret && info->sessionNum == sessionId);
        });
        if(thisEntrustIter != entrustList.end())
            //刷新值
            strcpy((*thisEntrustIter)->errorInfo, newErrorInfo.toUtf8());
        //刷新页面
        showEntrust(entrustCurrentPage);
    });
}
//加载行情连接
void Account::loadQuotaConnection()
{
    //获取行情信息指针
    keys = quotaImpl->getKeys();
    pStrockMap = quotaImpl->getStrockMap();
    //断线重连
    QObject::connect(quotaImpl, &CQuota_Impl::reconnection, this, [=](){
        //发起连接
        if(quotaImpl->InitConn(C_QuotaConnectPath)== -1)
        {
            //返回
            return;
        }
        //已连接
        quotaImpl->connection();
        //发送行情请求
        quotaImpl->sendSubscription(nIssueType.toInt(), exchangeType.toUtf8(), contractCode.toUtf8());
    });
    //点击上一页 行情查询 3
    QObject::connect(ui->last3, &QPushButton::clicked, this, [=](){
        //如果不是首页
        if(quotaCurrentPage != 0){
            //显示列表上一页
            --quotaCurrentPage;
        }
    });
    //点击下一页 行情查询 3
    QObject::connect(ui->next3, &QPushButton::clicked, this, [=](){
        //如果不是末尾
        if(quotaCurrentPage < quotaTotalPage - 1){
            ++quotaCurrentPage;
        }
    });
    //点击表格行获取信息行数
    QObject::connect(ui->quotaTable, &FreezeTableWidget::clicked, this, [=](const QModelIndex& index){
        //获取信息行数
        detailInformationRows = index.row() + C_QuotaPerPage * quotaCurrentPage;
    });
    //双击加载走势
    QObject::connect(ui->detailWidget, &ClickedWidget::doubleClicked, this, [=](){
        stockView = new StockWindow(pStrockMap->find((*keys)[detailInformationRows]/*.first*/)->second);/**/
    });
}
//加载登录会话
void Account::loadLogin()
{
    //创建登录会话框
    LoginDialog* loginDlg = new LoginDialog();

    //获取登录信息
    QObject::connect(impl, &CTrade_Impl::getLoginMsg, this, [=](QString msg){
        ui->information->setText(msg);
    });
    //点击登录 => 登录
    QObject::connect(loginDlg, &LoginDialog::clickedDialog, loginDlg, [=](){
        //获取账号信息
        user = loginDlg->loadInformation();
        //判断登录
        QObject::connect(impl, &CTrade_Impl::loginIsOk, this, [=](bool isOk){
            //登录成功后 => 用户操作连接
            if(isOk){
                // 1.加载交易连接事件处理
                loadTradeConnection();
                // 2.加载委托
                loadNewEntrust();
                // 3.加载持仓
                loadNewPosition();
                // 4.加载资产
                loadNewFund();
                // 5.下单
                //点击下单 => 下单
                QObject::connect(ui->orderConfirm, &QPushButton::clicked, this, &Account::order);
                // 6.发起行情订阅
                //发起行情订阅
                QObject::connect(ui->subscription, &QPushButton::clicked, this, &Account::subscribe);
                // 6.开启可用资金计时
                enableBalance_TimerId = startTimer(C_EnableBalance_TimeDis);
                // 7.自动发起行情
                autoSendQuota_TimerId = startTimer(C_AutoSend_TimeDis);
                // 7.初始化资金
                impl->cTradeFundSearch();
            }
        });
        //关闭窗口
        loginDlg->close();
    });
    //关闭登录
    QObject::connect(loginDlg, &LoginDialog::closedDialog, this, [=](){
        //点击登录造成的关闭
        if(loginDlg->isClicked == true){
            //回调登录
            impl->cTradeTransmissionApprove(user);
            //显示操作主界面(最大化)
            this->showMaximized();
        }
        //直接关闭
        else{
            this->~Account();
            exit(-1);
        }
    });
    //显示
    loginDlg->exec();
}
//加载委托
void Account::loadNewEntrust()
{
    //获取新委托
    QObject::connect(impl, &CTrade_Impl::getNewEntrustMsg, this, [=](EntrustInfo *newEntrust){
        entrustList.push_back(newEntrust);
    });
    //加载当日成交
    QObject::connect(impl, &CTrade_Impl::loadTodayEntrustEnding, this, [=](){
        impl->cTradeTodayBargainSearch(nullptr, GET_SIZE);
    });
    //加载当日委托
    impl->cTradeTodayEntrustSearch(nullptr, GET_SIZE);
}
//加载持仓
void Account::loadNewPosition()
{
    //获取新委托 -- 刷新表
    QObject::connect(impl, &CTrade_Impl::getNewPositionMsg, this, [=](PositionInfo *newPosition){
        //加入持仓
        positionList.push_back(newPosition);
        //初始化总页数 (PS: 最后一页可能没满)
        positionTotalPage = positionList.size() / C_PositionPerPage + (entrustList.size() % C_PositionPerPage == 0 ? 0 : 1) ;
        //显示列表
        showPosition(positionCurrentPage);
    });
}
//加载资金
void Account::loadNewFund()
{
    QObject::connect(impl, &CTrade_Impl::getNewFund, this, [=](const FundInfo* newFund){
        //显示可用资金
        ui->availableFund->setText(QString("可用资金: ") + QString::number(newFund->enableBalance, 'f', 2));
        //显示所有信息
        ui->fundAccountText->setText(QString::fromUtf8(newFund->fundAccount));
        ui->beginBalanceText->setText(QString::number(newFund->beginBalance, 'f', 2));
        ui->currentBalanceText->setText(QString::number(newFund->currentBalance, 'f', 2));
        ui->enableBalanceText->setText(QString::number(newFund->enableBalance, 'f', 2));
        ui->preEntrustBalanceText->setText(QString::number(newFund->preEntrustBalance, 'f', 2));
        ui->entrustBalanceText->setText(QString::number(newFund->entrustBalance, 'f', 2));
        ui->entrustFareText->setText(QString::number(newFund->entrustFare, 'f', 2));
        ui->holdIncomeText->setText(QString::number(newFund->holdIncome, 'f', 2));
        ui->holdIncomeFloatText->setText(QString::number(newFund->holdIncomeFloat, 'f', 2));
        ui->beginEquityBalanceText->setText(QString::number(newFund->beginEquityBalance, 'f', 2));
        ui->equityBalanceText->setText(QString::number(newFund->equityBalance, 'f', 2));
        ui->holdMarginText->setText(QString::number(newFund->holdMargin, 'f', 2));
        ui->clientRiskRateText->setText(QString::number(newFund->clientRiskRate, 'f', 2));
        //析构
        delete newFund;
    });

}
//下单
void Account::order()
{
    //判断是否为空
    if(ui->futuCode->text().isEmpty() || ui->entrustAmount->text().isEmpty() || ui->futuEntrustPrice->text().isEmpty())
    {
        ui->error->setText(C_ErrorInfo);
        return;
    }
    ui->error->clear();
    //定义需要实参
    char exchType[4];
    char code[30];
    //初始化 上述实参
    memset(exchType, '\0' , 4);
    memset(code, '\0' , 30);
    strcpy(exchType, ui->futuExchType->currentText().split(" ")[0].toUtf8());
    strcpy(code, ui->futuCode->text().toUtf8());
    //获取数值
    char bs = ui->entrustBs->currentIndex() + C_Int2Char; // 数字 转 数字字符
    int amount = ui->entrustAmount->text().toInt();
    char direction = ui->futuresDirection->currentIndex() + C_Int2Char;
    double price = ui->futuEntrustPrice->text().toDouble();

    //338202 -- 下单
    impl->cTradeSendEntrust(exchType, code, bs, direction, amount, price);
}
//订阅行情
void Account::subscribe()
{
    //发起连接
    if(quotaImpl->InitConn(C_QuotaConnectPath)== -1)
    {
        //消息
        emit quotaImpl->setSubStatus("订阅状态: 关");
        //连接失败
        QMessageBox* box = new QMessageBox(QMessageBox::Warning, "错误", QString("连接行情服务器失败"));
        //加载提示框
        box->exec();
        //失败
        emit quotaImpl->appendMsg("订阅行情失败");
        //返回
        return;
    }
    //已连接
    quotaImpl->connection();
    //加载行情连接
    loadQuotaConnection();
    //打开订阅会话框
    QuotaDialog* dialog = new QuotaDialog;
    //设置关闭析构
    dialog->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
    //会话框发起行情查询
    QObject::connect(dialog, &QuotaDialog::confirmValue, this, [=](QString IssueType, QString ExchangeType, QString ContractCode){
        if(!IssueType.isEmpty() && !ExchangeType.isEmpty() && !ContractCode.isEmpty()){
            this->nIssueType = IssueType;
            this->exchangeType = ExchangeType;
            this->contractCode = ContractCode;
            //查询行情订阅
            quotaImpl->sendSubscription(nIssueType.toInt(), exchangeType.toUtf8(), contractCode.toUtf8());
            //跳转行情查询界面
            ui->optionView->setCurrentIndex(4);
            //开启刷新数据
            flashQuotaData_TimerId = startTimer(C_FlashQuotaData_TimeDis);
            //关闭
            dialog->close();
        }
        else{
            dialog->showError(C_ErrorInfo);
        }
    });
    //查看所有订阅
    QObject::connect(dialog, &QuotaDialog::allValue, this, [=](QString IssueType){
        if(!IssueType.isEmpty()){
            this->nIssueType = IssueType;
            //查询行情订阅
            quotaImpl->sendSubscription(nIssueType.toInt(), "", "");
            //跳转行情查询界面
            ui->optionView->setCurrentIndex(4);
            //开启刷新数据
            flashQuotaData_TimerId = startTimer(C_FlashQuotaData_TimeDis);
            //关闭
            dialog->close();
        }
        else{
            dialog->showError(C_ErrorInfo);
        }
    });
    //打开
    dialog->exec();
}
//显示委托列表
void Account::showEntrust(int page)
{
    //创建表模型
    MyTableModel* model = new MyTableModel;
    //创建表头
    QStringList header;
    //赋值
    header << "委托编号" << "交易类别"<< "合约代码"<< "买卖方向"<< "开平仓方向"<< "投机/套保类型"<< "委托数量"<< "总成交数量"<< "委托价格"<< "委托状态"<< "委托时间" << "详情" << "撤单" << "错误信息";
    //表头加入表
    model->setHorizontalHeaderLabels(header);
    //遍历加入数据
    for(int rows = 0, i = page * C_EntrustPerPage; i < (page + 1) * C_EntrustPerPage && i < static_cast<int>(entrustList.size()); ++i, ++rows)
    {
        //加入数据
        model->setItem(rows, 0, new QStandardItem(QString::number(entrustList[i]->entrust_no)));
        model->setItem(rows, 1, new QStandardItem(QString::fromUtf8(entrustList[i]->futuExchType)));
        model->setItem(rows, 2, new QStandardItem(QString::fromUtf8(entrustList[i]->futuCode)));
        model->setItem(rows, 3, new QStandardItem(EntrustBs2String(entrustList[i]->entrustBs)));
        model->setItem(rows, 4, new QStandardItem(FuturesDirection2String(entrustList[i]->futuresDirection)));
        model->setItem(rows, 5, new QStandardItem(HedgeType2String(entrustList[i]->hedgeType)));
        model->setItem(rows, 6, new QStandardItem(QString::number(entrustList[i]->entrustAmount)));
        model->setItem(rows, 7, new QStandardItem(QString::number( entrustList[i]->totalBusinessAmount, 'f', 2)));//浮点型 小数点后6位
        model->setItem(rows, 8, new QStandardItem(QString::number(entrustList[i]->futuEntrustPrice, 'f', 2)));//浮点型 小数点后6位
        model->setItem(rows, 9, new QStandardItem(EntrustStatus2String(entrustList[i]->entrustStatus)));
        model->setItem(rows, 10, new QStandardItem(QString::number(entrustList[i]->entrustDate / 10000) + QString(":") + QString("%1").arg((entrustList[i]->entrustDate % 10000 - entrustList[i]->entrustDate % 100) / 100, 2, 10, QLatin1Char('0')) + QString(":") + QString("%1").arg(entrustList[i]->entrustDate % 100, 2, 10, QLatin1Char('0'))));
        model->setItem(rows, 13, new QStandardItem(QString::fromUtf8(entrustList[i]->errorInfo)));

    }
    //设置模型
    setTableViewModel(ui->entrustTable, model);
    //加入操作按钮
    for(int rows = 0, i = page * C_EntrustPerPage; i < (page + 1) * C_EntrustPerPage && i < static_cast<int>(entrustList.size()); ++i, ++rows)
    {
        //创建详情按钮
        //创建控件
        ClickedLabel* newOption = new ClickedLabel("<u>成交详情</u>");
        //设置连接
        QObject::connect(newOption, &ClickedLabel::Clicked, this, [=](){
            //定义模型
            MyTableModel* model1 = new MyTableModel;
            //创建表头
            QStringList header1;
            //赋值
            header1 << "成交编号" << "合约代码" << "成交价格" << "成交数量" << "成交时间" ;
            //加入表头
            model1->setHorizontalHeaderLabels(header1);
            //遍历加入数据
            for(int j = 0; j < static_cast<int>(entrustList[i]->bargainList.size()); ++j)
            {
                //加入数据
                model1->setItem(j, 0, new QStandardItem(QString::fromUtf8(entrustList[i]->bargainList[j]->businessId)));
                model1->setItem(j, 1, new QStandardItem(QString::fromUtf8(entrustList[i]->futuCode)));
                model1->setItem(j, 2, new QStandardItem(QString::number(entrustList[i]->bargainList[j]->futuBusinessPrice, 'f', 2)));
                model1->setItem(j, 3, new QStandardItem(QString::number(entrustList[i]->bargainList[j]->businessAmount)));
                model1->setItem(j, 4, new QStandardItem(QString::number(entrustList[i]->bargainList[j]->businessTime / 10000) + QString(":") + QString("%1").arg((entrustList[i]->bargainList[j]->businessTime % 10000 - entrustList[i]->bargainList[j]->businessTime % 100) / 100, 2, 10, QLatin1Char('0')) + QString(":") + QString("%1").arg(entrustList[i]->bargainList[j]->businessTime % 100, 2, 10, QLatin1Char('0'))));
            }
            //设置模型
            setTableViewModel(ui->bargainTable, model1);
        });
        //添加控件
        ui->entrustTable->setIndexWidget(ui->entrustTable->model()->index(rows, 11), newOption);

        //创建撤单按钮
        //创建控件
        ClickedLabel* cancel = new ClickedLabel("<u>撤单</u>");
        //设置连接
        QObject::connect(cancel, &ClickedLabel::Clicked, this, [=](){
            //获取对应委托编号
            int no = ui->entrustTable->model()->index(rows, 0).data().toInt();
            //获取状态
            char status = ui->entrustTable->model()->index(rows, 9).data().toString().toStdString()[1];
            // 可以撤单
            if((status == '0' || status == '2' || status == '7') && (no != 0))
            {
                //撤单
                impl->cTradeEntrustBackout(no);
                //说明有成交 --> 对应合约持仓会发生变化
                auto thisEntrustIter = std::find_if(entrustList.cbegin(), entrustList.cend(), [=](const EntrustInfo* info){
                    return info->entrust_no == no;
                });
                //改变状态
                (*thisEntrustIter)->entrustStatus = '3';
            }
            // 非正常委托无法撤单
            else
                //错误信息
                QMessageBox::warning(nullptr, "警告", "非正常委托, 无法撤单", QMessageBox::Ok);
        });
        //添加控件
        ui->entrustTable->setIndexWidget(ui->entrustTable->model()->index(rows, 12), cancel);
    }
}
//持仓查询
void Account::showPosition(int page)
{
    //创建表模型
    MyTableModel* model = new MyTableModel;
    //创建表头
    QStringList header;
    //赋值
    header << "交易类别" << "合约代码" << "买卖方向" << "期初数量" << "可用数量" << "当日开仓可用数量" << "投机/套保类型" << "成交数量" <<  "今总持仓" << "老仓持仓数量";
    //表头加入表
    model->setHorizontalHeaderLabels(header);
    //遍历加入数据
    for(int rows = 0, i = page * C_PositionPerPage; i < (page + 1) * C_PositionPerPage && i < static_cast<int>(positionList.size()); ++i, ++rows)
    {
        //加入数据
        model->setItem(rows, 0, new QStandardItem(QString::fromUtf8(positionList[i]->futuExchType)));
        model->setItem(rows, 1, new QStandardItem(QString::fromUtf8(positionList[i]->futuCode)));
        model->setItem(rows, 2, new QStandardItem(EntrustBs2String(positionList[i]->entrustBs)));
        model->setItem(rows, 3, new QStandardItem(QString::number(positionList[i]->beginAmount)));
        model->setItem(rows, 4, new QStandardItem(QString::number(positionList[i]->enableAmount)));
        model->setItem(rows, 5, new QStandardItem(QString::number(positionList[i]->realEnableAmount)));
        model->setItem(rows, 6, new QStandardItem(HedgeType2String(positionList[i]->hedgeType)));
        model->setItem(rows, 7, new QStandardItem(QString::number(positionList[i]->realAmount)));//浮点型 小数点后6位
        model->setItem(rows, 8, new QStandardItem(QString::number(positionList[i]->realCurrentAmount, 'f', 2)));//浮点型 小数点后6位
        model->setItem(rows, 9, new QStandardItem(QString::number(positionList[i]->oldCurrentAmount)));
    }
    //设置模型
    setTableViewModel(ui->positionTable, model);
}
//行情查询
void Account::showQuota(int page)
{
    //创建初始化模型
    QStandardItemModel* model = new QStandardItemModel;
    //创建表头
    QStringList header;
    //赋值
    header <<"合约代码          " << "合约名称          "  << " 开盘                            " << " 最高                            " << " 最低                            " << " 最新                            " << " 涨停                            " << " 跌停                            "/*<<"测试次数"*/;
    //加入表头
    model->setHorizontalHeaderLabels(header);
    //获取容器当前大小
    int size = static_cast<int>(keys->size());
    //遍历加入数据
    for(int rows = 0, i = page * C_QuotaPerPage; i < (page + 1) * C_QuotaPerPage && i < size; ++i, ++rows)
    {
        UnifiedQuotaData* detail = pStrockMap->find((*keys)[i]/*.first*/)->second;/**/
        //加入数据
        model->setItem(rows, 0, new QStandardItem(QString::fromUtf8(detail->m_sContractCode)));
        model->setItem(rows, 1, new QStandardItem(QString::fromUtf8(detail->m_sContractName)));
        model->setItem(rows, 2, new QStandardItem(QString::number(detail->m_nOpenPrice)));
        model->setItem(rows, 3, new QStandardItem(QString::number(detail->m_nHighestPrice)));
        model->setItem(rows, 4, new QStandardItem(QString::number(detail->m_nLowestPrice)));
        model->setItem(rows, 5, new QStandardItem(QString::number(static_cast<double>(detail->m_nLastPrice) / (double) detail->m_nPriceUnit, 'f', 4)));
        model->setItem(rows, 6, new QStandardItem(QString::number(detail->m_nUpLimitPrice)));
        model->setItem(rows, 7, new QStandardItem(QString::number(detail->m_nDownLimitPrice)));
//        model->setItem(rows, 8, new QStandardItem(QString::number((*keys)[i].second)));/**/
    }
    //设置模型
    setTableViewModel(ui->quotaTable, model);
    ui->quotaTable->init();
}
//展示指定行数信息
void Account::showQuotaDetailInfo(int index)
{
    //获取当前行键值
    std::string currentKey = (*keys)[index]/*.first*/;
    //获取相应迭代器
    const UnifiedQuotaData* detail = pStrockMap->find(currentKey)->second;/**/
    //获取键值信息
    ui->preCloseValue->setText(QString::number(detail->m_nPreClosePrice, 'f', 2));
    ui->preSettleValue->setText(QString::number(detail->m_nPreSettlePrice, 'f', 2));
    ui->closePriceValue->setText(QString::number(detail->m_nClosePrice, 'f', 2));
    ui->settlePriceValue->setText(QString::number(detail->m_nSettlePrice, 'f', 2));
    ui->businessAmountValue->setText(QString::number(static_cast<double>(detail->m_fBusinessAmount)/(double)1000000, 'f', 2) + QString("万手"));
    ui->businessBalanceValue->setText(QString::number(static_cast<double>(detail->m_fBusinessBalance)/(double)10000, 'f', 2) + QString("万元"));
    ui->holdAmountValue->setText(QString::number(detail->m_fHoldAmount, 'f', 2));
}
//行情买手
void Account::showBuyTable(int index)
{
    //获取当前行键值
    std::string currentKey = (*keys)[index]/*.first*/;
    //获取相应迭代器
    UnifiedQuotaData* detail = pStrockMap->find(currentKey)->second;/**/
    //获取buy值
    //创建初始化模型
    BuyModel* model = new BuyModel;
    //列
    for(int column = 0; column != 3; ++column)
    {
        //行
        switch (column) {
        case 0:
            //编号列
            for(int rows = 0; rows != 5; ++rows) model->setItem(rows, column, new QStandardItem(QString::number(rows + 1)));
            break;
        case 1:
            //报价列
            for(int rows = 0; rows != 5; ++rows) model->setItem(rows, column, new QStandardItem(QString::number(static_cast<double>(detail->m_nBidPrice[rows]) / (double) detail->m_nPriceUnit, 'f', 4)));
            break;
        case 2:
            //量列
            for(int rows = 0; rows != 5; ++rows) model->setItem(rows, column, new QStandardItem(QString::number(static_cast<double>(detail->m_nBidVolumn[rows])/100)));
            break;
        }
    }
    //设置模型
    setTableViewModel(ui->buyTable, model);
}
//行情卖手
void Account::showSellTable(int index)
{
    //获取当前行键值
    std::string currentKey = (*keys)[index]/*.first*/;
    //获取相应迭代器
    UnifiedQuotaData* detail = pStrockMap->find(currentKey)->second;/**/
    //获取sell值
    //创建初始化模型
    SellModel* model = new SellModel;
    //列
    for(int column = 0; column != 3; ++column)
    {
        //行
        switch (column) {
        case 0:
            //编号列
            for(int rows = 0; rows != 5; ++rows) model->setItem(rows, column, new QStandardItem(QString::number(5 - rows)));
            break;
        case 1:
            //报价列
            for(int rows = 0; rows != 5; ++rows) model->setItem(rows, column, new QStandardItem(QString::number(static_cast<double>(detail->m_nAskPrice[4-rows]) / (double) detail->m_nPriceUnit, 'f', 4)));
            break;
        case 2:
            //量列
            for(int rows = 0; rows != 5; ++rows) model->setItem(rows, column, new QStandardItem(QString::number(static_cast<double>(detail->m_nAskVolumn[4-rows])/100)));
            break;
        }
    }
    //设置模型
    setTableViewModel(ui->sellTable, model);
}
