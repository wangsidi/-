#ifndef FUNC_H
#define FUNC_H

#ifdef LINUX
#include <sys/un.h>
#endif
#include <iostream>
#include <string>

#include "t2sdk_interface.h"

using namespace std;

#define  MSGCENTER_FUNC_HEART		 620000								//消息中心心跳功能号
#define  MSGCENTER_FUNC_REG			 620001								//消息中心订阅功能号
#define  MSGCENTER_FUNC_REG_CANCLE   620002								//消息中心取消订阅功能号
#define  MSGCENTER_FUNC_SENDED		 620003								//消息中心主推功能号

#define  ISSUE_TYPE_MERKET           33100								//订阅期货单腿行情
#define  ISSUE_TYPE_REALTIME         33101								//订阅期货委托成交回报

///错误信息
struct CHSRspInfoField
{
    ///错误代码
    int	ErrorID;
    ///错误提示
    char	ErrorMsg[255];
};

//字符串拷贝
char *hs_strncpy(char *dest,const char *src, size_t size);
//打印Packer包
void ShowPacket(IF2UnPacker *lpUnPacker);
//字符串替换
string& replace_all(string& str,const string& old_value,const string& new_value);
//处理错误信息
void DealErrorInfo(int nReturnCode, int nErrorNo, const char *pszErrorInfo, IF2UnPacker *lpUnPacker, CHSRspInfoField *pRspInfo);

#endif
