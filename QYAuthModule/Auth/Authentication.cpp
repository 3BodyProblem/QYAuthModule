#include "Authentication.h"


///< ------------------------- 认证事件响应类 ---------------------------------------------------------------


void AuthEventCB::OnConnected()
{
	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnConnected() : enter" );

	Authentication::GetAuth().SetStatus( Authentication::ST_Connected );

	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnConnected() : leave" );
}

void AuthEventCB::OnDisconnected( int nErrorCode, const char *szErrMsg )
{
	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnDisconnected() : enter" );

	Authentication::GetAuth().SetStatus( Authentication::ST_Unconnected );

	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnDisconnected() : leave" );
}

void AuthEventCB::OnRspLogin( CQAuthRspInfo* pRspInfo, CQAuthAuthentication* pField, int nRequestID )
{
	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnRspLogin() : enter, reqid=%d", nRequestID );

	if( NULL != pRspInfo ) {
		if( 0 != pRspInfo->ErrorID ) {
			AuthLog::GetLogger().WriteError( "AuthEventCB::OnRspLogin() : error message : %s", pRspInfo->ErrorMsg );
			return;
		}
	}

	Authentication::GetAuth().SetStatus( Authentication::ST_Logined );

	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnRspLogin() : leave" );
}

void AuthEventCB::OnRspLogout( CQAuthRspInfo* pRspInfo, int nRequestID )
{
	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnRspLogout() : enter, reqid=%d", nRequestID );

	if( NULL != pRspInfo ) {
		if( 0 != pRspInfo->ErrorID ) {
			AuthLog::GetLogger().WriteError( "AuthEventCB::OnRspLogout() : error message : %s", pRspInfo->ErrorMsg );
			return;
		}
	}

	Authentication::GetAuth().SetStatus( Authentication::ST_Connected );

	AuthLog::GetLogger().WriteInfo( "AuthEventCB::OnRspLogout() : leave" );
}

void AuthEventCB::OnRspChangePassword( CQAuthRspInfo* pRspInfo, int nRequestID )
{
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

int Authentication::ReqLogin( unsigned int nReqNo, const char* pszUser, const char* pszPswd, const char* pszExtraPswd, unsigned int nPswdType )
{
	CQAuthLogin				tagLogin = { 0 };
	CriticalLock			lock( m_objLock );

	if( NULL == m_pCQAClientApi ) {
		AuthLog::GetLogger().WriteError( "Authentication::ReqLogin() : invalid CQAClientApi* pointer" );
		return -1;
	}

	if( Authentication::ST_Connected == m_eCurStatus ) {
		::memcpy( tagLogin.User, pszUser, ::strlen(pszUser) );						///< 账号
		::memcpy( tagLogin.Password, pszPswd, ::strlen(pszPswd) );					///< 密码
		::memcpy( tagLogin.ExtraPassword, pszExtraPswd, ::strlen(pszExtraPswd) );	///< 扩展密码
		tagLogin.PasswordType = nPswdType;											///< 密码类型

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



