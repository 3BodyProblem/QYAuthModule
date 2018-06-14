#ifndef __QYAUTHMODULE_AUTHENTICATION_H__
#define	__QYAUTHMODULE_AUTHENTICATION_H__


#pragma warning(disable: 4099)
#include "../targetver.h"
#include "../AuthInterface.h"
#include "../Configuration.h"
#include "../QAuthClientLibs/QAuthClientApi.h"


/**
 * @class					AuthEventCB
 * @brief					认证响应回调类
 * @author					barry
 */
class AuthEventCB : public CQAClientSpi
{
public:
	/**
	 * @brief				连接成功事件
	 */
	void OnConnected();

	/**
	 * @brief				连接断开事件
	 */
	void OnDisconnected( int nErrorCode, const char *szErrMsg );

	/**
	 * @brief				登录响应事件
	 */
	void OnRspLogin( CQAuthRspInfo* pRspInfo, CQAuthAuthentication* pField, int nRequestID );

	/**
	 * @brief				登出响应事件
	 */
	void OnRspLogout( CQAuthRspInfo* pRspInfo, int nRequestID );

	/**
	 * @brief				修改密码响应事件
	 */
	void OnRspChangePassword( CQAuthRspInfo* pRspInfo, int nRequestID );

	/**
	 * @brief				公共资源查询响应事件
	 */
	void OnRspQryPublicResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, int nRequestID, bool isLast );

	/**
	 * @brief				公共资源下载响应事件
	 */
	void OnRspDownloadPublicResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, const char* szData, int nRequestID );

	/**
	 * @brief				用户资源查询响应事件
	 */
	void OnRspQryPrivateResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, int nRequestID, bool isLast );

	/**
	 * @brief				用户资源下载响应事件
	 */
	void OnRspDownloadPrivateResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, const char* szData, int nRequestID );

	/**
	 * @brief				用户资源下载响应事件
	 */
	void OnRspUploadPrivateResource( CQAuthRspInfo* pRspInfo, int nRequestID );

	/**
	 * @brief				推送消息
	 */
	void OnRtnMessage( CQAuthMessage* pField );
};


/**
 * @class					Authentication
 * @brief					主认证类
 * @author					barry
 */
class Authentication : public I_AuthApi
{
public:
	typedef enum
	{
		ST_Unconnected = 0,		///< 未连接
		ST_Connected,			///< 已连接
		ST_Logined,				///< 已登录
		ST_Error = 100,			///< 出错了
	} EnumModuleStatus;
private:
	Authentication();
public:
	~Authentication();

	/**
	 * @brief				获取单健
	 */
	static Authentication&	GetAuth();

	/**
	 * @brief				初始化
	 * @param[in]			pEvent			认证会话回调接口
	 * @return				0				成功
	 */
	int						Initialize( I_AuthSessionEvent* pEvent );

	/**
	 * @brief				释放资源
	 */
	void					Release();

	/**
	 * @brief				建立连接
	 * @return				<0				出错
	 */
	int						Connect();

	/**
	 * @brief				断开连接
	 */
	void					Disconnect();

public:
	/**
	 * @brief				获取模块当前状态
	 */
	EnumModuleStatus		GetStatus();

	/**
	 * @brief				设置模块当前状态
	 */
	void					SetStatus( EnumModuleStatus eStatus );

public:
	/**
	 * @brief				登录服务器进行认证
	 * @param[in]			nReqNo			请求ID
	 * @param[in]			pszUser			用户名
	 * @param[in]			pszPswd			密码
	 * @param[in]			pszExtraPswd	扩展密码
	 * @param[in]			nPswdType		密码类型
	 * @return				<0				出错
							>=0				返回请求号(RequestID)
	 */
	int						ReqLogin( unsigned int nReqNo, const char* pszUser, const char* pszPswd, const char* pszExtraPswd, unsigned int nPswdType );

	/**
	 * @brief				从服务器登出
	 * @param[in]			nReqNo			请求ID
	 * @return				<0				出错
	 */
	int						ReqLogout( unsigned int nReqNo );

	/**
	 * @brief				改登录密码
	 * @param[in]			nReqNo			请求ID
	 * @param[in]			pszUserID		用户帐号
	 * @param[in]			pszOldPswd		旧密码
	 * @param[in]			pszNewPswd		新密码
	 * @param[in]			nPswdType		密码类型
	 * @return				<0				出错
	 */
	int						ReqChgPassword( unsigned int nReqNo, const char* pszUserID, const char* pszOldPswd, const char* pszNewPswd, unsigned int nPswdType );

protected:
	EnumModuleStatus		m_eCurStatus;				///< 模块当前状态

protected:
	CriticalObject			m_objLock;					///< 临界区
	CQAClientApi*			m_pCQAClientApi;			///< 认证插件
	AuthEventCB				m_objEventCallBack;			///< 认证事件回调
	I_AuthSessionEvent*		m_pUserNotify;				///< 用户通知
};


#endif





