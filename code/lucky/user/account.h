#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "accountinterface.h"
//用户类
class Account : public QMainWindow, public AccountInterface
{
    Q_OBJECT
public:
    Account(QWidget* parent = nullptr);
    virtual ~Account();
public:
    void start();
Q_SIGNALS:
    //改变持仓
    void changePosition(const char* code, int entrustBs, int amount);
    //主窗体关闭
public:
    //时间事件
    void timerEvent(QTimerEvent* ev) override;
protected:
    //加载信息
    const TradeAccount* loadInformation() override;
    //加载连接
    void loadConnection() override;
    //加载交易连接
    void loadTradeConnection() override;
    //加载行情连接
    void loadQuotaConnection() override;
    //加载登录会话
    void loadLogin() override;
    //加载委托
    void loadNewEntrust()override;
    //加载持仓
    void loadNewPosition() override;
    //加载资金
    void loadNewFund() override;
protected Q_SLOTS:
    //下单
    void order() override;
    //资金
    void subscribe() override;
private:
    //显示委托信息
    void showEntrust(int page);
    //持仓查询
    void showPosition(int page);
    //行情查询
    void showQuota(int page);
    //行情详情查询
    void showQuotaDetailInfo(int index);
    //行情买手
    void showBuyTable(int index);
    //行情卖手
    void showSellTable(int index);
private:
    //操作界面
    Ui::Account* ui;
    //实现对象
    CTrade_Impl* impl;
    //行情对象
    CQuota_Impl* quotaImpl;
    //用户账号
    const TradeAccount* user;
    //委托数量
    std::vector<EntrustInfo*> entrustList;
    //持仓数量
    std::vector<PositionInfo*> positionList;
//计时器
private:
    //可用资金计时器
    int enableBalance_TimerId;
    //行情数据刷新计时器
    int flashQuotaData_TimerId;
    //自动重新发起行情
    int autoSendQuota_TimerId;
    //股票表
    const StrockMap* pStrockMap;
    //股票键
//     const std::vector<std::pair<CodeKey, int>>* keys;/**/
    const std::vector<CodeKey>* keys;
     //股票图
     StockWindow* stockView;
private:
     QString nIssueType;
     QString exchangeType;
     QString contractCode;

     //连接状态
     bool connectStatus = false;
};

#endif // ACCOUNT_H
