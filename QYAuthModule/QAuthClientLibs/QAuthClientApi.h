#pragma once

///版本定义
#define    QAUTHCLI_VER_MAIN     0
#define    QAUTHCLI_VER_MAJOR    2
#define    QAUTHCLI_VER_MINOR    2

//常量
#ifndef		QAUTH_CONSTANT
#define		QAUTH_CONSTANT
	#define		QAUTH_PWDTYPE_PLAIN			1  //密码类型：明文
	#define		QAUTH_PWDTYPE_MD5			2  //密码类型：MD5

	#define		QAUTH_MSGTYPE_OFFLINE		11  //认证服务器掉线，正文为文本
	#define		QAUTH_MSGTYPE_KICKUSER		12  //踢用户，正文为文本
	#define		QAUTH_MSGTYPE_LOGINEXPIRE	13  //会话失效
	#define		QAUTH_MSGTYPE_TEXTMSG		21  //文本消息
	#define		QAUTH_MSGTYPE_HTMLMSG		22  //HTML消息

	#define		QAUTH_EXITTYPE_NONE			0  //不需要退出
	#define		QAUTH_EXITTYPE_LOGIN		1  //退回到登录界面
	#define		QAUTH_EXITTYPE_APP			2  //退出应用
	
	#define		QAUTH_RESOURCE_FILE			1  //资源类型：文件
	
	#define     QAUTH_RESOURCE_MAXSIZE      16777216 //资源最大长度，16M
#endif

///响应消息
struct CQAuthRspInfo {
	///错误号 <0 表示失败
	int ErrorID;
	///错误消息
	char ErrorMsg[256];
};

///账号认证请求信息 [256]
struct CQAuthLogin {
	///账号
	char User[64];
	///密码
	char Password[64];
	///密码类型
	unsigned int PasswordType;
	///扩展密码
	char ExtraPassword[64];
	///保留字段
	char _reserved[60];
};

#define     QAPASSPORT_SIZE           2048
struct CQAPassport {
	///网关服务器
	char GateServer[64];
	///编码
	int Code;
	///校验码
	unsigned int CRC;
	///保留
	char _reserved[184];
	///通行证
	char Passport[QAPASSPORT_SIZE];
};

///账号授权信息 [2048 + 10240 + CQAPassport]
struct CQAuthAuthentication {
	///通行证
	CQAPassport Passport;
	///会话ID
	char SessionID[64];
	///账户权限失效日期
	int AccountExpireDate;
	///外部IP地址
	char ExternalIPAddress[32];
	///客户ID
	unsigned int CustomerID;
	///客户名称
	char CustomerName[64];
	///产品ID
	unsigned int ProductID;
	///产品名称
	char ProductName[32];
	///授权开始日期
	unsigned int StartDate;
	///授权结束日期
	unsigned int EndDate;
	///支持权限，权限ID，用逗号分隔
	char RightList[300];
	///支持市场，市场ID，用逗号分隔
	char MarketList[64];
	///扩展信息
	char ExtendInfo[10240];
	///保留字段
	char _reserved2[964];
};

///修改密码请求信息
struct CQAuthChangePassword {
	///账号
	char User[64];
	///旧密码
	char OldPassword[64];
	///新密码
	char NewPassword[64];
	///密码类型
	unsigned int PasswordType;
	///保留字段
	char _reserved[60];
};

///查询资源目录&文件请求参数
struct CQAuthQryResource {
	///资源类型
	int ResourceType;
	///路径
	char Name[512];
	///是否遍历子目录，仅当Name是目录是于晓
	bool isRecursive;
	///保留字段
	char _reserved[504];
};

struct CQAuthDownloadResource {
	///资源类型
	int ResourceType;
	///路径
	char Name[512];
	///保留字段
	char _reserved[508];
};

///资源信息
struct CQAuthResourceInfo {
	///资源类型
	int ResourceType;
	///路径
	char Name[512];
	///大小
	int Size;
	///最后修改时间
	int LastUpdateTime;
	///保留字段
	char _reserved[500];
};

///认证服务器推送消息 [3072]
struct CQAuthMessage {
	///消息ID
	unsigned int MessageID;
	///消息类型
	unsigned int MessageType;
	///退出类型
	unsigned int ExitType;
	///消息标题
	char Title[256];
	///消息正文
	char Text[2048];
	///保留字段
	char _reserved[756];
};

/******************************************************************************
客户端认证模块接口说明
 
处理流程
>模块初始化：
  1. 调用 QAuthInit 初始化模块；
>创建API，并连接
  1. api = QAuthCreateApi(errmsg);
  2. api->RegisterSpi(spi)
  3. api->Connect()
>如果连接成功则登录（SPI收到连接成功调用）
  1. api->ReqLogin(field, reqid);
>登录即可执行各种其他操作
>API释放
  1. api->ReqLogout(reqid);
  2. api->Release();
>模块释放：
  1. 调用 CQAuthRelease;

  补充说明；
  * 原先接口中的 IP 等硬件信息，现全部由模块内部自行获取；

  客户端处理接口访问时请注意以下几点：
  * 处理回调请不要阻塞，不要调用 api->Release()；
  * 请求号 nRequestID 的范围为 1 - 32768，到达上限循环使用
******************************************************************************/
///API函数返回错误号
#define    QAUTH_SUCCESS                0     //操作成功

#define    QAUTH_ERR_INVALIDSTATUS      -100  //非法的操作状态
#define    QAUTH_ERR_NETERROR           -101  //网络错误

#define    QAUTH_ERR_PARAMERROR         -200  //参数错误

///日志接口类
#ifndef MLogIOInterface
class MLogIOInterface
{
public:
	//输出信息
	virtual void WriteInfo(const char * szFormat,...) = 0;
	//输出警告
	virtual void WriteWarning(const char * szFormat,...) = 0;
	//输出错误
	virtual void WriteError(const char * szFormat,...) = 0;
};
#endif

///认证服务器地址信息 [64]
struct CQAuthServerConfig {
	///地址
	char Host[32];
	///端口
	unsigned int Port;
	///保留字段
	char _reserved[28];
};

///认证模块初始化属性 [1536]
#define  AUTHSERVER_MAX     8
struct CQAuthClientInput {
	///客户端类型
	unsigned int ClientType;
	///客户端版本，主版本、子版本、Build号，32位各用8、12、12位
	unsigned int ClientVersion;
	///券商ID
	unsigned int CustomerID;
	///认证服务器数量
	unsigned int ServerCount;
	///认证服务器配置，最多8组
	CQAuthServerConfig Servers[AUTHSERVER_MAX];
	///是否使用SSL
	bool isUseSSL;
	///crt证书
	char CrtFilename[256];
	///pfx证书
	char PfxFilename[256];
	///pfx证书密码
	char PfxPassword[32];
	///网络通讯超时，单位：秒
	unsigned int NetworkTimeOut;
	///心跳间隔，单位：秒
	unsigned int HeartbeatInterval;
	///调试日志
	bool isOutputDebugLog;
	///日志函数
	MLogIOInterface * Logger;
	///保留字段
	char _reserved[444];
};

///认证客户端API回调类定义
class CQAClientSpi
{
public:
	///连接
	virtual void OnConnected() = 0;

	///断线&连接失败
	virtual void OnDisconnected(int nErrorCode, const char *szErrMsg) = 0;
	
	///登录响应
	virtual void OnRspLogin(CQAuthRspInfo *pRspInfo, CQAuthAuthentication *pField, int nRequestID) = 0;
	
	///登出响应
	virtual void OnRspLogout(CQAuthRspInfo *pRspInfo, int nRequestID) = 0;
	
	///修改密码响应
	virtual void OnRspChangePassword(CQAuthRspInfo *pRspInfo, int nRequestID) = 0;
	
	///公共资源查询响应
	virtual void OnRspQryPublicResource(CQAuthRspInfo *pRspInfo, CQAuthResourceInfo *pField, int nRequestID, bool isLast) = 0;
	
	///公共资源下载响应
	virtual void OnRspDownloadPublicResource(CQAuthRspInfo *pRspInfo, CQAuthResourceInfo *pField, const char *szData, int nRequestID) = 0;
	
	///用户资源查询响应
	virtual void OnRspQryPrivateResource(CQAuthRspInfo *pRspInfo, CQAuthResourceInfo *pField, int nRequestID, bool isLast) = 0;
	
	///用户资源下载响应
	virtual void OnRspDownloadPrivateResource(CQAuthRspInfo *pRspInfo, CQAuthResourceInfo *pField, const char *szData, int nRequestID) = 0;
	
	///用户资源下载响应
	virtual void OnRspUploadPrivateResource(CQAuthRspInfo *pRspInfo, int nRequestID) = 0;
	
	///推送消息
	virtual void OnRtnMessage(CQAuthMessage *pField) = 0;
};

///认证客户端API
class CQAClientApi
{
public:
	///获取ID
	virtual int GetID() = 0;

	///释放API
	virtual void Release() = 0;
	
	///注册SPI对象
	virtual void RegisterSpi(CQAClientSpi *pSpi) = 0;
	
	///开始连接
	virtual int Connect() = 0;
	
	///断开连接
	virtual void Disconnect() = 0;
	
	///用户登录
	virtual int ReqLogin(CQAuthLogin *pField, int nRequestID) = 0;
	
	///用户登出
	virtual int ReqLogout(int nRequestID) = 0;
	
	///用户登录
	virtual int ReqChangePassword(CQAuthChangePassword *pField, int nRequestID) = 0;
	
	///公共资源查询
	virtual int ReqQryPublicResource(CQAuthQryResource *pField, int nRequestID) = 0;
	
	///公共资源下载
	virtual int ReqDownloadPublicResource(CQAuthDownloadResource *pField, int nRequestID) = 0;
	
	///用户资源查询
	virtual int ReqQryPrivateResource(CQAuthQryResource *pField, int nRequestID) = 0;
	
	///用户资源下载
	virtual int ReqDownloadPrivateResource(CQAuthDownloadResource *pField, int nRequestID) = 0;
	
	///用户资源上传
	virtual int ReqUploadPrivateResource(CQAuthResourceInfo *pField, const char *szData, int nRequestID) = 0;

protected:
	virtual ~CQAClientApi(){};
};

///系统初始化
extern int QAuthInit(CQAuthClientInput *pInput, char *szErrMsg);

///创建客户单API对象
extern CQAClientApi * QAuthCreateApi(char *szErrMsg);

///释放系统资源
extern void QAuthRelease();
