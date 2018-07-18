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


//CRC16λУ��
class CRC16
{
	static unsigned short  scrctable[];
public:
	CRC16(void);
	virtual ~CRC16();
public:
	//����Ĭ�ϲ�����ԭ�������ļ��Ȳ���һ�����õ�ȫ�����ݵĽ��н�������У����
	unsigned short CheckCode(const char * lpInBuf,unsigned long sInSize,unsigned short wLastCk = 0 );
};

//CRC32λУ��
class CRC32
{
protected:
	static unsigned long	lcrctable[];
public:
	CRC32(void);
	virtual ~CRC32();
public:
	//����Ĭ�ϲ�����ԭ�������ļ��Ȳ���һ�����õ�ȫ�����ݵĽ��н�������У����
	unsigned long CheckCode(const char * lpInBuf,unsigned long sInSize, unsigned long dwLastCk = 0 );
};

//����͵ķ���У�飬TCP/IPЭ���м������ݵķ���
class ConCheckCode
{
public:
	ConCheckCode(void);
	~ConCheckCode();
public:
	unsigned short CheckCode(char * lpInBuf,unsigned long sInSize);
};

//MD5����ժҪ�㷨
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
	//add by liuyqy 20110923 for ���ӽ������㷽ʽ
	//Md5�������㷽ʽ��ע�����������һ�μ����ֵ���ܲ���ȴ�����Ҫ����Ϊ��������ʱ����64λʱ��ϵͳ���˿����ݲ�����㣬����Ϊ������һ���Լ�����ͬ����ÿ�����ݼ��㳤�ȱ���Ϊ64�ı���
	//in_OldMd5ValueΪ��һ�ε�MACֵ��lpInBufΪ��Ҫ��������ݣ�iInSizeΪ��Ҫ��������ݳ���, szOutBufΪ�����MACֵ
	int  CalMD5(const char in_OldMd5[16], const char * lpInBuf,int iInSize,char szOutBuf[16]);
};

#endif




