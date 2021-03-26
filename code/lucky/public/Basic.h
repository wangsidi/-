#ifndef BASIC_H
#define BASIC_H
#include <cstring>
#include <vector>
#include <QTableView>
//前置声明
class QTextCodec;
//编码处理
extern  QTextCodec* codec;
//全局调用
//规定大小
extern const size_t GET_SIZE;
//错误信息
const QString C_ErrorInfo = "信息不能为空";
//缺省数
extern int LOSS_NUM;
//最大引用
extern int INDEX_LIST;
//转详细信息
inline QString EntrustBs2String(char entrustBs)
{
    switch (entrustBs)
    {
    case '1':
        return "买入";
    case '2':
        return "卖出";
    default:
        return "";
    }
}
inline QString FuturesDirection2String(char futuresDirection)
{
    switch (futuresDirection)
    {
    case '1':
        return "开仓";
    case '2':
        return "平仓";
    case '3':
        return "交割";
    case '4':
        return "平今仓";
    case '5':
        return "平仓+平今仓";
    default:
        return "";
    }
}
inline QString HedgeType2String(char hedgeType)
{
    switch (hedgeType)
    {
    case '0':
        return "投机";
    case '1':
        return "套保";
    case '2':
        return "套利";
    case '3':
        return "做市商";
    case '4':
        return "备兑";
    default:
        return "";
    }
}
inline QString EntrustStatus2String(char entrustStatus)
{
    switch (entrustStatus)
    {
    case '0':
        return " 0-未报 ";
    case '1':
        return " 1-待报 ";
    case '2':
        return " 2-已报 ";
    case '3':
        return " 3-已报待撤 ";
    case '4':
        return " 4-部成待撤 ";
    case '5':
        return " 5-部撤 ";
    case '6':
        return " 6-已撤 ";
    case '7':
        return " 7-部成 ";
    case '8':
        return " 8-已成 ";
    case '9':
        return " 9-废单 ";
    case 'A':
        return " A-已报待改(港股) ";
    case 'B':
        return " B-预埋单撤单(港股) ";
    case 'C':
        return " C-正报 ";
    case 'D':
        return " D-撤废 ";
    case 'E':
        return " E-部成待改(港股) ";
    case 'F':
        return " F-预埋单废单(港股) ";
    case 'G':
        return " G-单腿成交 ";
    case 'H':
        return "H-待审核(港股)";
    case 'J':
        return " J-复核未通过(港股) ";
    case 'M':
        return " M-Wait for Confirming ";
    case 'S':
        return " S-已报待改 ";
    case 'T':
        return " T-部成待改 ";
    case 'U':
        return " U-已确认待撤 ";
    case 'V':
        return " V-已确认 ";
    case 'W':
        return " W-待确认 ";
    case 'X':
        return " X-预成交 ";
    case 'Y':
        return " Y-购回待确认 ";
    case 'Z':
        return " Z-已购回 ";
    default:
        return "";
    }
}
//加入model
inline void setTableViewModel(QTableView* view, QAbstractItemModel* newModel)
{
    //原先的model
    QItemSelectionModel* delmodel = view->selectionModel();;
    //加入新的model
    view->setModel(newModel);
    //释放原先的model
    delete delmodel;
}
//账号信息
struct TradeAccount{
    //资金账号（请联系环境负责人获取资金账号）
    char FundAccount[32];
    //交易密码（请联系环境负责人获取对应密码）
    char Password[32];
    //委托方式
    char EntrustWay;
    //看穿式AppID
    char AppId[255];
    //看穿式认证串
    char AuthCode[64];

    TradeAccount(){
        memset(FundAccount, '\0', sizeof(FundAccount));
        memset(Password, '\0', sizeof(Password));
        EntrustWay = '7';
        memset(AuthCode, '\0', sizeof(AuthCode));
        memset(AppId, '\0', sizeof(AppId));
    }

};
//成交信息
struct BargainInfo{
    //委托编号
    char businessId[32];
    //成交量
    float businessAmount = 0;
    //成交价格
    double futuBusinessPrice = 0;
    //成交时间
    int businessTime = 0;
    //初始化
    void init(){
        memset(businessId, '\0', 32);
    }
    //构建
    BargainInfo(){
        init();
    }
};
//委托信息
struct EntrustInfo{
    //委托编号
    int entrust_no = 0;
    //交易类别
    char futuExchType[4];
    //合约代码
    char futuCode[30];
    //买卖方向
    char entrustBs = '0';
    //开平仓方向
    int futuresDirection = 0;
    //投机/套保类型
    char hedgeType = '0';
    //委托数量
    int entrustAmount = 0;
    //总成交数量
    double totalBusinessAmount = 0;
    //委托价格
    double futuEntrustPrice = 0;
    //委托状态
    char entrustStatus = '0';
    //委托时间
    int entrustDate = 0;
    //定位器
    int ret = -1;
    //会话编号
    int sessionNum = -1;
    //成交单
    std::vector<BargainInfo*> bargainList;
    //错误信息
    char errorInfo[20];
    //构造
    EntrustInfo(){
        init();
    }
    //初始化
    void init()
    {
        memset(futuExchType, '\0', 4);
        memset(futuCode, '\0', 30);
        memset(errorInfo, '\0', 20);
    }
};
//合约汇总
struct FutuCollect{
    //合约代码
    char futuCode[30];
    //持仓
    int position = 0;
    //构造
    FutuCollect(){
        init();
    }
    //初始化
    void init()
    {
        memset(futuCode, '\0', 30);
    }
};
//持仓信息
struct PositionInfo{
    //交易类别
    char futuExchType[4];
    //合约代码
    char futuCode[30];
    //买卖方向
    char entrustBs = '0';
    //期初数量
    int beginAmount = 0;
    //可用数量
    int enableAmount = 0;
    //当日开仓可用数量
    int realEnableAmount = 0;
    //投机/套保类型
    char hedgeType = '0';
    //成交数量
    int realAmount = 0;
    //今总持仓
    double realCurrentAmount = 0;
    //老仓持仓数量
    int oldCurrentAmount = 0;
    //构造
    PositionInfo(){
        init();
    }
    //初始化
    void init()
    {
        memset(futuExchType, '\0', 4);
        memset(futuCode, '\0', 30);
    }
};

//资金
struct FundInfo{
    //资产账户
    char fundAccount[18];
    //期初余额
    double beginBalance = 0;
    //当前余额
    double currentBalance = 0;
    //可用资金
    double enableBalance = 0;
    //当日开仓预冻结金额
    double preEntrustBalance = 0;
    //委托金额
    double entrustBalance = 0;
    //委托费用
    double entrustFare = 0;
    //期货盯市盈亏
    double holdIncome = 0;
    //持仓浮动盈亏
    double holdIncomeFloat = 0;
    //期初客户权益
    double beginEquityBalance = 0;
    //客户权益
    double equityBalance = 0;
    //持仓保证金
    double holdMargin = 0;
    //客户风险率
    double clientRiskRate = 0;
    //构造
    FundInfo(){
        init();
    }
    //初始化
    void init()
    {
        memset(fundAccount, '\0', 18);
    }
};
#endif // BASIC_H
