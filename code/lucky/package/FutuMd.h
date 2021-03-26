#ifndef FUTU_MD_H
#define FUTU_MD_H

#include "t2sdk_interface.h"
#include "Func.h"

//行情实现类
//quote realization class
class CMd_Impl : public CCallbackInterface
{
public:
	// 因为CCallbackInterface的最终纯虚基类是IKnown，所以需要实现一下这3个方法
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
	//构造函数
	//constructed function
	CMd_Impl();
	//析构函数
	//Destructor
	~CMd_Impl();

	//注册前置机网络地址
	//Register front-end machine network address
	void RegisterFront(const char* pszFrontAddress);
	//初始化连接对象
	//Initialize connection object
	int InitConn(const char* pszLicFile, const char* pszSafeLevel = "", const char* pszPwd = "", const char* pszSslFile = "", const char* pszSslPwd = "");

	//331100 登入
	//331100 login
	int ReqFunction331100(const char* pszFundAccount, const char* pszPassword, char cEntrustWay);

	//620001-33100期货单腿行情
	int SubFunction33100(int nIssueType);

	//维护心跳
	void OnHeartbeat(IBizMessage* pMsg);

	//331100 登录应答
	void OnResponse_331100(IF2UnPacker *pUnPacker);
private:
	//T2配置变量
	//T2 config variable
	CConfigInterface*				m_pConfig;
	//T2连接变量
	//T2connections variable
	CConnectionInterface*			m_pConnection;

	//前置地址
	//Front address
	char							m_szFrontAddress[64];
	//T2有名连接的连接名
	//name of T2 named connection
	char							m_szConnName[32];

	//资金账号
	//Fundaccount
	char							m_szFundAccount[32];
	//交易密码
	//Password
	char							m_szPassword[32]; 
	//委托方式
	//Entrust way
	char							m_cEntrustWay;

	//UserToken（登录331100应答获取）
	char							m_szUserToken[64];
	//营业部号（登录331100应答获取）
	int								m_nBranchNo;
	//客户编号（登录331100应答获取）
	char							m_szClientId[18];
	//操作分支机构（登录331100应答获取）
	int								m_nOpBranchNo;
};


#endif
