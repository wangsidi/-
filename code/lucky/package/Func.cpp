#include "Func.h"

char *hs_strncpy(char *dest,const char *src, size_t size)
{
    if( dest == NULL)
        return 0;
    if( src == NULL)
    {
        dest[0] = 0;
        return 0;
    }
    char *ret = strncpy(dest, src, size - 1);
    dest[size-1] = 0;
    return ret;
}

string& replace_all (string& str,const string& old_value,const string& new_value)
{
    while(true)
    {
        string::size_type   pos(0);
        if((pos=str.find(old_value))!=string::npos)
        {
            str.replace(pos,old_value.length(),new_value);
        }
        else   break;
    }
    return   str;
}

void DealErrorInfo(int nReturnCode, int nErrorNo, const char *pszErrorInfo, IF2UnPacker *lpUnPacker, CHSRspInfoField *pRspInfo)
{
    //ReturnCode（7域）为0，说明请求处理正常（包括通讯和业务）
    if(nReturnCode == 0)
    {
        //不对Packer处理
        pRspInfo->ErrorID = 0;
        hs_strncpy(pRspInfo->ErrorMsg, "No Error", sizeof(pRspInfo->ErrorMsg));
    }
    else
    {
        //判断如果Packer不为空，则取业务包中的错误信息
        if(lpUnPacker != NULL)
        {
            pRspInfo->ErrorID = lpUnPacker->GetInt("error_no");
            hs_strncpy(pRspInfo->ErrorMsg, lpUnPacker->GetStr("error_info"), sizeof(pRspInfo->ErrorMsg));
        }
        //否则直接取ESB的ErrorNo（19域）和ErrorInfo（20域）
        else
        {
            pRspInfo->ErrorID = nErrorNo;
            hs_strncpy(pRspInfo->ErrorMsg, pszErrorInfo, sizeof(pRspInfo->ErrorMsg));
        }
    }
};
