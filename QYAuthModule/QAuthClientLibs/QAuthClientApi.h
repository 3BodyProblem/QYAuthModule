#pragma once

///�汾����
#define    QAUTHCLI_VER_MAIN     0
#define    QAUTHCLI_VER_MAJOR    2
#define    QAUTHCLI_VER_MINOR    2

//����
#ifndef		QAUTH_CONSTANT
#define		QAUTH_CONSTANT
	#define		QAUTH_PWDTYPE_PLAIN			1  //�������ͣ�����
	#define		QAUTH_PWDTYPE_MD5			2  //�������ͣ�MD5

	#define		QAUTH_MSGTYPE_OFFLINE		11  //��֤���������ߣ�����Ϊ�ı�
	#define		QAUTH_MSGTYPE_KICKUSER		12  //���û�������Ϊ�ı�
	#define		QAUTH_MSGTYPE_LOGINEXPIRE	13  //�ỰʧЧ
	#define		QAUTH_MSGTYPE_TEXTMSG		21  //�ı���Ϣ
	#define		QAUTH_MSGTYPE_HTMLMSG		22  //HTML��Ϣ

	#define		QAUTH_EXITTYPE_NONE			0  //����Ҫ�˳�
	#define		QAUTH_EXITTYPE_LOGIN		1  //�˻ص���¼����
	#define		QAUTH_EXITTYPE_APP			2  //�˳�Ӧ��
	
	#define		QAUTH_RESOURCE_FILE			1  //��Դ���ͣ��ļ�
	
	#define     QAUTH_RESOURCE_MAXSIZE      16777216 //��Դ��󳤶ȣ�16M
#endif

///��Ӧ��Ϣ
struct CQAuthRspInfo {
	///����� <0 ��ʾʧ��
	int ErrorID;
	///������Ϣ
	char ErrorMsg[256];
};

///�˺���֤������Ϣ [256]
struct CQAuthLogin {
	///�˺�
	char User[64];
	///����
	char Password[64];
	///��������
	unsigned int PasswordType;
	///��չ����
	char ExtraPassword[64];
	///�����ֶ�
	char _reserved[60];
};

#define     QAPASSPORT_SIZE           2048
struct CQAPassport {
	///���ط�����
	char GateServer[64];
	///����
	int Code;
	///У����
	unsigned int CRC;
	///����
	char _reserved[184];
	///ͨ��֤
	char Passport[QAPASSPORT_SIZE];
};

///�˺���Ȩ��Ϣ [2048 + 10240 + CQAPassport]
struct CQAuthAuthentication {
	///ͨ��֤
	CQAPassport Passport;
	///�ỰID
	char SessionID[64];
	///�˻�Ȩ��ʧЧ����
	int AccountExpireDate;
	///�ⲿIP��ַ
	char ExternalIPAddress[32];
	///�ͻ�ID
	unsigned int CustomerID;
	///�ͻ�����
	char CustomerName[64];
	///��ƷID
	unsigned int ProductID;
	///��Ʒ����
	char ProductName[32];
	///��Ȩ��ʼ����
	unsigned int StartDate;
	///��Ȩ��������
	unsigned int EndDate;
	///֧��Ȩ�ޣ�Ȩ��ID���ö��ŷָ�
	char RightList[300];
	///֧���г����г�ID���ö��ŷָ�
	char MarketList[64];
	///��չ��Ϣ
	char ExtendInfo[10240];
	///�����ֶ�
	char _reserved2[964];
};

///�޸�����������Ϣ
struct CQAuthChangePassword {
	///�˺�
	char User[64];
	///������
	char OldPassword[64];
	///������
	char NewPassword[64];
	///��������
	unsigned int PasswordType;
	///�����ֶ�
	char _reserved[60];
};

///��ѯ��ԴĿ¼&�ļ��������
struct CQAuthQryResource {
	///��Դ����
	int ResourceType;
	///·��
	char Name[512];
	///�Ƿ������Ŀ¼������Name��Ŀ¼������
	bool isRecursive;
	///�����ֶ�
	char _reserved[504];
};

struct CQAuthDownloadResource {
	///��Դ����
	int ResourceType;
	///·��
	char Name[512];
	///�����ֶ�
	char _reserved[508];
};

///��Դ��Ϣ
struct CQAuthResourceInfo {
	///��Դ����
	int ResourceType;
	///·��
	char Name[512];
	///��С
	int Size;
	///����޸�ʱ��
	int LastUpdateTime;
	///�����ֶ�
	char _reserved[500];
};

///��֤������������Ϣ [3072]
struct CQAuthMessage {
	///��ϢID
	unsigned int MessageID;
	///��Ϣ����
	unsigned int MessageType;
	///�˳�����
	unsigned int ExitType;
	///��Ϣ����
	char Title[256];
	///��Ϣ����
	char Text[2048];
	///�����ֶ�
	char _reserved[756];
};

/******************************************************************************
�ͻ�����֤ģ��ӿ�˵��
 
��������
>ģ���ʼ����
  1. ���� QAuthInit ��ʼ��ģ�飻
>����API��������
  1. api = QAuthCreateApi(errmsg);
  2. api->RegisterSpi(spi)
  3. api->Connect()
>������ӳɹ����¼��SPI�յ����ӳɹ����ã�
  1. api->ReqLogin(field, reqid);
>��¼����ִ�и�����������
>API�ͷ�
  1. api->ReqLogout(reqid);
  2. api->Release();
>ģ���ͷţ�
  1. ���� CQAuthRelease;

  ����˵����
  * ԭ�Ƚӿ��е� IP ��Ӳ����Ϣ����ȫ����ģ���ڲ����л�ȡ��

  �ͻ��˴���ӿڷ���ʱ��ע�����¼��㣺
  * ����ص��벻Ҫ��������Ҫ���� api->Release()��
  * ����� nRequestID �ķ�ΧΪ 1 - 32768����������ѭ��ʹ��
******************************************************************************/
///API�������ش����
#define    QAUTH_SUCCESS                0     //�����ɹ�

#define    QAUTH_ERR_INVALIDSTATUS      -100  //�Ƿ��Ĳ���״̬
#define    QAUTH_ERR_NETERROR           -101  //�������

#define    QAUTH_ERR_PARAMERROR         -200  //��������

///��־�ӿ���
#ifndef MLogIOInterface
class MLogIOInterface
{
public:
	//�����Ϣ
	virtual void WriteInfo(const char * szFormat,...) = 0;
	//�������
	virtual void WriteWarning(const char * szFormat,...) = 0;
	//�������
	virtual void WriteError(const char * szFormat,...) = 0;
};
#endif

///��֤��������ַ��Ϣ [64]
struct CQAuthServerConfig {
	///��ַ
	char Host[32];
	///�˿�
	unsigned int Port;
	///�����ֶ�
	char _reserved[28];
};

///��֤ģ���ʼ������ [1536]
#define  AUTHSERVER_MAX     8
struct CQAuthClientInput {
	///�ͻ�������
	unsigned int ClientType;
	///�ͻ��˰汾�����汾���Ӱ汾��Build�ţ�32λ����8��12��12λ
	unsigned int ClientVersion;
	///ȯ��ID
	unsigned int CustomerID;
	///��֤����������
	unsigned int ServerCount;
	///��֤���������ã����8��
	CQAuthServerConfig Servers[AUTHSERVER_MAX];
	///�Ƿ�ʹ��SSL
	bool isUseSSL;
	///crt֤��
	char CrtFilename[256];
	///pfx֤��
	char PfxFilename[256];
	///pfx֤������
	char PfxPassword[32];
	///����ͨѶ��ʱ����λ����
	unsigned int NetworkTimeOut;
	///�����������λ����
	unsigned int HeartbeatInterval;
	///������־
	bool isOutputDebugLog;
	///��־����
	MLogIOInterface * Logger;
	///�����ֶ�
	char _reserved[444];
};

///��֤�ͻ���API�ص��ඨ��
class CQAClientSpi
{
public:
	///����
	virtual void OnConnected() = 0;

	///����&����ʧ��
	virtual void OnDisconnected(int nErrorCode, const char *szErrMsg) = 0;
	
	///��¼��Ӧ
	virtual void OnRspLogin(CQAuthRspInfo *pRspInfo, CQAuthAuthentication *pField, int nRequestID) = 0;
	
	///�ǳ���Ӧ
	virtual void OnRspLogout(CQAuthRspInfo *pRspInfo, int nRequestID) = 0;
	
	///�޸�������Ӧ
	virtual void OnRspChangePassword(CQAuthRspInfo *pRspInfo, int nRequestID) = 0;
	
	///������Դ��ѯ��Ӧ
	virtual void OnRspQryPublicResource(CQAuthRspInfo *pRspInfo, CQAuthResourceInfo *pField, int nRequestID, bool isLast) = 0;
	
	///������Դ������Ӧ
	virtual void OnRspDownloadPublicResource(CQAuthRspInfo *pRspInfo, CQAuthResourceInfo *pField, const char *szData, int nRequestID) = 0;
	
	///�û���Դ��ѯ��Ӧ
	virtual void OnRspQryPrivateResource(CQAuthRspInfo *pRspInfo, CQAuthResourceInfo *pField, int nRequestID, bool isLast) = 0;
	
	///�û���Դ������Ӧ
	virtual void OnRspDownloadPrivateResource(CQAuthRspInfo *pRspInfo, CQAuthResourceInfo *pField, const char *szData, int nRequestID) = 0;
	
	///�û���Դ������Ӧ
	virtual void OnRspUploadPrivateResource(CQAuthRspInfo *pRspInfo, int nRequestID) = 0;
	
	///������Ϣ
	virtual void OnRtnMessage(CQAuthMessage *pField) = 0;
};

///��֤�ͻ���API
class CQAClientApi
{
public:
	///��ȡID
	virtual int GetID() = 0;

	///�ͷ�API
	virtual void Release() = 0;
	
	///ע��SPI����
	virtual void RegisterSpi(CQAClientSpi *pSpi) = 0;
	
	///��ʼ����
	virtual int Connect() = 0;
	
	///�Ͽ�����
	virtual void Disconnect() = 0;
	
	///�û���¼
	virtual int ReqLogin(CQAuthLogin *pField, int nRequestID) = 0;
	
	///�û��ǳ�
	virtual int ReqLogout(int nRequestID) = 0;
	
	///�û���¼
	virtual int ReqChangePassword(CQAuthChangePassword *pField, int nRequestID) = 0;
	
	///������Դ��ѯ
	virtual int ReqQryPublicResource(CQAuthQryResource *pField, int nRequestID) = 0;
	
	///������Դ����
	virtual int ReqDownloadPublicResource(CQAuthDownloadResource *pField, int nRequestID) = 0;
	
	///�û���Դ��ѯ
	virtual int ReqQryPrivateResource(CQAuthQryResource *pField, int nRequestID) = 0;
	
	///�û���Դ����
	virtual int ReqDownloadPrivateResource(CQAuthDownloadResource *pField, int nRequestID) = 0;
	
	///�û���Դ�ϴ�
	virtual int ReqUploadPrivateResource(CQAuthResourceInfo *pField, const char *szData, int nRequestID) = 0;

protected:
	virtual ~CQAClientApi(){};
};

///ϵͳ��ʼ��
extern int QAuthInit(CQAuthClientInput *pInput, char *szErrMsg);

///�����ͻ���API����
extern CQAClientApi * QAuthCreateApi(char *szErrMsg);

///�ͷ�ϵͳ��Դ
extern void QAuthRelease();
