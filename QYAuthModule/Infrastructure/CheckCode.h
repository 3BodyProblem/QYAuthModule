#ifndef __MEngine_MCheckCodeH__
#define __MEngine_MCheckCodeH__


#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <exception>


#ifndef LINUXCODE
#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <winnt.h>
#include <windef.h>
#else
#endif


//CRC16位校验
class CRC16
{
	static unsigned short  scrctable[];
public:
	CRC16(void);
	virtual ~CRC16();
public:
	//加入默认参数的原因是像文件等不能一次性拿到全部内容的进行接力计算校验码
	unsigned short CheckCode(const char * lpInBuf,unsigned long sInSize,unsigned short wLastCk = 0 );
};

//CRC32位校验
class CRC32
{
protected:
	static unsigned long	lcrctable[];
public:
	CRC32(void);
	virtual ~CRC32();
public:
	//加入默认参数的原因是像文件等不能一次性拿到全部内容的进行接力计算校验码
	unsigned long CheckCode(const char * lpInBuf,unsigned long sInSize, unsigned long dwLastCk = 0 );
};

//反码和的反码校验，TCP/IP协议中检验数据的方法
class ConCheckCode
{
public:
	ConCheckCode(void);
	~ConCheckCode();
public:
	unsigned short CheckCode(char * lpInBuf,unsigned long sInSize);
};

//MD5报文摘要算法
class MD5
{
private:
    __inline DWORD  inner_f(DWORD X,DWORD Y,DWORD Z);
    __inline DWORD  inner_g(DWORD X,DWORD Y,DWORD Z);
    __inline DWORD  inner_h(DWORD X,DWORD Y,DWORD Z);
    __inline DWORD  inner_i(DWORD X,DWORD Y,DWORD Z);
    __inline DWORD  inner_shrloop(DWORD Value,unsigned char N);
    __inline void   inner_ff(DWORD &A,DWORD B,DWORD C,DWORD D,DWORD X,unsigned char S,DWORD T);
    __inline void   inner_gg(DWORD &A,DWORD B,DWORD C,DWORD D,DWORD X,unsigned char S,DWORD T);
    __inline void   inner_hh(DWORD &A,DWORD B,DWORD C,DWORD D,DWORD X,unsigned char S,DWORD T);
    __inline void   inner_ii(DWORD &A,DWORD B,DWORD C,DWORD D,DWORD X,unsigned char S,DWORD T);
public:
    MD5(void);
    ~MD5();
public:
    int  CalMD5(const char * lpInBuf,int iInSize,char szOutBuf[16]);
	//add by liuyqy 20110923 for 增加接力计算方式
	//Md5接力计算方式，注意接力计算与一次计算的值可能不相等待，主要是因为接力计算时不足64位时的系统补了空数据参与计算，接力为保存与一次性计算相同，则每次数据计算长度必须为64的倍数
	//in_OldMd5Value为上一次的MAC值，lpInBuf为需要计算的数据，iInSize为需要计算的数据长度, szOutBuf为输出的MAC值
	int  CalMD5(const char in_OldMd5[16], const char * lpInBuf,int iInSize,char szOutBuf[16]);

public:
	static long ConvertMD5ToStr(const char *pMD5, char *pStr, unsigned long Size);
	static long ConvertStrToMD5(const char *pStr, char *pMD5, unsigned long Size);
};

#endif




