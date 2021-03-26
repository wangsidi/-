#pragma once

#ifdef _WIN32

typedef char				int8;
typedef short				int16;
typedef int					int32;
typedef long long    		int64;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned long long  uint64;

#else

#include <sys/types.h>

#if defined(__linux__)
#include <stdint.h>
#endif

typedef int8_t				int8;
typedef int16_t				int16;
typedef int32_t				int32;
typedef int64_t				int64;
typedef uint8_t				uint8;
typedef uint16_t			uint16;
typedef uint32_t			uint32;
typedef uint64_t			uint64;

#endif

#pragma pack(push)
#pragma pack(1)//设置结构体的边界对齐为1个字节，也就是所有数据在内存中是连续存储的
#include <map>
//期货、期权、股票统一行情结构
struct UnifiedQuotaData{
    char		m_sExchangeType[8];		//市场代码
    char		m_sContractCode[16];	//合约代码
    char		m_sContractName[32];	//合约名称
    int32		m_nPriceUnit;			//价格单位-行情中价格全乘以此系数（盘口量若非整数，也相同处理成整数）
    int32		m_nPriceDecimal;		//小数位数-价格展示时需要带上的小数点位数
    int32		m_nDeepLength;			//盘口深度-默认五档
    int32		m_nAmountPerHand;		//合约乘数
    char		m_sContractStatus[4];	//合约状态
    int32		m_nPreClosePrice;		//昨收盘价
    int32		m_nPreSettlePrice;		//昨结算价
    int32		m_nOpenPrice;			//开盘价格
    int32		m_nHighestPrice;		//最高价格=====
    int32		m_nLowestPrice;			//最低价格=====
    int32		m_nLastPrice;			//最新价格=====
    int32		m_nUpLimitPrice;		//涨停价格
    int32		m_nDownLimitPrice;		//跌停价格
    int32		m_nClosePrice;			//今收盘价=====
    int32		m_nSettlePrice;			//今结算价=====
    double		m_fBusinessAmount;		//成交数量=====
    double		m_fBusinessBalance;		//成交金额=====
    double		m_fBuyTotalAmount;		//买入总量=====
    double		m_fSellTotalAmount;		//卖出总量=====
    double		m_fHoldAmount;			//持仓总量=====
    int32		m_nAveragePrice;		//平均价格=====
    int32		m_nUpCircuitPrice;		//熔断上限
    int32		m_nDownCircuitPrice;	//熔断下限
    int32		m_nAuctionPrice;		//动态参考价
    char		m_sObjectCode[16];		//品种识别码
    int32		m_nExerciseDate;		//行权日期-格式YYYYMMDD 到日期，行权日。0代表长期有效
    int32		m_nInitDate;			//交易日期-交易所日期YYYYMMDD
    int32		m_nExchangeTime;		//交易所时间-交易所时间，格式HHMMSSmmm===========
    int64		m_nTimeStamp1;			//时间戳1
    int64		m_nTimeStamp2;			//时间戳2
    char		m_sReserveStr[16];		//保留字符串类型字段
    int32		m_nReserveInt;			//保留整型字段
    int32		m_nAskPrice[5];			//卖方五档价格=======
    int32		m_nAskVolumn[5];		//卖方五档数量=======
    int32		m_nBidPrice[5];			//买方五档价格=======
    int32		m_nBidVolumn[5];		//买方五档数量=======
};
struct QuotaData{
    UnifiedQuotaData* data;

    int number = 0;
};

#pragma pack(pop)
