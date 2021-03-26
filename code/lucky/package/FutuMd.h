#ifndef FUTU_MD_H
#define FUTU_MD_H

#include "t2sdk_interface.h"
#include "Func.h"

//����ʵ����
//quote realization class
class CMd_Impl : public CCallbackInterface
{
public:
	// ��ΪCCallbackInterface�����մ��������IKnown��������Ҫʵ��һ����3������
	unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
	unsigned long  FUNCTION_CALL_MODE AddRef();
	unsigned long  FUNCTION_CALL_MODE Release();

	// �����¼�����ʱ�Ļص�������ʵ��ʹ��ʱ���Ը�����Ҫ��ѡ��ʵ�֣����ڲ���Ҫ���¼��ص���������ֱ��return
	// Reserved?Ϊ����������Ϊ�Ժ���չ��׼����ʵ��ʱ��ֱ��return��return 0��
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
	//���캯��
	//constructed function
	CMd_Impl();
	//��������
	//Destructor
	~CMd_Impl();

	//ע��ǰ�û������ַ
	//Register front-end machine network address
	void RegisterFront(const char* pszFrontAddress);
	//��ʼ�����Ӷ���
	//Initialize connection object
	int InitConn(const char* pszLicFile, const char* pszSafeLevel = "", const char* pszPwd = "", const char* pszSslFile = "", const char* pszSslPwd = "");

	//331100 ����
	//331100 login
	int ReqFunction331100(const char* pszFundAccount, const char* pszPassword, char cEntrustWay);

	//620001-33100�ڻ���������
	int SubFunction33100(int nIssueType);

	//ά������
	void OnHeartbeat(IBizMessage* pMsg);

	//331100 ��¼Ӧ��
	void OnResponse_331100(IF2UnPacker *pUnPacker);
private:
	//T2���ñ���
	//T2 config variable
	CConfigInterface*				m_pConfig;
	//T2���ӱ���
	//T2connections variable
	CConnectionInterface*			m_pConnection;

	//ǰ�õ�ַ
	//Front address
	char							m_szFrontAddress[64];
	//T2�������ӵ�������
	//name of T2 named connection
	char							m_szConnName[32];

	//�ʽ��˺�
	//Fundaccount
	char							m_szFundAccount[32];
	//��������
	//Password
	char							m_szPassword[32]; 
	//ί�з�ʽ
	//Entrust way
	char							m_cEntrustWay;

	//UserToken����¼331100Ӧ���ȡ��
	char							m_szUserToken[64];
	//Ӫҵ���ţ���¼331100Ӧ���ȡ��
	int								m_nBranchNo;
	//�ͻ���ţ���¼331100Ӧ���ȡ��
	char							m_szClientId[18];
	//������֧��������¼331100Ӧ���ȡ��
	int								m_nOpBranchNo;
};


#endif
