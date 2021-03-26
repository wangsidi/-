/*******************************************************************************************************/
//
//   1、此C++ Demo使用异步发送接收模式；
//    2、Demo连接的是恒生仿真测试环境，如要对接券商环境，需要修改t2sdk.ini文件中的IP和许可证文件等；
//    3、不同环境的验证信息可能不一样，更换连接环境时，包头字段设置需要确认；
//    4、接口字段说明请参考接口文档"恒生统一接入平台_周边接口规范(期货，期权).xls"；
//    5、T2函数技术说明参考开发文档“T2SDK 外部版开发指南.docx"；
//    6、如有UFX接口技术疑问可联系大金融讨论组（261969915）；
//    7、UFX技术支持网站 https://ufx.hscloud.cn/；
//    8、demo仅供参考。
//
/*******************************************************************************************************/
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QTextCodec>

#include <mutex>
#include <sstream>

#include "public/Basic.h"
#include "FutuTrade.h"
//插入引用(取最大引用)
int INDEX_LIST = 1;
//缺省数量
int LOSS_NUM = 0;
//得到数量
static int GET_NUM = 0;
//规定大小
const size_t GET_SIZE = 30;
//会话编号
static int SESSION_NUM = 0;
//构造函数
CTrade_Impl::CTrade_Impl()
{
    //链接对象创建及配置
    pConfig = NULL;
    pConnection = NULL;
    //初始化属性
    memset(szFundAccount, '\0', sizeof(szFundAccount));
    memset(szPassword, '\0', sizeof(szPassword));
    hs_strncpy(szAppSessionID, "", sizeof(szAppSessionID));
    hs_strncpy(szUserToken, "", sizeof(szUserToken));
    nBranchNo = 0;
    hs_strncpy(szClientId, "", sizeof(szClientId));
    nOpBranchNo = 0;
    //初始化配置
    pConfig = NewConfig();
    pConfig->AddRef();
}
//析构函数
CTrade_Impl::~CTrade_Impl()
{
    if(pConnection)
        pConnection->Release();
    if(pConfig)
        pConfig->Release();
}

unsigned long CTrade_Impl::QueryInterface(const char *iid, IKnown **ppv){
    return 0;
}

unsigned long CTrade_Impl::AddRef(){
    return 0;
}

unsigned long CTrade_Impl::Release(){
    return 0;
}

void CTrade_Impl::OnConnect(CConnectionInterface *pConnection){
    cerr<<"CTrade_Impl::OnConnect"<<endl;
}

void CTrade_Impl::OnSafeConnect(CConnectionInterface *pConnection){
    cerr<<"CTrade_Impl::OnSafeConnect"<<endl;
}

void CTrade_Impl::OnRegister(CConnectionInterface *pConnection){
    cerr<<"CTrade_Impl::OnRegister"<<endl;
}
void CTrade_Impl::OnClose(CConnectionInterface *pConnection){
    emit connectClose();
}

void CTrade_Impl::OnSent(CConnectionInterface *pConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData){
    cerr<<"CTrade_Impl::Onsent"<<endl;
}

void CTrade_Impl::Reserved1(void *a, void *b, void *c, void *d){
    cerr<<"CTrade_Impl::Reserved1"<<endl;
}


void CTrade_Impl::Reserved2(void *a, void *b, void *c, void *d){
    cerr<<"CTrade_Impl::Reserved2"<<endl;
}

int  CTrade_Impl::Reserved3()
{
    cerr<<"CTrade_Impl::Reserved3"<<endl;
    return 0;
}

void CTrade_Impl::Reserved4()
{
    cerr<<"CTrade_Impl::Reserved4"<<endl;
}

void CTrade_Impl::Reserved5()
{
    cerr<<"CTrade_Impl::Reserved5"<<endl;
}

void CTrade_Impl::Reserved6()
{
    cerr<<"CTrade_Impl::Reserved6"<<endl;
}

void CTrade_Impl::Reserved7()
{
    cerr<<"CTrade_Impl::Reserved7"<<endl;
}

void CTrade_Impl::OnReceivedBiz(CConnectionInterface *pConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
    cerr<<"CTrade_Impl::OnReceivedBiz"<<endl;
}
void CTrade_Impl::OnReceivedBizEx(CConnectionInterface *pConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult){
    cerr<<"CTrade_Impl::OnReceivedBizEx"<<endl;
}
void CTrade_Impl::OnReceivedBizMsg(CConnectionInterface *pConnection, int hSend, IBizMessage* lpMsg)
{

    if(lpMsg->GetFunction() != 620000)
        qDebug()<<"CTrade_Impl::OnReceivedBizMsg: " << lpMsg->GetFunction() <<endl;
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


        switch(lpMsg->GetFunction())
        {
        case 331480:
            //338410响应处理
            DealTradeTransmissionApprove(pUnPacker);
            break;
        case 331481:
            //338411响应处理
            DealTradeClientApprove(pUnPacker);
            break;
        case 331482:
            //338412响应处理
            DealTradeClientAppear(pUnPacker);
            break;
        case 331100:
            //331100响应处理
            DealTradeLogin(pUnPacker);
            break;
        case 338202:
            //338202响应处理
            DealTradeSendEntrust(hSend, pUnPacker);
            break;
        case 338217:
            //338217响应处理
            DealTradeEntrustBackout(pUnPacker);
            break;
        case 338300:
            //338300响应处理
            DealTradeFundSearch(pUnPacker);
            break;
        case 338301:
            //338301响应处理
            DealTradeTodayEntrustSearch(pUnPacker);
            break;
        case 338302:
            //338302响应处理
            DealTradeTodayBargainSearch(pUnPacker);
            break;
        case 338303:
            //338303响应处理
            DealTradePositionSearch(pUnPacker);
            break;
         case 620000:
            //620000响应处理
            DealRepFunction620000(lpMsg);
            break;
        case 620001:
            //620001响应处理
            DealRepFunction620001(pUnPacker);
            break;
        case 620003:
            //620003响应处理
            DealRepFunction620003(pUnPacker);
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
//初始化连接
int CTrade_Impl::InitConn(QString infoPath)
{
    //配置连接对象
    pConfig->Load(infoPath.toUtf8());

    int nRet = 0;
    if(pConnection != NULL)
    {
        pConnection->Release();
        pConnection = NULL;
    }
    pConnection = NewConnection(pConfig);
    pConnection->AddRef();

    pConnection->AddRef();
    //初始化失败
    if (0 != (nRet = pConnection->Create2BizMsg(this)))
    {
        cerr<<"nRet="<<nRet<<" msg:"<<pConnection->GetErrorMsg(nRet)<<endl;
        return -1;
    }
    //连接失败
    if (0 != (nRet = pConnection->Connect(5000)))
    {
        cerr<<"nRet="<<nRet<<" msg:"<<pConnection->GetErrorMsg(nRet)<<endl;
        return -1;
    }

    return nRet;
}

//331480 穿透式认证请求
int CTrade_Impl::cTradeTransmissionApprove(const TradeAccount* u)
{
    //获取内存访问权限
    user = u;
    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("fund_account",'S',18);                                          //资产账户
    pPacker->AddField("password",'S',13);                                              //密码
    pPacker->AddField("password_type",'C');                                            //密码类别
    //加入对应的字段值
    pPacker->AddStr(user->FundAccount);
    pPacker->AddStr(user->Password);
    pPacker->AddChar('2');
    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage();
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(331480);
    pBizMessage->SetPacketType(REQUEST_PACKET);
    //设置包内容
    pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //异步发送
    int nRet = pConnection->SendBizMsg(pBizMessage, 1);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    return nRet;
}

//331481 客户端认证请求
int CTrade_Impl::cTradeClientApprove()
{
    int iOutLen = 0;
    char szAuthCode[64];
    char encrypted_session_id[256] = { 0 };
    hs_strncpy(szAuthCode, user->AuthCode, sizeof(szAuthCode));
    int inLen = strlen(szAppSessionID);
    int nRet = hundsun_encrypt(szAppSessionID, inLen, encrypted_session_id, &iOutLen, szAuthCode);
    if (nRet != 0)
    {
        emit appendMsg("会话加密失败");
        return nRet;
    }

    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("fund_account",'S',18);                                          //资产账户
    pPacker->AddField("app_id",'S',255);                                               //客户端id名称
    pPacker->AddField("session_id",'S',64);                                            //会话号
    pPacker->AddField("encrypted_session_id",'S',64);                                  //加密后的会话id
    //加入对应的字段值
    pPacker->AddStr(user->FundAccount);
    pPacker->AddStr(user->AppId);
    pPacker->AddStr(szAppSessionID);
    pPacker->AddStr(encrypted_session_id);
    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage();
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(331481);
    pBizMessage->SetPacketType(REQUEST_PACKET);
    //设置包内容
    pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //异步发送
    nRet = pConnection->SendBizMsg(pBizMessage, 1);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    return nRet;
}

//331482 客户端系统信息上报请求
int CTrade_Impl::cTradeClientAppear()
{
    /*采集系统信息
    *@param pszSysInfo:  加密后的采集信息, 缓冲区长度至少492字节
    *@param iSysInfoLen:  加密后的采集信息的长度
    *@param pszSysInfoIntegrity:   采集信息完整度，缓冲区长度至少300字节
    *@param iSysInfoIntegrityLen:   采集信息完整度的长度
    *@return 0：正常1：终端信息采集为空2：终端采集数据加密密钥版本异常3：终端信息数据异常
    */
    int   nSysInfoLen = 0;
    int   nSysInfoIntegrityLen = 0;
    char  szszSysInfo[512] = { 0 };
    char  szSysInfoIntegrity[512] = { 0 };

    int nRet = hundsun_getsysteminfo(szszSysInfo, &nSysInfoLen, szSysInfoIntegrity, &nSysInfoIntegrityLen);
    if (1 == nRet)
        emit appendMsg("终端信息采集为空");
    else if (2 == nRet)
        emit appendMsg("终端采集数据加密密钥版本异常");
    else if (3 == nRet)
        emit appendMsg("终端信息数据异常");
    if (nRet != 0)
    {
        return nRet;
    }

    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("session_id",'S',64);                                            //会话号
    pPacker->AddField("fund_account",'S',18);                                          //资产账户
    pPacker->AddField("app_id",'S',255);                                               //客户端id名称
    pPacker->AddField("app_sys_info",'S',2000);                                        //客户端系统信息
    pPacker->AddField("app_sys_info_integrity",'S',2000);                              //客户端系统信息采集完整度
    pPacker->AddField("app_abnormal_type",'C');                                        //客户端系统信息采集异常标识
    pPacker->AddField("app_login_time",'S',20);                                        //客户端登入时间
    //加入对应的字段值
    pPacker->AddStr(szAppSessionID);
    pPacker->AddStr(user->FundAccount);
    pPacker->AddStr(user->AppId);
    pPacker->AddStr(szszSysInfo);
    pPacker->AddStr(szSysInfoIntegrity);
    pPacker->AddChar((nRet + '0'));
    pPacker->AddStr("");
    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage();
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(331482);
    pBizMessage->SetPacketType(REQUEST_PACKET);
    //设置包内容
    pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //异步发送
    nRet = pConnection->SendBizMsg(pBizMessage, 1);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    return nRet;
}

//331100期货客户登陆
int CTrade_Impl::cTradeLogin()
{
    //拷贝信息
    hs_strncpy(szFundAccount, user->FundAccount, sizeof(szFundAccount));
    hs_strncpy(szPassword, user->Password, sizeof(szPassword));
    cEntrustWay = user->EntrustWay;

    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);                 //操作分支机构
    pPacker->AddField("op_entrust_way", 'C', 1);               //委托方式
    pPacker->AddField("op_station", 'S', 255);                 //站点地址
    pPacker->AddField("branch_no", 'I', 5);                    //分支机构
    pPacker->AddField("input_content", 'C');                   //客户标志类别
    pPacker->AddField("account_content", 'S', 30);             //输入内容
    pPacker->AddField("content_type", 'S', 6);                 //银行号、市场类别
    pPacker->AddField("password", 'S', 10);                    //密码
    pPacker->AddField("password_type", 'C');                   //密码类型
    //加入对应的字段值
    pPacker->AddInt(nOpBranchNo);
    pPacker->AddChar(cEntrustWay);
    pPacker->AddStr("ip;mac");
    pPacker->AddInt(nBranchNo);
    pPacker->AddChar('1');
    pPacker->AddStr(szFundAccount);
    pPacker->AddStr("0");
    pPacker->AddStr(szPassword);
    pPacker->AddChar('2');

    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage(); // 创建消息体
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(331100); //func ID
    pBizMessage->SetPacketType(REQUEST_PACKET); //请求包
    //设置包内容
    pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //异步发送
    int nRet = pConnection->SendBizMsg(pBizMessage, 1);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    return nRet;
}

//338202期货委托确认
int CTrade_Impl::cTradeSendEntrust(const char* pszExchangeType, const char* pszFutuCode, char cEntrustBs, char cFuturesDirection, int nEntrustAmount, double dFutuEntrustPrice)
{
    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();

    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);                  //操作分支机构
    pPacker->AddField("op_entrust_way", 'C', 1);                //委托方式
    pPacker->AddField("op_station", 'S', 255);                  //站点地址
    pPacker->AddField("branch_no", 'I', 5);                     //分支机构
    pPacker->AddField("client_id", 'S', 18);                    //客户编号
    pPacker->AddField("fund_account", 'S', 18);                 //资产账户
    pPacker->AddField("password", 'S', 10);	                    //密码
    pPacker->AddField("user_token", 'S', 40);                   //user_token
    pPacker->AddField("futu_exch_type", 'S', 4);                //交易类别
    pPacker->AddField("futures_account", 'S', 12);              //交易编码
    pPacker->AddField("futu_code", 'S', 30);                    //合约代码
    pPacker->AddField("entrust_bs", 'C', 1);                    //买卖方向
    pPacker->AddField("futures_direction", 'C', 1);             //开平仓方向
    pPacker->AddField("hedge_type", 'C', 1);                    //投机/套保类型
    pPacker->AddField("entrust_amount", 'I');                   //委托数量
    pPacker->AddField("futu_entrust_price", 'F', 12, 6);        //委托价格
    pPacker->AddField("entrust_prop", 'S', 3);                  //委托属性
    pPacker->AddField("entrust_occasion", 'S', 32);	            //委托场景
    //加入对应的字段值
    pPacker->AddInt(nOpBranchNo);
    pPacker->AddChar(cEntrustWay);
    pPacker->AddStr("ip;mac");
    pPacker->AddInt(nBranchNo);
    pPacker->AddStr(szClientId);
    pPacker->AddStr(szFundAccount);
    pPacker->AddStr(szPassword);
    pPacker->AddStr(szUserToken);
    pPacker->AddStr(pszExchangeType);
    pPacker->AddStr("");
    pPacker->AddStr(pszFutuCode);
    pPacker->AddChar(cEntrustBs);
    pPacker->AddChar(cFuturesDirection);
    pPacker->AddChar('0');
    pPacker->AddInt(nEntrustAmount);
    pPacker->AddDouble(dFutuEntrustPrice);
    pPacker->AddStr("F0");
    pPacker->AddStr("");

    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage();
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(338202);
    pBizMessage->SetPacketType(REQUEST_PACKET);
    //设置包内容
    pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //创建新委托
    EntrustInfo* newEntrust = createNewEntrust(pszExchangeType, pszFutuCode, cEntrustBs, cFuturesDirection, nEntrustAmount, dFutuEntrustPrice);
    //发送
    //异步发送
    int nRet = pConnection->SendBizMsg(pBizMessage, 1);

    newEntrust->ret = nRet;
    newEntrust->sessionNum = SESSION_NUM;
    emit getNewEntrustMsg(newEntrust);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    //结束发送下单
    return nRet;
}

//338217期货撤单委托
int CTrade_Impl::cTradeEntrustBackout(int nEntrustNo)
{
    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();

    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);                    //操作分支机构
    pPacker->AddField("op_entrust_way", 'C', 1);                  //委托方式
    pPacker->AddField("op_station", 'S', 255);                    //站点地址
    pPacker->AddField("branch_no", 'I', 5);			              //分支机构
    pPacker->AddField("client_id", 'S', 18);			          //客户编号
    pPacker->AddField("fund_account", 'S', 18);		              //资产账户
    pPacker->AddField("password", 'S', 10);						  //密码
    pPacker->AddField("user_token", 'S', 40);                     //用户口令
    pPacker->AddField("futu_exch_type", 'S', 4);                  //交易类别
    pPacker->AddField("entrust_no", 'I', 8);                      //委托编号
    pPacker->AddField("entrust_reference", 'S', 32);	          //委托引用-非必输字段

    //加入对应的字段值
    pPacker->AddInt(nOpBranchNo);
    pPacker->AddChar(cEntrustWay);
    pPacker->AddStr("ip;mac");
    pPacker->AddInt(nBranchNo);
    pPacker->AddStr(szClientId);
    pPacker->AddStr(szFundAccount);
    pPacker->AddStr(szPassword);
    pPacker->AddStr(szUserToken);
    pPacker->AddStr("F1");
    pPacker->AddInt(nEntrustNo);
    pPacker->AddStr("0");

    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage();
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(338217);
    pBizMessage->SetPacketType(REQUEST_PACKET);
    //设置包的内容
    pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //异步发送
    int nRet = pConnection->SendBizMsg(pBizMessage, 1);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    return nRet;
}
//338300资金查询
int CTrade_Impl::cTradeFundSearch()
{
    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();

    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5);
    pPacker->AddField("client_id", 'S', 18);
    pPacker->AddField("fund_account", 'S', 18);
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("user_token", 'S', 40);
    //加入对应的字段值
    pPacker->AddInt(nOpBranchNo);
    pPacker->AddChar(cEntrustWay);
    pPacker->AddStr("ip;mac");
    pPacker->AddInt(nBranchNo);
    pPacker->AddStr(szClientId);
    pPacker->AddStr(szFundAccount);
    pPacker->AddStr(szPassword);
    pPacker->AddStr(szUserToken);

    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage();
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(338300);
    pBizMessage->SetPacketType(REQUEST_PACKET);
    //设置包的内容
    pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //异步发送
    int nRet = pConnection->SendBizMsg(pBizMessage, 1);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    return nRet;
}

//338301期货委托查询
int CTrade_Impl::cTradeTodayEntrustSearch(const char* pszPositionStr, int nRequestNum)
{
    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();

    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5);
    pPacker->AddField("client_id", 'S', 18);
    pPacker->AddField("fund_account", 'S', 18);
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("user_token", 'S', 40);
    pPacker->AddField("position_str",'S'); //定位串
    pPacker->AddField("request_num",'I', 10); //请求行数
    //加入对应的字段值
    pPacker->AddInt(nOpBranchNo);
    pPacker->AddChar(cEntrustWay);
    pPacker->AddStr("ip;mac");
    pPacker->AddInt(nBranchNo);
    pPacker->AddStr(szClientId);
    pPacker->AddStr(szFundAccount);
    pPacker->AddStr(szPassword);
    pPacker->AddStr(szUserToken);
    pPacker->AddStr(pszPositionStr);
    pPacker->AddInt(nRequestNum);

    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage();
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(338301);
    pBizMessage->SetPacketType(REQUEST_PACKET);
    //设置包的内容
    pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //异步发送
    int nRet = pConnection->SendBizMsg(pBizMessage, 1);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    return nRet;
}

//338302期货成交查询
int CTrade_Impl::cTradeTodayBargainSearch(const char* pszPositionStr, int nRequestNum)
{
    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();

    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5);
    pPacker->AddField("client_id", 'S', 18);
    pPacker->AddField("fund_account", 'S', 18);
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("user_token", 'S', 40);
    pPacker->AddField("futu_exch_type", 'S', 4);
    pPacker->AddField("position_str",'S'); //定位串
    pPacker->AddField("request_num",'I', 10); //请求行数
    //加入对应的字段值
    pPacker->AddInt(nOpBranchNo);
    pPacker->AddChar(cEntrustWay);
    pPacker->AddStr("ip;mac");
    pPacker->AddInt(nBranchNo);
    pPacker->AddStr(szClientId);
    pPacker->AddStr(szFundAccount);
    pPacker->AddStr(szPassword);
    pPacker->AddStr(szUserToken);
    pPacker->AddStr("");
    pPacker->AddStr(pszPositionStr);
    pPacker->AddInt(nRequestNum);


    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage();
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(338302);
    pBizMessage->SetPacketType(REQUEST_PACKET);
    //设置包的内容
    pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //异步发送
    int nRet = pConnection->SendBizMsg(pBizMessage, 1);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    return nRet;
}

//338303期货持仓查询
int CTrade_Impl::cTradePositionSearch(const char* pszPositionStr, int nRequestNum)
{
    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();

    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5);
    pPacker->AddField("client_id", 'S', 18);
    pPacker->AddField("fund_account", 'S', 18);
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("user_token", 'S', 40);
    pPacker->AddField("position_str",'S'); //定位串
    pPacker->AddField("request_num",'I', 10); //请求行数
    //加入对应的字段值
    pPacker->AddInt(nOpBranchNo);
    pPacker->AddChar(cEntrustWay);
    pPacker->AddStr("ip;mac");
    pPacker->AddInt(nBranchNo);
    pPacker->AddStr(szClientId);
    pPacker->AddStr(szFundAccount);
    pPacker->AddStr(szPassword);
    pPacker->AddStr(szUserToken);
    pPacker->AddStr(pszPositionStr);
    pPacker->AddInt(nRequestNum);

    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage();
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(338303);
    pBizMessage->SetPacketType(REQUEST_PACKET);
    //设置包的内容
    pBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //异步发送
    int nRet = pConnection->SendBizMsg(pBizMessage, 1);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    return nRet;
}
//620001_33101期权订阅委托成交回报
int CTrade_Impl::sendSubscription(int nIssueType)
{
    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("branch_no", 'I', 5);
    pPacker->AddField("fund_account", 'S');
    pPacker->AddField("issue_type",'I',8);
    //加入对应的字段值
    pPacker->AddInt(nOpBranchNo);
    pPacker->AddStr(szFundAccount);
    pPacker->AddInt(nIssueType);

    //结束打包
    pPacker->EndPack();

    IBizMessage* pBizMessage=NewBizMessage();
    pBizMessage->AddRef();
    //设置包头
    pBizMessage->SetFunction(MSGCENTER_FUNC_REG);
    pBizMessage->SetPacketType(REQUEST_PACKET);
    pBizMessage->SetIssueType(nIssueType);
    //设置内容
    pBizMessage->SetKeyInfo(pPacker->GetPackBuf(),pPacker->GetPackLen());
    //异步发送
    int nRet = pConnection->SendBizMsg(pBizMessage, 1);

    //释放资源
    pPacker->FreeMem(pPacker->GetPackBuf());
    pPacker->Release();
    pBizMessage->Release();

    return nRet;
}
//创建新委托
EntrustInfo *CTrade_Impl::createNewEntrust(const char* pszExchangeType, const char* pszFutuCode, char cEntrustBs, char cFuturesDirection, int nEntrustAmount, double dFutuEntrustPrice)
{
    //创建新委托
    EntrustInfo* newEntrust = new EntrustInfo;
    //复制信息
    strcpy(newEntrust->futuCode, pszFutuCode);
    newEntrust->entrustBs = cEntrustBs;
    newEntrust->hedgeType = '0';
    newEntrust->entrust_no = 0;
    newEntrust->entrustDate = 0;
    strcpy(newEntrust->futuExchType, pszExchangeType);
    newEntrust->entrustAmount = nEntrustAmount;
    newEntrust->entrustStatus = '0';
    newEntrust->futuEntrustPrice = dFutuEntrustPrice;
    newEntrust->futuresDirection = cFuturesDirection;
    newEntrust->totalBusinessAmount = 0;
    //析构已拷贝资源
    delete pszFutuCode;
    delete pszExchangeType;
    //返回
    return newEntrust;
}
//创建新委托
EntrustInfo *CTrade_Impl::createNewEntrust(const int entrustNo, const char* pszExchangeType, const char* pszFutuCode, char cEntrustBs, char cFuturesDirection, char cHedgeType, int nEntrustAmount, double cTotalBusinessAmount, double dFutuEntrustPrice, int cEntrustDate, char cEntrustStatus)
{
    //创建新委托
    EntrustInfo* newEntrust = new EntrustInfo;
    //复制信息
    newEntrust->entrust_no = entrustNo;
    strcpy(newEntrust->futuExchType, pszExchangeType);
    strcpy(newEntrust->futuCode, pszFutuCode);
    newEntrust->entrustBs = cEntrustBs;
    newEntrust->futuresDirection = cFuturesDirection;
    newEntrust->hedgeType = cHedgeType;
    newEntrust->entrustAmount = nEntrustAmount;
    newEntrust->totalBusinessAmount = cTotalBusinessAmount;
    newEntrust->futuEntrustPrice = dFutuEntrustPrice;
    newEntrust->entrustStatus = cEntrustStatus;
    newEntrust->entrustDate = cEntrustDate;
    //析构已拷贝资源
    delete pszFutuCode;
    delete pszExchangeType;
    //返回
    return newEntrust;
}
//331480响应处理
void CTrade_Impl::DealTradeTransmissionApprove(IF2UnPacker *pUnPacker)
{
    //判断是否为空 为空成功
    if(pUnPacker->GetStr("error_no") == 0)
    {
        //获取sessionId
        strcpy(szAppSessionID, pUnPacker->GetStr("session_id"));
        this->cTradeClientApprove();
    }
    else
        emit appendMsg(codec->toUnicode( pUnPacker->GetStr("error_info")));

}
//331481响应处理
void CTrade_Impl::DealTradeClientApprove(IF2UnPacker *pUnPacker)
{
    //判断是否为空 为空成功
    if(pUnPacker->GetStr("error_no") == 0)
        this->cTradeClientAppear();
    else
        emit appendMsg(codec->toUnicode( pUnPacker->GetStr("error_info")));
}
//331482响应处理
void CTrade_Impl::DealTradeClientAppear(IF2UnPacker *pUnPacker)
{
    //判断是否为空 为空成功
    if(pUnPacker->GetStr("error_no") == 0)
        this->cTradeLogin();
    else
        emit appendMsg(codec->toUnicode(pUnPacker->GetStr("error_info")));
}
//331100响应处理
void CTrade_Impl::DealTradeLogin(IF2UnPacker* pUnPacker)
{
    //如果登录成功
    if(pUnPacker->GetStr("error_no") == 0){
        //获取编号
        SESSION_NUM = pUnPacker->GetInt("session_no");
        //用户口令录入
        strcpy(szUserToken, pUnPacker->GetStr("user_token"));
        //营业部号录入
        nBranchNo = pUnPacker->GetInt("branch_no");
        //客户编号录入
        strcpy(szClientId, pUnPacker->GetStr("client_id"));
        //设置信息
        emit getLoginMsg(QString("编号: ") + pUnPacker->GetStr("client_id") + "\t" + QString("用户名: ") +codec->toUnicode(pUnPacker->GetStr("client_name")));
        //发送消息
        emit appendMsg("登录成功");
        //发起订阅
        this->sendSubscription(33101);
        //初始化可用资金
        this->cTradeFundSearch();
        //登录成功
        emit loginIsOk(true);
    }
    //登录失败
    else{
        //消息
        emit appendMsg("登录失败");
        //错误消息
        emit appendMsg(codec->toUnicode( pUnPacker->GetStr("error_info")));
        //登录失败
        emit loginIsOk(false);
    }
}
//338202响应处理
void CTrade_Impl::DealTradeSendEntrust(int Ret, IF2UnPacker* pUnPacker)
{
    //下单成功
    if(pUnPacker->GetStr("error_no") == 0)
    {
        int entrustNo = pUnPacker->GetInt("entrust_no");
        char entrustStatus = pUnPacker->GetChar("entrust_status");
        int entrustDate = pUnPacker->GetInt("entrust_time");
        if(entrustNo)
            emit changeEntrustNoByRet(Ret, SESSION_NUM,entrustNo);
        if(entrustStatus)
            emit changeEntrustStatusByRet(Ret,SESSION_NUM, entrustStatus);
        if(entrustDate)
            emit changeEntrustDateByRet(Ret,SESSION_NUM, entrustDate);
    }
    //下单失败
    else
    {
        const char* errorInfo = pUnPacker->GetStr("error_info");
        emit changeErrorInfoByRet(Ret,SESSION_NUM, codec->toUnicode(errorInfo));
    }
}
//338217响应处理
void CTrade_Impl::DealTradeEntrustBackout(IF2UnPacker *pUnPacker)
{
}
//338300期货资金查询
void CTrade_Impl::DealTradeFundSearch(IF2UnPacker *pUnPacker)
{
    //创建资金信息
    FundInfo* newFund = new FundInfo;
    //赋值
    strcpy(newFund->fundAccount, pUnPacker->GetStr("fund_account"));
    newFund->beginBalance = pUnPacker->GetDouble("begin_balance");
    newFund->currentBalance = pUnPacker->GetDouble("current_balance");
    newFund->enableBalance = pUnPacker->GetDouble("enable_balance");
    newFund->preEntrustBalance = pUnPacker->GetDouble("pre_entrust_balance");
    newFund->entrustBalance = pUnPacker->GetDouble("entrust_balance");
    newFund->entrustFare = pUnPacker->GetDouble("entrust_fare");
    newFund->holdIncome = pUnPacker->GetDouble("hold_income");
    newFund->holdIncomeFloat = pUnPacker->GetDouble("hold_income_float");
    newFund->beginEquityBalance = pUnPacker->GetDouble("begin_equity_balance");
    newFund->equityBalance = pUnPacker->GetDouble("equity_balance");
    newFund->holdMargin = pUnPacker->GetDouble("hold_margin");
    newFund->clientRiskRate = pUnPacker->GetDouble("client_risk_rate");
    //创建
    emit getNewFund(newFund);
}
//338301响应处理
void CTrade_Impl::DealTradeTodayEntrustSearch(IF2UnPacker *pUnPacker)
{
    if (pUnPacker != NULL)
    {
        int nDataSetCount = pUnPacker->GetDatasetCount();

        for (int nIndex = 0; nIndex < nDataSetCount; nIndex++)
        {
            // 设置当前结果集
            pUnPacker->SetCurrentDatasetByIndex(nIndex);
            // 遍历获取结果集列表数据
            for (size_t i = 0; i < pUnPacker->GetRowCount(); i++)
            {
                int entrustNo = pUnPacker->GetInt("entrust_no");
                const char* futuType = pUnPacker->GetStr("futu_exch_type");
                const char* futuCode = pUnPacker->GetStr("futu_code");
                char entrustBs = pUnPacker->GetChar("entrust_bs");
                char futuDir = pUnPacker->GetChar("futures_direction");
                char hedgeType = pUnPacker->GetChar("hedge_type");
                int entrustAmount = pUnPacker->GetInt("entrust_amount");
                double totalBusinessAmount= pUnPacker->GetDouble("business_amount");
                double futuEntrustPrice = pUnPacker->GetDouble("futu_entrust_price");
                int entrustDate = pUnPacker->GetInt("entrust_time");
                char entrustStatus = pUnPacker->GetChar("entrust_status");

                EntrustInfo* newInfo = createNewEntrust(entrustNo, futuType, futuCode,  entrustBs, futuDir, hedgeType, entrustAmount, totalBusinessAmount, futuEntrustPrice,  entrustDate, entrustStatus);

                //发送
                emit getNewEntrustMsg(newInfo);

                pUnPacker->Next();
            }
            pUnPacker->Next();
            //如果row还有
            if(pUnPacker->GetRowCount() == GET_SIZE)
            {
                const char* position = pUnPacker->GetStr("position_str");
                this->cTradeTodayEntrustSearch(position, GET_SIZE);
            }
            //加载完成
            else
            {
                emit loadTodayEntrustEnding();
            }
        }
    }
}
//338302响应处理
void CTrade_Impl::DealTradeTodayBargainSearch(IF2UnPacker *pUnPacker)
{
    if (pUnPacker != NULL)
    {
        int nDataSetCount = pUnPacker->GetDatasetCount();

        for (int nIndex = 0; nIndex < nDataSetCount; nIndex++)
        {
            // 设置当前结果集
            pUnPacker->SetCurrentDatasetByIndex(nIndex);
            // 遍历获取结果集列表数据
            for (size_t i = 0; i < pUnPacker->GetRowCount(); i++)
            {
                //创建新的成交单
                BargainInfo* newBargain = new BargainInfo;
                //赋值
                int no = pUnPacker->GetInt("entrust_no");
                strcpy(newBargain->businessId, pUnPacker->GetStr("business_id"));
                newBargain->futuBusinessPrice = pUnPacker->GetDouble("futu_business_price");
                newBargain->businessAmount =   pUnPacker->GetDouble("business_amount");
                newBargain->businessTime = pUnPacker->GetInt("business_time");
                //成交量
                emit changeTodayBarginInfo(no, newBargain);
                //下一个
                pUnPacker->Next();
            }
            pUnPacker->Next();
            //如果row还有
            if(pUnPacker->GetRowCount() == GET_SIZE){
                const char* position = pUnPacker->GetStr("position_str");
                this->cTradeTodayBargainSearch(position, GET_SIZE);
            }
        }
    }
}
//338303响应处理
void CTrade_Impl::DealTradePositionSearch(IF2UnPacker *pUnPacker)
{
    if (pUnPacker != NULL)
    {
        int nDataSetCount = pUnPacker->GetDatasetCount();

        for (int nIndex = 0; nIndex < nDataSetCount; nIndex++)
        {
            // 设置当前结果集
            pUnPacker->SetCurrentDatasetByIndex(nIndex);
            // 遍历获取结果集列表数据
            for (size_t i = 0; i < pUnPacker->GetRowCount(); i++)
            {
                //创建持仓信息
                PositionInfo* newPosition = new PositionInfo;
                //获取数据
                strcpy(newPosition->futuExchType, pUnPacker->GetStr("futu_exch_type"));
                strcpy(newPosition->futuCode, pUnPacker->GetStr("futu_code"));
                newPosition->entrustBs = pUnPacker->GetChar("entrust_bs");
                newPosition->beginAmount = pUnPacker->GetInt("begin_amount");
                newPosition->enableAmount = pUnPacker->GetInt("enable_amount");
                newPosition->realEnableAmount = pUnPacker->GetInt("real_enable_amount");
                newPosition->hedgeType = pUnPacker->GetChar("hedge_type");
                newPosition->realAmount = pUnPacker->GetInt("real_amount");
                newPosition->realCurrentAmount = pUnPacker->GetDouble("real_amount");
                newPosition->oldCurrentAmount = pUnPacker->GetInt("real_amount");
                //发送信息
                emit getNewPositionMsg(newPosition);
                //下一个
                pUnPacker->Next();
            }
            pUnPacker->Next();
            //如果row还有
            if(pUnPacker->GetRowCount() == GET_SIZE){
                const char* position = pUnPacker->GetStr("position_str");
                this->cTradeTodayBargainSearch(position, GET_SIZE);
            }
        }
    }
}
//620000响应处理
void CTrade_Impl::DealRepFunction620000(IBizMessage* lpMsg)
{
    //改变为请求包
    lpMsg->ChangeReq2AnsMessage();
    //如果连接成功
    if(pConnection)
    {
        //发送请求包
        pConnection->SendBizMsg(lpMsg, 1);
    }
}
//620001响应处理
void CTrade_Impl::DealRepFunction620001(IF2UnPacker *pUnPacker)
{
    //消息
    emit appendMsg("订阅成功");
}
//620003响应处理
void CTrade_Impl::DealRepFunction620003(IF2UnPacker *pUnPacker)
{
    // 用于call_once的局部静态变量
    static std::once_flag oc;
    //消息
    std::call_once(oc, [=](){
           emit appendMsg("订阅连接成功");
    });
    //获取成交类型
    char ly =  pUnPacker->GetChar("LY");
    //获取信息
    QStringList list = QString::fromStdString((pUnPacker->GetStr("QH"))).split('\1');
    //如果为委托汇报
    if(ly != 'B'){
        //委托成功
        emit appendMsg("请求委托成功");
        //改变状态
        emit changeEntrustStatus(list[0].toInt(), list[16].toStdString()[0]);
        //改变日期
        emit changeEntrustDate(list[0].toInt(), list[23].toInt());
        //非已报
        if(list[16] == "2"){
            //委托处于已报状态
            emit appendMsg("委托已上报");
        }
        //部撤
        else if(list[16] == "5"){
            //委托处于已报状态
            emit appendMsg("部分委托已撤销");
        }
        //已撤
        else if(list[16] == "6"){
            //委托处于已报状态
            emit appendMsg("委托已撤销");
        }
        //已报
        else{
            //委托处于异常状态
            emit appendMsg("委托未上报");
        }
    }
    //处于成交状态
    else{
        //创建新的成交单
        BargainInfo* newBargain = new BargainInfo;
        //赋值
        strcpy(newBargain->businessId, list[3].toUtf8());
        newBargain->futuBusinessPrice = list[7].toDouble();
        newBargain->businessAmount =   list[8].toDouble();
        newBargain->businessTime = list[26].toInt();
        //成交量
        emit changeBarginInfo(list[0].toInt(), newBargain);
        //委托处于成交状态
        emit appendMsg("已经买入/卖出" + QString::number(newBargain->businessAmount) + "笔");
    }
}
