#ifndef FUTU_QUOTA_H
#define FUTU_QUOTA_H
#include <QObject>

#include "t2sdk_interface.h"
#include "HsFutuSystemInfo.h"
#include "Func.h"

#include "CTradeImplInterface.h"
//前置声明
class QuotaData;
//类型声明
//定义code键值
typedef std::string CodeKey;
//定义股票map
typedef std::map<CodeKey, UnifiedQuotaData*> StrockMap;
// 自定义类CCallback，通过继承（实现）CCallbackInterface，来自定义各种事件（包括连接成功、
// 连接断开、发送完成、收到数据等）发生时的回调方法
//By inheriting the CCallbackInterface to define events(include connection success、disconnect、send enging、receive data and so on)
class CQuota_Impl : public QObject, public CCallbackInterface
{
    Q_OBJECT
signals:
    void appendMsg(const QString msg);
    void setSubStatus(const QString msg);
    //断线重连
    void reconnection();
public:
    //构造函数
    CQuota_Impl();
    //析构函数
    virtual ~CQuota_Impl();

public:
//    inline const std::vector<std::pair<CodeKey, int>>* getKeys() const {return &keys;}/**/
    inline const std::vector<CodeKey>* getKeys() const {return &keys;}
    inline const StrockMap* getStrockMap() const {return pStrockMap;}

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
    //已连接
    void connection() { isConnect = true; }
    //连接状态
    bool connectionStatus() const { return isConnect; }
    //620001_33101期货订阅委托成交回报功能
    int sendSubscription(int nIssueType, const char* exchangeType, const char* contractCode);
    //620000响应处理
    void DealRepFunction620000(IBizMessage* lpMsg = nullptr);
    //620001响应处理
    void DealRepFunction620001(IF2UnPacker* pUnPacker = nullptr);
    //620003响应处理
    void DealRepFunction620003(const UnifiedQuotaData* contentBuffer);
    //维护心跳
    void OnHeartbeat(IBizMessage* pMsg);

private:
    //T2配置变量
    CConfigInterface*				pConfig;
    //T2连接变量
    CConnectionInterface*			pConnection;
    //股票表
    StrockMap* pStrockMap;
    //股票键
//    std::vector<std::pair<CodeKey, int>> keys;/**/
    std::vector<CodeKey> keys;
    //是否连接
    bool isConnect;
};

#endif
