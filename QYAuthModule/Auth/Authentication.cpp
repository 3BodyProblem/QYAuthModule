#include "Authentication.h"
#include "../Infrastructure/CheckCode.h"


CQWinSecurity		g_QWinSecurity;


bool CQWinSecurity::Init()
{
	if(!m_Inited)
	{
		std::string			sPath;
		int					nErrCode = 0;
		char				pszTmp[1024*8] = { 0 };
		char				errmsg[256] = { 0 };

		::GetModuleFileName( g_oModule, pszTmp, sizeof(pszTmp) );
		sPath = pszTmp;
		sPath = sPath.substr( 0, sPath.find("QYAuthModule.") ) + "QWinSecurityPlugin.dll";

		nErrCode = m_MQWinSecurity.Instance( sPath.c_str(), errmsg, 256);
		m_Inited = (1 == nErrCode) ? true : false;
	}

	return m_Inited;
}

void CQWinSecurity::Release()
{
	m_MQWinSecurity.Release();
}

MQWinSecurityPlugin_Interface * CQWinSecurity::GetQWinSecurityPluginApi()
{
	if(Init())
	{
		return m_MQWinSecurity.GetQWinSecurityPluginApi(QWINSECURITYPLUGIN_VERSION);
	}

	return NULL;
}

bool CQWinSecurity::String_Encrypt(const char * lpIn, string& lpOut,const char * lpKey,char * lpErrorInfo,unsigned int uiErrorSize)
{
	bool ret = false;
	lpOut = "";

	MQWinSecurityPlugin_Interface* pMQWinSecurity = GetQWinSecurityPluginApi();

	if(pMQWinSecurity != NULL)
	{
		int len = max(256, strlen(lpIn) * 5 + 1);
		char* chrout = new char[len];
		memset(chrout, 0, len);

		ret = pMQWinSecurity->String_Encrypt(lpIn, chrout, len, lpKey, lpErrorInfo, uiErrorSize);

		lpOut = chrout;

		memset(chrout, 0, len);
		delete []chrout;
	}

	return ret;
}

bool CQWinSecurity::String_Decrypt(const char * lpIn, string& lpOut,const char * lpKey,char * lpErrorInfo,unsigned int uiErrorSize, int minLen_Ciphertext)
{
	bool ret = false;
	lpOut = "";

	int len = strlen(lpIn);

	if(len <= minLen_Ciphertext)
	{
		lpOut = lpIn;
		return true;
	}

	MQWinSecurityPlugin_Interface* pMQWinSecurity = GetQWinSecurityPluginApi();

	if(pMQWinSecurity != NULL)
	{
		len = len * 5 + 1;
		char* chrout = new char[len];
		memset(chrout, 0, len);

		ret = pMQWinSecurity->String_Decrypt(lpIn, chrout, len, lpKey, lpErrorInfo, uiErrorSize);

		lpOut = chrout;

		memset(chrout, 0, len);
		delete []chrout;
	}

	return ret;
}

int CQWinSecurity::PBKDF2_SHA256(const char * lpIn, const char * lpKey, int num, char * lpOut, int outlen, char * lpErrorInfo,unsigned int uiErrorSize)
{
	int ret = 0;

	MQWinSecurityPlugin_Interface* pMQWinSecurity = GetQWinSecurityPluginApi();

	if(pMQWinSecurity != NULL)
	{
		ret = pMQWinSecurity->PBKDF2_SHA256(lpIn, lpKey, strlen(lpKey), num, lpOut, outlen, lpErrorInfo, uiErrorSize);
	}

	return ret;
}

CQWinSecurityInterface* GetQWinSecurityInterface()
{
	return &g_QWinSecurity;
}

///< ------------------------- 认证事件响应类 ---------------------------------------------------------------


void AuthEventCB::OnConnected()
{
	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnConnected() : enter" );

	Authentication::GetAuth().SetStatus( Authentication::ST_Connected );
	Authentication::GetAuth()->OnConnected();

	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnConnected() : leave" );
}

void AuthEventCB::OnDisconnected( int nErrorCode, const char *szErrMsg )
{
	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnDisconnected() : enter" );

	Authentication::GetAuth().SetStatus( Authentication::ST_Unconnected );
	Authentication::GetAuth()->OnDisconnected( nErrorCode, szErrMsg );

	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnDisconnected() : leave" );
}

void AuthEventCB::OnRspLogin( CQAuthRspInfo* pRspInfo, CQAuthAuthentication* pField, int nRequestID )
{
	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnRspLogin() : enter, reqid=%d", nRequestID );

	if( NULL != pRspInfo ) {
		if( 0 != pRspInfo->ErrorID ) {
			AuthLog::GetLogger().WriteError( "AuthEventCB::OnRspLogin() : error message : %s", pRspInfo->ErrorMsg );
			Authentication::GetAuth()->OnRspLogin( nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg );
			return;
		}
	}

	Authentication::GetAuth().SetStatus( Authentication::ST_Logined );
	Authentication::GetAuth()->OnRspLogin( nRequestID, 0, NULL );

	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnRspLogin() : leave" );
}

void AuthEventCB::OnRspLogout( CQAuthRspInfo* pRspInfo, int nRequestID )
{
	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnRspLogout() : enter, reqid=%d", nRequestID );

	if( NULL != pRspInfo ) {
		if( 0 != pRspInfo->ErrorID ) {
			AuthLog::GetLogger().WriteError( "AuthEventCB::OnRspLogout() : error message : %s", pRspInfo->ErrorMsg );
			Authentication::GetAuth()->OnRspLogout( nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg );
			return;
		}
	}

	Authentication::GetAuth().SetStatus( Authentication::ST_Connected );
	Authentication::GetAuth()->OnRspLogout( nRequestID, 0, NULL );

	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnRspLogout() : leave" );
}

void AuthEventCB::OnRspChangePassword( CQAuthRspInfo* pRspInfo, int nRequestID )
{
	Authentication::GetAuth()->OnRspChangePassword( nRequestID, pRspInfo->ErrorID, pRspInfo->ErrorMsg );
}

void AuthEventCB::OnRspQryPublicResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, int nRequestID, bool isLast )
{
}

void AuthEventCB::OnRspDownloadPublicResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, const char* szData, int nRequestID )
{
}

void AuthEventCB::OnRspQryPrivateResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, int nRequestID, bool isLast )
{
}

void AuthEventCB::OnRspDownloadPrivateResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, const char* szData, int nRequestID )
{
}

void AuthEventCB::OnRspUploadPrivateResource( CQAuthRspInfo* pRspInfo, int nRequestID )
{
}

void AuthEventCB::OnRtnMessage( CQAuthMessage* pField )
{
}


///< ------------------------- 认证管理类 ---------------------------------------------------------------


Authentication::Authentication()
 : m_pCQAClientApi( NULL ), m_eCurStatus( ST_Unconnected )
 , m_pUserNotify( NULL )
{
}

Authentication::~Authentication()
{
	Release();
}

Authentication&	Authentication::GetAuth()
{
	static Authentication	obj;

	return obj;
}

int Authentication::Initialize( I_AuthSessionEvent* pEvent )
{
	CQAuthClientInput		objInput;						///< 输入参数
	int						nErrorCode = 0;					///< 错误码
	char					pszErrMsg[1024*8] = { 0 };		///< 输出错误信息
	CriticalLock			lock( m_objLock );

	AuthLog::GetLogger().WriteInfo( "Authentication::Initialize() : enter ..." );
	if( g_QWinSecurity.Init() ) {
		AuthLog::GetLogger().WriteError( "Authentication::Initialize() : cannot initialize security module." );
		return -98;
	}

	if( 0 != Configuration::GetConfig().Initialize() ) {
		AuthLog::GetLogger().WriteError( "Authentication::Initialize() : cannot load configuration." );
		return -99;
	}

	Release();
	m_pUserNotify = pEvent;									///< 设置用户事件通知接口
	Configuration::GetConfig().FetchAuthConfig( objInput );	///< 获取初始化配置
	if( NULL == m_pUserNotify ) {
		AuthLog::GetLogger().WriteError( "Authentication::Initialize() : invalid user notify interface ptr." );
		return -100;
	}

	nErrorCode = ::QAuthInit( &objInput, pszErrMsg );		///< 初始化认证库
	if( nErrorCode < 0 ) {
		AuthLog::GetLogger().WriteError( "Authentication::Initialize() : cannot initialize QAuthLib, error msg : %s", pszErrMsg );
		return nErrorCode;
	}

	::memset( pszErrMsg, 0, sizeof(pszErrMsg) );			///< 创建认证控制对象
	m_pCQAClientApi = ::QAuthCreateApi( pszErrMsg );
	if( NULL == m_pCQAClientApi ) {
		AuthLog::GetLogger().WriteError( "Authentication::Initialize() : cannot grab CQAClientApi* pointer, error msg : %s", pszErrMsg );
		return -100;
	}

	m_pCQAClientApi->RegisterSpi( &m_objEventCallBack );	///< 注册回调事件接口
	AuthLog::GetLogger().WriteInfo( "Authentication::Initialize() : leave ..." );

	return nErrorCode;
}

void Authentication::Release()
{
	CriticalLock			lock( m_objLock );

	m_pUserNotify = NULL;
	m_eCurStatus = ST_Unconnected;

	if( NULL != m_pCQAClientApi ) {
		::QAuthRelease();									///< 释放认证模块资源
		m_pCQAClientApi = NULL;								///< 清空认证模块控制对象
	}
}

I_AuthSessionEvent* Authentication::operator->()
{
	return m_pUserNotify;
}

void Authentication::SetStatus( Authentication::EnumModuleStatus eStatus )
{
	CriticalLock			lock( m_objLock );

	m_eCurStatus = eStatus;
}

Authentication::EnumModuleStatus Authentication::GetStatus()
{
	CriticalLock			lock( m_objLock );

	return m_eCurStatus;
}

int Authentication::Connect()
{
	CriticalLock			lock( m_objLock );

	if( NULL == m_pCQAClientApi ) {
		AuthLog::GetLogger().WriteError( "Authentication::Connect() : invalid CQAClientApi* pointer" );
		return -100;
	}

	return m_pCQAClientApi->Connect();
}

void Authentication::Disconnect()
{
	CriticalLock			lock( m_objLock );

	if( NULL == m_pCQAClientApi ) {
		AuthLog::GetLogger().WriteError( "Authentication::Disconnect() : invalid CQAClientApi* pointer" );
		return;
	}

	m_pCQAClientApi->Disconnect();
}

void Bytes2XStr(const char* bytes, int count, char* pstr)
{
	pstr[0] = 0;

	char* p = pstr;
	int i = 0;
	for(i=0; i<count; i++,p+=2)
	{
		sprintf(p, "%02x", (unsigned char)bytes[i]);
	}
}

void Authentication::SetAccount( const char* User, const char* Password )
{
	MD5							oMD5;
	char						szMd5[16] = { 0 };
	char						szMd5Password[33] = { 0 };
	//char						szMd5_2[16] = { 0 };
	char						outStr[64] = { 0 };
	char						errMsgA[256] = { 0 };
	char						ExtraPassword[256] = { 0 };
	CQWinSecurityInterface*		pQWinSecurity = GetQWinSecurityInterface();

	pQWinSecurity->PBKDF2_SHA256(Password, "http://www.iqwin.com.cn", 5, outStr, 32, errMsgA, 256);
	Bytes2XStr(outStr, 30, ExtraPassword);
	ExtraPassword[60] = 0;

	m_sLoginUser = User;
	m_sLoginPswd = ExtraPassword;
	oMD5.CalMD5( Password, strlen(Password), szMd5 );
	//oMD5.CalMD5("88888888", strlen("88888888"), szMd5_2);
	MD5::ConvertMD5ToStr(szMd5, szMd5Password, sizeof(szMd5Password));
	m_sLoginMd5Pwd = szMd5Password;
}


int Authentication::ReqLogin( unsigned int nReqNo, const char* pszUser, const char* pszPswd, const char* pszExtraPswd, unsigned int nPswdType )
{
	CQAuthLogin				tagLogin = { 0 };
	CriticalLock			lock( m_objLock );

	if( NULL == m_pCQAClientApi ) {
		AuthLog::GetLogger().WriteError( "Authentication::ReqLogin() : invalid CQAClientApi* pointer" );
		return -1;
	}

	SetAccount( pszUser, pszPswd );

	if( Authentication::ST_Connected == m_eCurStatus ) {
		::strcpy( tagLogin.User, m_sLoginUser.c_str() );
		::strcpy( tagLogin.ExtraPassword, m_sLoginPswd.c_str() );
		::strcpy( tagLogin.Password, m_sLoginMd5Pwd.c_str() );
		tagLogin.PasswordType = QAUTH_PWDTYPE_MD5;

		return m_pCQAClientApi->ReqLogin( &tagLogin, nReqNo );
	}

	return -100;
}

int Authentication::ReqLogout( unsigned int nReqNo )
{
	CriticalLock			lock( m_objLock );

	if( NULL == m_pCQAClientApi ) {
		AuthLog::GetLogger().WriteError( "Authentication::ReqLogout() : invalid CQAClientApi* pointer" );
		return -1;
	}

	if( Authentication::ST_Logined == m_eCurStatus ) {
		return m_pCQAClientApi->ReqLogout( nReqNo );
	}

	return -100;
}

int Authentication::ReqChgPassword( unsigned int nReqNo, const char* pszUserID, const char* pszOldPswd, const char* pszNewPswd, unsigned int nPswdType )
{
	CriticalLock			lock( m_objLock );

	if( NULL == m_pCQAClientApi ) {
		AuthLog::GetLogger().WriteError( "Authentication::ReqChgPassword() : invalid CQAClientApi* pointer" );
		return -1;
	}

	if( Authentication::ST_Logined == m_eCurStatus ) {
		CQAuthChangePassword	tagChgPswd = { 0 };

		::strncpy( tagChgPswd.User, pszUserID, ::strlen(pszUserID) );
		::strncpy( tagChgPswd.OldPassword, pszOldPswd, ::strlen(pszOldPswd) );
		::strncpy( tagChgPswd.NewPassword, pszNewPswd, ::strlen(pszNewPswd) );
		tagChgPswd.PasswordType = nPswdType;
		return m_pCQAClientApi->ReqLogout( nReqNo );
	}

	return -100;
}



