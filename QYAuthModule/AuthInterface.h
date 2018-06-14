#ifndef __QYAUTHMODULE_AUTH_INTERFACE_H__
#define __QYAUTHMODULE_AUTH_INTERFACE_H__


/**
 * @class					I_AuthSessionEvent
 * @brief					认证会话事件回调
 * @detail					该类的指针由I_AuthApi接口进行初始化的时候传入dll,供回调使用
 * @author					barry
 * @date					2018/6/14
 */
class I_AuthSessionEvent
{
public:
	/**
	 * @brief				连接建立成功
	 */
	virtual void			OnConnected() = 0;

	/**
	 * @brief				连接断开
	 * @param[in]			nErrorCode<0	表示出错
	 * @param[in]			szErrMsg		错误信息串
	 */
	virtual void			OnDisconnected( int nErrorCode, const char *szErrMsg ) = 0;

	virtual void			OnRspLogin( int nErrorCode, const char *szErrMsg ) = 0;

	/**
	 * @brief				登出响应
	 * @param[in]			nRequestID		请求号
	 * @param[in]			nErrorCode<0	表示出错
	 * @param[in]			szErrMsg		错误信息串
	 */
	virtual void			OnRspLogout( int nRequestID, int nErrorCode, const char *szErrMsg ) = 0;
};


/**
 * @class					I_AuthApi
 * @brief					认证模块管理api接口
 * @detail					该接口由DLL导出
 * @author					barry
 * @date					2018/6/14
 */
class I_AuthApi
{
public:
	/**
	 * @brief				初始化认证库
	 * @param[in]			pEvent			认证会话事件回调接口
	 * @return				0				成功
	 */
	virtual int				Initialize( I_AuthSessionEvent* pEvent ) = 0;

	/**
	 * @brief				释放认证库资源
	 */
	virtual void			Release() = 0;

	/**
	 * @brief				建立连接
	 * @return				<0				出错
	 */
	virtual int				Connect() = 0;

	/**
	 * @brief				断开连接
	 */
	virtual void			Disconnect() = 0;

	/**
	 * @brief				登录服务器进行认证
	 * @param[in]			nReqNo			请求ID
	 * @param[in]			pszUser			用户名
	 * @param[in]			pszPswd			密码
	 * @param[in]			pszExtraPswd	扩展密码
	 * @param[in]			nPswdType		密码类型
	 * @return				<0				出错
	 */
	virtual int				ReqLogin( unsigned int nReqNo, const char* pszUser, const char* pszPswd, const char* pszExtraPswd, unsigned int nPswdType ) = 0;

	/**
	 * @brief				从服务器登出
	 * @param[in]			nReqNo			请求ID
	 * @return				<0				出错
	 */
	virtual int				ReqLogout( unsigned int nReqNo ) = 0;

};


#endif









