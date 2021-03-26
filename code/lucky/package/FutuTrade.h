#ifndef FUTU_TRADE_H
#define FUTU_TRADE_H
#include <QObject>

#include "t2sdk_interface.h"
#include "HsFutuSystemInfo.h"
#include "Func.h"

#include "CTradeImplInterface.h"
//前置声明
class BargainInfo;
class EntrustInfo;
class FundInfo;
class PositionInfo;
class UnifiedQuotaData;
// 自定义类CCallback，通过继承（实现）CCallbackInterface，来自定义各种事件（包括连接成功、
// 连接断开、发送完成、收到数据等）发生时的回调方法
//By inheriting the CCallbackInterface to define events(include connection success、disconnect、send enging、receive data and so on)
class CTrade_Impl : public QObject, public CCallbackInterface, public CTradeImplInterFace
{
    Q_OBJECT
signals:
    void appendMsg(const QString msg);
    void loginIsOk(bool isOk);
    void getLoginMsg(const QString msg);
    void getNewEntrustMsg(EntrustInfo* newEntrust);
    void getNewPositionMsg(PositionInfo* newPosition);
    void getNewFund(FundInfo* newFund);
    void connectClose();
    void loadTodayEntrustEnding();
    //根据下标改变数据
    void changeBarginInfo(int no, BargainInfo* newBargain);
    void changeTodayBarginInfo(int no, BargainInfo* newBargain);
    void changeEntrustNo(int no, int newEntrustNo);
    void changeEntrustStatus(int no, char newEntrustStatus);
    void changeEntrustDate(int no, int newEntrustDate);
    void changeErrorInfo(int no, QString newErrorInfo);

    void changeEntrustNoByRet(int ret, int sessionId, int newEntrustNo);
    void changeEntrustStatusByRet(int ret, int sessionId, char newEntrustStatus);
    void changeEntrustDateByRet(int ret, int sessionId, int newEntrustDate);
    void changeErrorInfoByRet(int ret, int sessionId, QString newErrorInfo);
public:
    //构造函数
    CTrade_Impl();
    //析构函数
    virtual ~CTrade_Impl();

public:
    // 因为CCallbackInterface的最终纯虚基类是IKnown，所以需要实现一下这3个方法
    //the funally calss of CCallbackInterface is IKnown,so should realize the three
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release();

    // 各种事件发生时的回调方法，实际使用时可以根据需要来选择实现，对于不需要的事件回调方法，可直接return
    // Reserved?为保留方法，为以后扩展做准备，实现时可直接return或return 0。
    void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData);
    void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d);
    void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d);
    int  FUNCTION_CALL_MODE Reserved3();
    void FUNCTION_CALL_MODE Reserved4();
    void FUNCTION_CALL_MODE Reserved5();
    void FUNCTION_CALL_MODE Reserved6();
    void FUNCTION_CALL_MODE Reserved7();
    void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult);
    void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult);
    void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg);
public:
    //初始化连接对象
    int InitConn(QString infoPath);

    //331480 穿透式认证请求
    int cTradeTransmissionApprove(const TradeAccount* user);
    //331481 客户端认证请求
    int cTradeClientApprove();
    //331482 客户端系统信息上报请求
    int cTradeClientAppear();
    //331100 登入
    int cTradeLogin();
    //338202期货委托确认
    int cTradeSendEntrust(const char* pszExchangeType, const char* pszFutuCode, char cEntrustBs, char cFuturesDirection, int nEntrustAmount, double dFutuEntrustPrice);
    //338217期货撤单委托
    int cTradeEntrustBackout(int nEntrustNo);
    //338300期货资金查询
    int cTradeFundSearch();
    //338301期货当日委托查询
    int cTradeTodayEntrustSearch(const char* pszPositionStr, int nRequestNum);
    //338302期货当日成交查询
    int cTradeTodayBargainSearch(const char* pszPositionStr, int nRequestNum);
    //338303期货持仓查询
    int cTradePositionSearch(const char* pszPositionStr, int nRequestNum);
    //620001_33101期货订阅委托成交回报功能
    int sendSubscription(int nIssueType);

    //生成新委托
    EntrustInfo* createNewEntrust(const char* pszExchangeType, const char* pszFutuCode, char cEntrustBs, char cFuturesDirection, int nEntrustAmount, double dFutuEntrustPrice);
    EntrustInfo* createNewEntrust(const int entrustNo, const char* pszExchangeType, const char* pszFutuCode, char cEntrustBs, char cFuturesDirection, char cHedgeType, int nEntrustAmount, double cTotalBusinessAmount, double dFutuEntrustPrice, int cEntrustDate, char cEntrustStatus);
    //331480响应处理
    void DealTradeTransmissionApprove(IF2UnPacker* pUnPacker);
    //331481响应处理
    void DealTradeClientApprove(IF2UnPacker* pUnPacker);
    //331482响应处理
    void DealTradeClientAppear(IF2UnPacker* pUnPacker);
    //331100响应处理
    void DealTradeLogin(IF2UnPacker* pUnPacker);
    //338202响应处理
    void DealTradeSendEntrust(int Ret, IF2UnPacker* pUnPacker);
    //338217响应处理
    void DealTradeEntrustBackout(IF2UnPacker* pUnPacker);
    //338300响应处理
    void DealTradeFundSearch(IF2UnPacker *pUnPacker);
    //338301响应处理
    void DealTradeTodayEntrustSearch(IF2UnPacker *pUnPacker);
    //338302响应处理
    void DealTradeTodayBargainSearch(IF2UnPacker *pUnPacker);
    //338303响应处理
    void DealTradePositionSearch(IF2UnPacker *pUnPacker);
    //620000响应处理
    void DealRepFunction620000(IBizMessage* lpMsg);
    //620001响应处理
    void DealRepFunction620001(IF2UnPacker* pUnPacker);
    //620003响应处理
    void DealRepFunction620003(IF2UnPacker* pUnPacker);
    //维护心跳
    void OnHeartbeat(IBizMessage* pMsg);

private:
    //T2配置变量
    CConfigInterface*				pConfig;
    //T2连接变量
    CConnectionInterface*			pConnection;

    //资金账号
    char							szFundAccount[32];
    //交易密码
    char							szPassword[32];
    //委托方式
    char							cEntrustWay;
    //看穿式会话号（看穿式331480应答获取）
    char							szAppSessionID[64];
    //UserToken（登录331100应答获取）
    char							szUserToken[64];
    //营业部号（登录331100应答获取）
    int								nBranchNo;
    //客户编号（登录331100应答获取）
    char							szClientId[18];
    //操作分支机构（登录331100应答获取）
    int								nOpBranchNo;
private:
    //用于访问内存中的账号信息 不用在这析构
    const TradeAccount* user = nullptr;
};

#endif
