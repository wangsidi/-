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

		//获得错误信息
		CHSRspInfoField RspInfo;
		DealErrorInfo(lpMsg->GetReturnCode(), lpMsg->GetErrorNo(), lpMsg->GetErrorInfo(), pUnPacker, &RspInfo);

		//打印应答包
		//Print Reply packet
		ShowPacket(pUnPacker);

		switch(lpMsg->GetFunction())
		{
		case MSGCENTER_FUNC_HEART:
			//订阅心跳维护
			//Subscribe heartbeat maintenance
			OnHeartbeat(lpMsg);
			break;
		case MSGCENTER_FUNC_REG:
			//接收到订阅的返回包
			ShowPacket(pUnPacker_key);
			break;
		case MSGCENTER_FUNC_SENDED:
			//接收到主推包
			if (pUnPacker_key){
				cerr<<"收到期货单腿行情"<<endl;
				string futuqh = pUnPacker->GetStr("QH");
				cerr<<replace_all(futuqh, "\001", " | ")<<endl;
			}
			break;
		case 331100:
			//接收到客户登录接口的返回包解包
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

//构造函数
//constructed function
CMd_Impl::CMd_Impl()
{
	m_pConfig = NULL;
	m_pConnection = NULL;

	hs_strncpy(m_szConnName, "FutuDemo_Md", sizeof(m_szConnName));

	m_pConfig = NewConfig();
	m_pConfig->AddRef();
}

//析构函数
//Destructor
CMd_Impl::~CMd_Impl()
{
	m_pConnection->Release();
	m_pConfig->Release();
}

void CMd_Impl::RegisterFront(const char *pszFrontAddress)
{
	//赋值前置地址
	//Assignment pre address
	hs_strncpy(m_szFrontAddress, pszFrontAddress, sizeof(m_szFrontAddress));
}

int CMd_Impl::InitConn(const char* pszLicFile, const char* pszSafeLevel, const char* pszPwd, const char* pszSslFile, const char* pszSslPwd)
{
	//配置连接对象
	//Configure connection objects
	m_pConfig->SetString("t2sdk", "servers", m_szFrontAddress);
	m_pConfig->SetString("t2sdk", "license_file", pszLicFile);
	m_pConfig->SetString("t2sdk", "login_name", m_szConnName);

	m_pConfig->SetInt("t2sdk", "init_recv_buf_size", 512);
	m_pConfig->SetInt("t2sdk", "init_send_buf_size", 512);
	m_pConfig->SetInt("t2sdk", "send_queue_size", 10000);
	m_pConfig->SetInt("t2sdk", "heartbeat_time", 30);

	//通讯加密设置，必须在NewConnection之前
	//encryption should set in front of NewConnection,if the T2 route set safe_level = pwd
	//如果接入前置t2通道设置了safe_level = pwd
	if(pszSafeLevel == "pwd")
	{
		m_pConfig->SetString("safe", "safe_level", "pwd");
		//通信密码
		//Communication password
		m_pConfig->SetString("safe", "comm_pwd", pszPwd); 
	}

	//如果接入前置t2通道设置了safe_level = ssl
	//if the T2 route set safe_level = pwd
	if(strcmp(pszSslFile, "") != 0)
	{
		m_pConfig->SetString("safe", "safe_level", "ssl");
		m_pConfig->SetString("safe", "cert_file", pszSslFile); //Cert文件(file）
		m_pConfig->SetString("safe", "cert_pwd", pszSslPwd); //Cert密码(password)
	}

	int nRet = 0;
	if(m_pConnection != NULL)
	{
		m_pConnection->Release();
		m_pConnection = NULL;
	}
	m_pConnection = NewConnection(m_pConfig);
	m_pConnection->AddRef();

	cout<<"-------------Connected successfully！----------"<<endl;
	const char* pszServer = m_pConfig->GetString("t2sdk", "servers", "");
	const char* pszLogin = m_pConfig->GetString("t2sdk", "login_name", "");
	cerr<<"您连接AR地址是："<<pszServer<<endl;
	cerr<<"您连接名字是："<<pszLogin<<endl;
	cerr<<"------------------Well Done!-------------------"<<endl;

	m_pConnection->AddRef();
	if (0 != (nRet = m_pConnection->Create2BizMsg(this)))
	{
		cerr<<"初始化失败.nRet="<<nRet<<" msg:"<<m_pConnection->GetErrorMsg(nRet)<<endl; 
		return -1;
	}
	if (0 != (nRet = m_pConnection->Connect(5000)))
	{
		cerr<<"连接.nRet="<<nRet<<" msg:"<<m_pConnection->GetErrorMsg(nRet)<<endl; 
		return -1;
	}

	return nRet;
}

//331100期货客户登陆
//331100 Futures customer login
int CMd_Impl::ReqFunction331100(const char* pszFundAccount, const char* pszPassword, char cEntrustWay)
{
	hs_strncpy(m_szFundAccount, pszFundAccount, sizeof(m_szFundAccount));
	hs_strncpy(m_szPassword, pszPassword, sizeof(m_szPassword));
	m_cEntrustWay = cEntrustWay;

	//创建业务包体
	IF2Packer *pPacker=NewPacker(2);
	pPacker->AddRef();
	pPacker->BeginPack();

	///加入字段名
	//Add field name
	pPacker->AddField("op_branch_no", 'I', 5);                 //操作分支机构
	pPacker->AddField("op_entrust_way", 'C', 1);               //委托方式 
	pPacker->AddField("op_station", 'S', 255);                 //站点地址
	pPacker->AddField("branch_no", 'I', 5);                    //分支机构
	pPacker->AddField("input_content", 'C');                   //客户标志类别
	pPacker->AddField("account_content", 'S', 30);             //输入内容
	pPacker->AddField("content_type", 'S', 6);                 //银行号、市场类别
	pPacker->AddField("password", 'S', 10);                    //密码
	pPacker->AddField("password_type", 'C');                   //密码类型
	///加入对应的字段值
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
	///结束打包
	//end packing
	pPacker->EndPack();

	IBizMessage* pBizMessage=NewBizMessage();
	pBizMessage->AddRef();
	//设置包头
	//set packet header
	pBizMessage->SetFunction(331100);
	pBizMessage->SetPacketType(REQUEST_PACKET);
	//设置包内容
	//set package
	pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
	//异步发送
	//send Asynchronously
	int nRet = m_pConnection->SendBizMsg(pBizMessage, 1);

	//释放资源
	//Release resources
	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	pBizMessage->Release();

	return nRet;
}

int CMd_Impl::SubFunction33100(int nIssueType)
{
	//创建业务包体
	IF2Packer *pPacker = NewPacker(2);
	pPacker->AddRef();
	pPacker->BeginPack();
	//加入字段名
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

	//加入对应的字段值
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
	///结束打包
	pPacker->EndPack();

	IBizMessage* pBizMessage = NewBizMessage();
	pBizMessage->AddRef();
	//设置包头
	pBizMessage->SetFunction(MSGCENTER_FUNC_REG);
	pBizMessage->SetPacketType(REQUEST_PACKET);
	pBizMessage->SetIssueType(nIssueType);
	//设置内容
	pBizMessage->SetKeyInfo(pPacker->GetPackBuf(), pPacker->GetPackLen());
	//异步发送
	//send Asynchronously
	int nRet = m_pConnection->SendBizMsg(pBizMessage, 1);

	//释放资源
	//Release resources
	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	pBizMessage->Release();

	return nRet;
}

////维护心跳
void CMd_Impl::OnHeartbeat(IBizMessage* pMsg)
{
	//订阅心跳维护
	if (pMsg->GetPacketType() == REQUEST_PACKET)
	{
		pMsg->ChangeReq2AnsMessage();
		if(NULL != m_pConnection)
			m_pConnection->SendBizMsg(pMsg, 1);
	}
}

//登陆后获取Usertoken，BranchNo等 
void CMd_Impl::OnResponse_331100(IF2UnPacker *pUnPacker)
{   
	if(pUnPacker->GetStr("client_id") != NULL)
		hs_strncpy(m_szClientId, pUnPacker->GetStr("client_id"), sizeof(m_szClientId));
	if(pUnPacker->GetStr("user_token") != NULL)
		hs_strncpy(m_szUserToken, pUnPacker->GetStr("user_token"), sizeof(m_szUserToken));
	m_nBranchNo = pUnPacker->GetInt("branch_no");
	m_nOpBranchNo = pUnPacker->GetInt("op_branch_no");
}
