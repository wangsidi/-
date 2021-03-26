#ifndef ACCOUNTINTERFACE_H
#define ACCOUNTINTERFACE_H

#include <vector>
#include <QMainWindow>
//前置声明
namespace Ui {
class Account;
}
class CTrade_Impl;
class CQuota_Impl;
class EntrustInfo;
class Fund;
class StockWindow;
class PositionInfo;
class TradeAccount;
class UnifiedQuotaData;
//类型声明
//定义code键值
typedef std::string CodeKey;
//定义股票map
typedef std::map<CodeKey, UnifiedQuotaData*> StrockMap;
//用户类接口
struct AccountInterface
{
    //加载连接
    virtual void loadConnection() = 0;
    //加载登录会话
    virtual void loadLogin() = 0;
    //加载信息
    virtual const TradeAccount* loadInformation() = 0;
    //加载交易连接
    virtual void loadTradeConnection() = 0;
    //加载行情连接
    virtual void loadQuotaConnection() = 0;
    //加载委托
    virtual void loadNewEntrust() = 0;
    //加载持仓
    virtual void loadNewPosition() = 0;
    //加载资金
    virtual void loadNewFund() = 0;

    //下单
    virtual void order() = 0;
    //订阅
    virtual void subscribe() = 0;
};

#endif // ACCOUNTINTERFACE_H
