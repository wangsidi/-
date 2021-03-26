#ifndef CTRADEIMPLINTERFACE_H
#define CTRADEIMPLINTERFACE_H
//前置声明
class EntrustInfo;
class IF2UnPacker;
class IBizMessage;
class TradeAccount;
class UnifiedQuotaData;
//CTrade_Impl 自定义接口
struct CTradeImplInterFace{
    //331480 穿透式认证请求
    virtual int cTradeTransmissionApprove(const TradeAccount* user) = 0;
    //331481 客户端认证请求
    virtual int cTradeClientApprove() = 0;
    //331482 客户端系统信息上报请求
    virtual int cTradeClientAppear() = 0;
    //331100 登入
    virtual int cTradeLogin() = 0;
    //338202期货委托确认
    virtual int cTradeSendEntrust(const char* pszExchangeType, const char* pszFutuCode, char cEntrustBs, char cFuturesDirection, int nEntrustAmount, double dFutuEntrustPrice) = 0;
    //338217期货撤单委托
    virtual int cTradeEntrustBackout(int nEntrustNo) = 0;
    //338300期货资金查询
    virtual int cTradeFundSearch() = 0;
    //338301期货当日委托查询
    virtual int cTradeTodayEntrustSearch(const char* pszPositionStr, int nRequestNum) = 0;
    //338302期货当日成交查询
    virtual int cTradeTodayBargainSearch(const char* pszPositionStr, int nRequestNum) = 0;
    //338303期货持仓查询
    virtual int cTradePositionSearch(const char* pszPositionStr, int nRequestNum) = 0;
    //620001_33101期货订阅委托成交回报功能
    virtual int sendSubscription(int nIssueType) = 0;
    //生成新委托
    virtual EntrustInfo* createNewEntrust(const char* pszExchangeType, const char* pszFutuCode, char cEntrustBs, char cFuturesDirection, int nEntrustAmount, double dFutuEntrustPrice) = 0;
    virtual EntrustInfo* createNewEntrust(const int entrustNo, const char* pszExchangeType, const char* pszFutuCode, char cEntrustBs, char cFuturesDirection, char cHedgeType, int nEntrustAmount, double cTotalBusinessAmount, double dFutuEntrustPrice, int cEntrustDate, char cEntrustStatus) = 0;
    //331480响应处理
    virtual void DealTradeTransmissionApprove(IF2UnPacker* pUnPacker) = 0;
    //331481响应处理
    virtual void DealTradeClientApprove(IF2UnPacker* pUnPacker) = 0;
    //331482响应处理
    virtual void DealTradeClientAppear(IF2UnPacker* pUnPacker) = 0;
    //331100响应处理
    virtual void DealTradeLogin(IF2UnPacker* pUnPacker) = 0;
    //338202响应处理
    virtual void DealTradeSendEntrust(int Ret, IF2UnPacker* pUnPacker) = 0;
    //338217响应处理
    virtual void DealTradeEntrustBackout(IF2UnPacker* pUnPacker) = 0;
    //338300响应处理
    virtual void DealTradeFundSearch(IF2UnPacker *pUnPacker) = 0;
    //338302响应处理
    virtual void DealTradeTodayBargainSearch(IF2UnPacker *pUnPacker) = 0;
    //338303响应处理
    virtual void DealTradePositionSearch(IF2UnPacker *pUnPacker) = 0;
    //620000响应处理
    virtual void DealRepFunction620000(IBizMessage* lpMsg) = 0;
    //620001响应处理
    virtual void DealRepFunction620001(IF2UnPacker* pUnPacker) = 0;
    //620003响应处理
    virtual void DealRepFunction620003(IF2UnPacker* pUnPacker) = 0;
};

#endif // CTRADEIMPLINTERFACE_H
