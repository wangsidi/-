#include "FutuQuota.h"

#include <QDebug>
#include "public/QuotaStruct.h"

static bool lock = false;
//构造函数
CQuota_Impl::CQuota_Impl():
    pStrockMap(new StrockMap),
    isConnect(false)
{
    //链接对象创建及配置
    pConfig = NULL;
    pConnection = NULL;
    //初始化配置
    pConfig = NewConfig();
    pConfig->AddRef();
}
//析构函数
CQuota_Impl::~CQuota_Impl()
{
    lock = true;
    if(pConnection)
        pConnection->Release();
    if(pConfig)
        pConfig->Release();
    //析构map指针
    for(std::string key : keys)
        delete pStrockMap->find(key/*.first*/)->second;
    //析构值
    pStrockMap->clear();
    //析构建
    keys.clear();
}

unsigned long CQuota_Impl::QueryInterface(const char *iid, IKnown **ppv)
{
    return 0;
}

unsigned long CQuota_Impl::AddRef()
{
    return 0;
}

unsigned long CQuota_Impl::Release()
{
    return 0;
}
void CQuota_Impl::OnConnect(CConnectionInterface *pConnection){
    cerr<<"CQuota_Impl::OnConnect"<<endl;
}

void CQuota_Impl::OnSafeConnect(CConnectionInterface *pConnection){
    cerr<<"CQuota_Impl::OnSafeConnect"<<endl;
}

void CQuota_Impl::OnRegister(CConnectionInterface *pConnection){
    cerr<<"CQuota_Impl::OnRegister"<<endl;
}
void CQuota_Impl::OnClose(CConnectionInterface *pConnection){
    //无连接
    isConnect = false;
    //消息
    emit setSubStatus("订阅状态: 关");
    //发射重连信号
    emit reconnection();

    cerr<<"CQuota_Impl::OnClose"<<endl;
}

void CQuota_Impl::OnSent(CConnectionInterface *pConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData){
    cerr<<"CQuota_Impl::Onsent"<<endl;
}

void CQuota_Impl::Reserved1(void *a, void *b, void *c, void *d){
    cerr<<"CQuota_Impl::Reserved1"<<endl;
}


void CQuota_Impl::Reserved2(void *a, void *b, void *c, void *d){
    cerr<<"CQuota_Impl::Reserved2"<<endl;
}

int  CQuota_Impl::Reserved3()
{
    cerr<<"CQuota_Impl::Reserved3"<<endl;
    return 0;
}

void CQuota_Impl::Reserved4()
{
    cerr<<"CQuota_Impl::Reserved4"<<endl;
}

void CQuota_Impl::Reserved5()
{
    cerr<<"CQuota_Impl::Reserved5"<<endl;
}

void CQuota_Impl::Reserved6()
{
    cerr<<"CQuota_Impl::Reserved6"<<endl;
}

void CQuota_Impl::Reserved7()
{
    cerr<<"CQuota_Impl::Reserved7"<<endl;
}

void CQuota_Impl::OnReceivedBiz(CConnectionInterface *pConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
    cerr<<"CQuota_Impl::OnReceivedBiz"<<endl;
}
void CQuota_Impl::OnReceivedBizEx(CConnectionInterface *pConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult){
    cerr<<"CQuota_Impl::OnReceivedBizEx"<<endl;
}
void CQuota_Impl::OnReceivedBizMsg(CConnectionInterface *pConnection, int hSend, IBizMessage* lpMsg)
{
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
        case 620001:
            //620001响应处理
            DealRepFunction620001();
            break;
        case 620003:
        {
            const UnifiedQuotaData* buffer = static_cast<const UnifiedQuotaData*>(lpMsg->GetContent(iLen));
            //620003响应处理
            DealRepFunction620003(buffer);
            break;
        }
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
int CQuota_Impl::InitConn(QString infoPath)
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
    //返回成功 nRet = 0;
    return nRet;
}
//620001_33101期权订阅委托成交回报 -- 行情
int CQuota_Impl::sendSubscription(int nIssueType, const char* exchangeType, const char* contractCode)
{
    //创建业务包体
    IF2Packer *pPacker=NewPacker(2);
    pPacker->AddRef();
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("exchange_type", 'S', 8);
    pPacker->AddField("contract_code", 'S', 16);
    pPacker->AddField("object_code",'S');
    //加入对应的字段值
    pPacker->AddStr(exchangeType);
    pPacker->AddStr(contractCode);
    pPacker->AddStr("*");
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
//620000响应处理
void CQuota_Impl::DealRepFunction620000(IBizMessage* lpMsg)
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
void CQuota_Impl::DealRepFunction620001(IF2UnPacker *pUnPacker)
{
    //消息
    emit setSubStatus("订阅状态: 开");
}
//620003响应处理
void CQuota_Impl::DealRepFunction620003(const UnifiedQuotaData* contentBuffer)
{
    //锁
    if(lock) return;
    //键值
    CodeKey key = std::string(contentBuffer->m_sContractCode);
    //如果key为空 return
    if(key.empty()) return;
    //是否存在
    //    auto findIter = std::find_if(keys.begin(), keys.end(), [=](const std::pair<CodeKey, int> pair){/**/
    //        return (key == pair.first);/**/
    //    });/**/
    auto findIter = std::find_if(keys.begin(), keys.end(), [=](const CodeKey k){/**/
        return (key == k);/**/
    });/**/
    //如果存在
    if(findIter != keys.end()){
        //++
//        findIter->second++;/**/
        //赋值 -- 部分不需要重复赋值-- 需要修改
        (*pStrockMap)[key]->m_nInitDate = contentBuffer->m_nInitDate;
        (*pStrockMap)[key]->m_nLastPrice = contentBuffer->m_nLastPrice;
        (*pStrockMap)[key]->m_nPriceUnit = contentBuffer->m_nPriceUnit;
        (*pStrockMap)[key]->m_fHoldAmount = contentBuffer->m_fHoldAmount;
        (*pStrockMap)[key]->m_nClosePrice = contentBuffer->m_nClosePrice;
        (*pStrockMap)[key]->m_nDeepLength = contentBuffer->m_nDeepLength;
        (*pStrockMap)[key]->m_nReserveInt = contentBuffer->m_nReserveInt;
        (*pStrockMap)[key]->m_nTimeStamp1 = contentBuffer->m_nTimeStamp1;
        (*pStrockMap)[key]->m_nTimeStamp2 = contentBuffer->m_nTimeStamp2;
        (*pStrockMap)[key]->m_nLowestPrice = contentBuffer->m_nLowestPrice;
        (*pStrockMap)[key]->m_nSettlePrice = contentBuffer->m_nSettlePrice;
        (*pStrockMap)[key]->m_nAuctionPrice = contentBuffer->m_nAuctionPrice;
        (*pStrockMap)[key]->m_nAveragePrice = contentBuffer->m_nAveragePrice;
        (*pStrockMap)[key]->m_nExchangeTime = contentBuffer->m_nExchangeTime;
        (*pStrockMap)[key]->m_nExerciseDate = contentBuffer->m_nExerciseDate;
        (*pStrockMap)[key]->m_nHighestPrice = contentBuffer->m_nHighestPrice;
        (*pStrockMap)[key]->m_nPriceDecimal = contentBuffer->m_nPriceDecimal;
        (*pStrockMap)[key]->m_nUpLimitPrice = contentBuffer->m_nUpLimitPrice;
        (*pStrockMap)[key]->m_nAmountPerHand = contentBuffer->m_nAmountPerHand;
        (*pStrockMap)[key]->m_fBusinessAmount = contentBuffer->m_fBusinessAmount;
        (*pStrockMap)[key]->m_fBuyTotalAmount = contentBuffer->m_fBuyTotalAmount;
        (*pStrockMap)[key]->m_nDownLimitPrice = contentBuffer->m_nDownLimitPrice;
        (*pStrockMap)[key]->m_nUpCircuitPrice = contentBuffer->m_nUpCircuitPrice;
        (*pStrockMap)[key]->m_fBusinessBalance = contentBuffer->m_fBusinessBalance;
        (*pStrockMap)[key]->m_fSellTotalAmount = contentBuffer->m_fSellTotalAmount;
        (*pStrockMap)[key]->m_nDownCircuitPrice = contentBuffer->m_nDownCircuitPrice;
        strcpy((*pStrockMap)[key]->m_sObjectCode, contentBuffer->m_sObjectCode);
        strcpy((*pStrockMap)[key]->m_sReserveStr, contentBuffer->m_sReserveStr);
        strcpy((*pStrockMap)[key]->m_sContractStatus, contentBuffer->m_sContractStatus);
        for(int i = 0; i != 5; ++i)
            (*pStrockMap)[key]->m_nAskPrice[i] = contentBuffer->m_nAskPrice[i];
        for(int i = 0; i != 5; ++i)
            (*pStrockMap)[key]->m_nBidPrice[i] = contentBuffer->m_nBidPrice[i];
        for(int i = 0; i != 5; ++i)
            (*pStrockMap)[key]->m_nAskVolumn[i] = contentBuffer->m_nAskVolumn[i];
        for(int i = 0; i != 5; ++i)
            (*pStrockMap)[key]->m_nBidVolumn[i] = contentBuffer->m_nBidVolumn[i];
    }
    else
    {
        //加入键集合
        keys.push_back(/*make_pair(*/key/*, 1)*/);/**/
        //新的股票信息
        UnifiedQuotaData* newData = new UnifiedQuotaData;
        //赋值
        newData->m_nInitDate = contentBuffer->m_nInitDate;
        newData->m_nLastPrice = contentBuffer->m_nLastPrice;
        newData->m_nOpenPrice = contentBuffer->m_nOpenPrice;
        newData->m_nPriceUnit = contentBuffer->m_nPriceUnit;
        newData->m_fHoldAmount = contentBuffer->m_fHoldAmount;
        newData->m_nClosePrice = contentBuffer->m_nClosePrice;
        newData->m_nDeepLength = contentBuffer->m_nDeepLength;
        newData->m_nReserveInt = contentBuffer->m_nReserveInt;
        newData->m_nTimeStamp1 = contentBuffer->m_nTimeStamp1;
        newData->m_nTimeStamp2 = contentBuffer->m_nTimeStamp2;
        newData->m_nLowestPrice = contentBuffer->m_nLowestPrice;
        newData->m_nSettlePrice = contentBuffer->m_nSettlePrice;
        newData->m_nAuctionPrice = contentBuffer->m_nAuctionPrice;
        newData->m_nAveragePrice = contentBuffer->m_nAveragePrice;
        newData->m_nExchangeTime = contentBuffer->m_nExchangeTime;
        newData->m_nExerciseDate = contentBuffer->m_nExerciseDate;
        newData->m_nHighestPrice = contentBuffer->m_nHighestPrice;
        newData->m_nPriceDecimal = contentBuffer->m_nPriceDecimal;
        newData->m_nUpLimitPrice = contentBuffer->m_nUpLimitPrice;
        newData->m_nAmountPerHand = contentBuffer->m_nAmountPerHand;
        newData->m_nPreClosePrice = contentBuffer->m_nPreClosePrice;
        newData->m_fBusinessAmount = contentBuffer->m_fBusinessAmount;
        newData->m_fBuyTotalAmount = contentBuffer->m_fBuyTotalAmount;
        newData->m_nDownLimitPrice = contentBuffer->m_nDownLimitPrice;
        newData->m_nPreSettlePrice = contentBuffer->m_nPreSettlePrice;
        newData->m_nUpCircuitPrice = contentBuffer->m_nUpCircuitPrice;
        newData->m_fBusinessBalance = contentBuffer->m_fBusinessBalance;
        newData->m_fSellTotalAmount = contentBuffer->m_fSellTotalAmount;
        newData->m_nDownCircuitPrice = contentBuffer->m_nDownCircuitPrice;
        strcpy(newData->m_sObjectCode, contentBuffer->m_sObjectCode);
        strcpy(newData->m_sReserveStr, contentBuffer->m_sReserveStr);
        strcpy(newData->m_sContractCode, contentBuffer->m_sContractCode);
        strcpy(newData->m_sContractName, contentBuffer->m_sContractName);
        strcpy(newData->m_sExchangeType, contentBuffer->m_sExchangeType);
        strcpy(newData->m_sContractStatus, contentBuffer->m_sContractStatus);
        for(int i = 0; i != 5; ++i)
            newData->m_nAskPrice[i] = contentBuffer->m_nAskPrice[i];
        for(int i = 0; i != 5; ++i)
            newData->m_nBidPrice[i] = contentBuffer->m_nBidPrice[i];
        for(int i = 0; i != 5; ++i)
            newData->m_nAskVolumn[i] = contentBuffer->m_nAskVolumn[i];
        for(int i = 0; i != 5; ++i)
            newData->m_nBidVolumn[i] = contentBuffer->m_nBidVolumn[i];
        //赋值信息
        (*pStrockMap)[key] = newData;
    }
}
//维护心跳
void CQuota_Impl::OnHeartbeat(IBizMessage* pMsg)
{
    //订阅心跳维护
    if (pMsg->GetPacketType() == REQUEST_PACKET)
    {
        pMsg->ChangeReq2AnsMessage();
        if(NULL != pConnection)
            pConnection->SendBizMsg(pMsg, 1);
    }
}
