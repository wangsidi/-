#include "FutuMd.h"

unsigned long CMd_Impl::QueryInterface(const char *iid, IKnown **ppv)
{
	return 0;
}

unsigned long CMd_Impl::AddRef()
{
	return 0;
}

unsigned long CMd_Impl::Release()
{
	return 0;
}

void CMd_Impl::OnConnect(CConnectionInterface *m_pConnection)
{
	cerr<<"CMd_Impl::OnConnect"<<endl;
}

void CMd_Impl::OnSafeConnect(CConnectionInterface *m_pConnection)
{
	cerr<<"CMd_Impl::OnSafeConnect"<<endl;
}

void CMd_Impl::OnRegister(CConnectionInterface *m_pConnection)
{
	cerr<<"CMd_Impl::OnRegister"<<endl;
}

void CMd_Impl::OnClose(CConnectionInterface *m_pConnection)
{
	cerr<<"CMd_Impl::OnClose"<<endl;
}

void CMd_Impl::OnSent(CConnectionInterface *m_pConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
	cerr<<"CMd_Impl::Onsent"<<endl;
}

void CMd_Impl::Reserved1(void *a, void *b, void *c, void *d)
{
	cerr<<"CMd_Impl::Reserved1"<<endl;
}

void CMd_Impl::Reserved2(void *a, void *b, void *c, void *d)
{
	cerr<<"CMd_Impl::Reserved2"<<endl;
}

int CMd_Impl::Reserved3()
{
	cerr<<"CMd_Impl::Reserved3"<<endl;
	return 0;
}

void CMd_Impl::Reserved4()
{
	cerr<<"CMd_Impl::Reserved4"<<endl;
}

void CMd_Impl::Reserved5()
{
	cerr<<"CMd_Impl::Reserved5"<<endl;
}

void CMd_Impl::Reserved6()
{
	cerr<<"CMd_Impl::Reserved6"<<endl;
}

void CMd_Impl::Reserved7()
{
	cerr<<"CMd_Impl::Reserved7"<<endl;
}

void CMd_Impl::OnReceivedBiz(CConnectionInterface *m_pConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
	cerr<<"CMd_Impl::OnReceivedBiz"<<endl;
}

void CMd_Impl::OnReceivedBizEx(CConnectionInterface *m_pConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
	cerr<<"CMd_Impl::OnReceivedBizEx"<<endl;
}

void CMd_Impl::OnReceivedBizMsg(CConnectionInterface *m_pConnection, int hSend, IBizMessage* lpMsg)
{
	cerr<<"CTradeCallback::OnReceivedBizMsg"<<endl;

	if (lpMsg != NULL)
	{
		int iLen = 0;
		const void *pBuffer = lpMsg->GetContent(iLen);
		IF2UnPacker *pUnPacker = NewUnPacker((void *)pBuffer,iLen);
		if(pUnPacker != NULL)
			pUnPacker->AddRef();
		int iLen_key = 0;
		const void *pBuffer_key = lpMsg->GetKeyInfo(iLen_key);
		IF2UnPacker *pUnPacker_key = NewUnPacker((void *)pBuffer_key,iLen_key);
		if(pUnPacker_key != NULL)
			pUnPacker_key->AddRef();

		//��ô�����Ϣ
		CHSRspInfoField RspInfo;
		DealErrorInfo(lpMsg->GetReturnCode(), lpMsg->GetErrorNo(), lpMsg->GetErrorInfo(), pUnPacker, &RspInfo);

		//��ӡӦ���
		//Print Reply packet
		ShowPacket(pUnPacker);

		switch(lpMsg->GetFunction())
		{
		case MSGCENTER_FUNC_HEART:
			//��������ά��
			//Subscribe heartbeat maintenance
			OnHeartbeat(lpMsg);
			break;
		case MSGCENTER_FUNC_REG:
			//���յ����ĵķ��ذ�
			ShowPacket(pUnPacker_key);
			break;
		case MSGCENTER_FUNC_SENDED:
			//���յ����ư�
			if (pUnPacker_key){
				cerr<<"�յ��ڻ���������"<<endl;
				string futuqh = pUnPacker->GetStr("QH");
				cerr<<replace_all(futuqh, "\001", " | ")<<endl;
			}
			break;
		case 331100:
			//���յ��ͻ���¼�ӿڵķ��ذ����
			OnResponse_331100(pUnPacker);
			break;
		default:
			break;
		}

		if(pUnPacker != NULL)
			pUnPacker->Release();
		if(pUnPacker_key != NULL)
			pUnPacker_key->Release();
	}
}

//���캯��
//constructed function
CMd_Impl::CMd_Impl()
{
	m_pConfig = NULL;
	m_pConnection = NULL;

	hs_strncpy(m_szConnName, "FutuDemo_Md", sizeof(m_szConnName));

	m_pConfig = NewConfig();
	m_pConfig->AddRef();
}

//��������
//Destructor
CMd_Impl::~CMd_Impl()
{
	m_pConnection->Release();
	m_pConfig->Release();
}

void CMd_Impl::RegisterFront(const char *pszFrontAddress)
{
	//��ֵǰ�õ�ַ
	//Assignment pre address
	hs_strncpy(m_szFrontAddress, pszFrontAddress, sizeof(m_szFrontAddress));
}

int CMd_Impl::InitConn(const char* pszLicFile, const char* pszSafeLevel, const char* pszPwd, const char* pszSslFile, const char* pszSslPwd)
{
	//�������Ӷ���
	//Configure connection objects
	m_pConfig->SetString("t2sdk", "servers", m_szFrontAddress);
	m_pConfig->SetString("t2sdk", "license_file", pszLicFile);
	m_pConfig->SetString("t2sdk", "login_name", m_szConnName);

	m_pConfig->SetInt("t2sdk", "init_recv_buf_size", 512);
	m_pConfig->SetInt("t2sdk", "init_send_buf_size", 512);
	m_pConfig->SetInt("t2sdk", "send_queue_size", 10000);
	m_pConfig->SetInt("t2sdk", "heartbeat_time", 30);

	//ͨѶ�������ã�������NewConnection֮ǰ
	//encryption should set in front of NewConnection,if the T2 route set safe_level = pwd
	//�������ǰ��t2ͨ��������safe_level = pwd
	if(pszSafeLevel == "pwd")
	{
		m_pConfig->SetString("safe", "safe_level", "pwd");
		//ͨ������
		//Communication password
		m_pConfig->SetString("safe", "comm_pwd", pszPwd); 
	}

	//�������ǰ��t2ͨ��������safe_level = ssl
	//if the T2 route set safe_level = pwd
	if(strcmp(pszSslFile, "") != 0)
	{
		m_pConfig->SetString("safe", "safe_level", "ssl");
		m_pConfig->SetString("safe", "cert_file", pszSslFile); //Cert�ļ�(file��
		m_pConfig->SetString("safe", "cert_pwd", pszSslPwd); //Cert����(password)
	}

	int nRet = 0;
	if(m_pConnection != NULL)
	{
		m_pConnection->Release();
		m_pConnection = NULL;
	}
	m_pConnection = NewConnection(m_pConfig);
	m_pConnection->AddRef();

	cout<<"-------------Connected successfully��----------"<<endl;
	const char* pszServer = m_pConfig->GetString("t2sdk", "servers", "");
	const char* pszLogin = m_pConfig->GetString("t2sdk", "login_name", "");
	cerr<<"������AR��ַ�ǣ�"<<pszServer<<endl;
	cerr<<"�����������ǣ�"<<pszLogin<<endl;
	cerr<<"------------------Well Done!-------------------"<<endl;

	m_pConnection->AddRef();
	if (0 != (nRet = m_pConnection->Create2BizMsg(this)))
	{
		cerr<<"��ʼ��ʧ��.nRet="<<nRet<<" msg:"<<m_pConnection->GetErrorMsg(nRet)<<endl; 
		return -1;
	}
	if (0 != (nRet = m_pConnection->Connect(5000)))
	{
		cerr<<"����.nRet="<<nRet<<" msg:"<<m_pConnection->GetErrorMsg(nRet)<<endl; 
		return -1;
	}

	return nRet;
}

//331100�ڻ��ͻ���½
//331100 Futures customer login
int CMd_Impl::ReqFunction331100(const char* pszFundAccount, const char* pszPassword, char cEntrustWay)
{
	hs_strncpy(m_szFundAccount, pszFundAccount, sizeof(m_szFundAccount));
	hs_strncpy(m_szPassword, pszPassword, sizeof(m_szPassword));
	m_cEntrustWay = cEntrustWay;

	//����ҵ�����
	IF2Packer *pPacker=NewPacker(2);
	pPacker->AddRef();
	pPacker->BeginPack();

	///�����ֶ���
	//Add field name
	pPacker->AddField("op_branch_no", 'I', 5);                 //������֧����
	pPacker->AddField("op_entrust_way", 'C', 1);               //ί�з�ʽ 
	pPacker->AddField("op_station", 'S', 255);                 //վ���ַ
	pPacker->AddField("branch_no", 'I', 5);                    //��֧����
	pPacker->AddField("input_content", 'C');                   //�ͻ���־���
	pPacker->AddField("account_content", 'S', 30);             //��������
	pPacker->AddField("content_type", 'S', 6);                 //���кš��г����
	pPacker->AddField("password", 'S', 10);                    //����
	pPacker->AddField("password_type", 'C');                   //��������
	///�����Ӧ���ֶ�ֵ
	//Add corresponding field value
	pPacker->AddInt(m_nOpBranchNo);						
	pPacker->AddChar(m_cEntrustWay);				
	pPacker->AddStr("ip;mac");				
	pPacker->AddInt(m_nBranchNo);			
	pPacker->AddChar('1');					
	pPacker->AddStr(m_szFundAccount);			
	pPacker->AddStr("0");	
	pPacker->AddStr(m_szPassword);			
	pPacker->AddChar('2');
	///�������
	//end packing
	pPacker->EndPack();

	IBizMessage* pBizMessage=NewBizMessage();
	pBizMessage->AddRef();
	//���ð�ͷ
	//set packet header
	pBizMessage->SetFunction(331100);
	pBizMessage->SetPacketType(REQUEST_PACKET);
	//���ð�����
	//set package
	pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
	//�첽����
	//send Asynchronously
	int nRet = m_pConnection->SendBizMsg(pBizMessage, 1);

	//�ͷ���Դ
	//Release resources
	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	pBizMessage->Release();

	return nRet;
}

int CMd_Impl::SubFunction33100(int nIssueType)
{
	//����ҵ�����
	IF2Packer *pPacker = NewPacker(2);
	pPacker->AddRef();
	pPacker->BeginPack();
	//�����ֶ���
	pPacker->AddField("branch_no", 'I', 5);
	pPacker->AddField("fund_account", 'S', 18);
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("client_id", 'S', 18);
	pPacker->AddField("password", 'S', 10);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("issue_type", 'I', 8);
	pPacker->AddField("futu_code", 'S', 30);

	//�����Ӧ���ֶ�ֵ
	pPacker->AddInt(m_nOpBranchNo);						
	pPacker->AddChar(m_cEntrustWay);					
	pPacker->AddStr("ip;mac");				
	pPacker->AddInt(m_nBranchNo);				
	pPacker->AddStr(m_szClientId);			 
	pPacker->AddStr(m_szFundAccount);			
	pPacker->AddStr(m_szPassword);									
	pPacker->AddStr(m_szUserToken);			
	pPacker->AddInt(nIssueType);
	pPacker->AddStr("IF1708");
	///�������
	pPacker->EndPack();

	IBizMessage* pBizMessage = NewBizMessage();
	pBizMessage->AddRef();
	//���ð�ͷ
	pBizMessage->SetFunction(MSGCENTER_FUNC_REG);
	pBizMessage->SetPacketType(REQUEST_PACKET);
	pBizMessage->SetIssueType(nIssueType);
	//��������
	pBizMessage->SetKeyInfo(pPacker->GetPackBuf(), pPacker->GetPackLen());
	//�첽����
	//send Asynchronously
	int nRet = m_pConnection->SendBizMsg(pBizMessage, 1);

	//�ͷ���Դ
	//Release resources
	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	pBizMessage->Release();

	return nRet;
}

////ά������
void CMd_Impl::OnHeartbeat(IBizMessage* pMsg)
{
	//��������ά��
	if (pMsg->GetPacketType() == REQUEST_PACKET)
	{
		pMsg->ChangeReq2AnsMessage();
		if(NULL != m_pConnection)
			m_pConnection->SendBizMsg(pMsg, 1);
	}
}

//��½���ȡUsertoken��BranchNo�� 
void CMd_Impl::OnResponse_331100(IF2UnPacker *pUnPacker)
{   
	if(pUnPacker->GetStr("client_id") != NULL)
		hs_strncpy(m_szClientId, pUnPacker->GetStr("client_id"), sizeof(m_szClientId));
	if(pUnPacker->GetStr("user_token") != NULL)
		hs_strncpy(m_szUserToken, pUnPacker->GetStr("user_token"), sizeof(m_szUserToken));
	m_nBranchNo = pUnPacker->GetInt("branch_no");
	m_nOpBranchNo = pUnPacker->GetInt("op_branch_no");
}
