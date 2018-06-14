#ifndef __QYAUTHMODULE_CONFIGURATION_H__
#define __QYAUTHMODULE_CONFIGURATION_H__


#pragma warning(disable: 4996)
#pragma warning(disable: 4786)
#include <string>
#include <fstream>
#include "Infrastructure/Lock.h"
#include "QAuthClientLibs/QAuthClientApi.h"


/**
 * @class						AuthLog
 * @brief						日志类
 * @author						barry
 */
class AuthLog : public MLogIOInterface
{
public:
	typedef	enum	EnumLogLevel
	{
		LV_INFO = 0,			///< 普通日志
		LV_WARN,				///< 警告日志
		LV_ERR,					///< 错误日志
	};
private:
	AuthLog();

public:
	/**
	 * @brief					获取单健
	 */
	static AuthLog&				GetLogger();

public:
	/**
	 * @brief					输出信息
	 */
	virtual void				WriteInfo( const char* szFormat, ... );

	/**
	 * @brief					输出警告
	 */
	virtual void				WriteWarning( const char* szFormat, ... );

	/**
	 * @brief					输出错误
	 */
	virtual void				WriteError( const char* szFormat, ... );

protected:
	CriticalObject				m_objLock;				///< 临界区
	std::ofstream				m_objLogFile;			///< 日志文件
};


extern	HMODULE					g_oModule;						///< 当前dll模块变量


typedef struct
{
	unsigned int				m_nClientType;					///< 客户端类型
	unsigned int				m_nClientVersion;				///< 客户端版本，主版本、子版本、Build号，32位各用8、12、12位
	unsigned int				m_nCustomerID;					///< 券商ID
	unsigned int				m_nServerCount;					///< 认证服务器数量
	CQAuthServerConfig			m_vctServers[AUTHSERVER_MAX];	///< 认证服务器配置，最多8组
	bool						m_bIsUseSSL;					///< 是否使用SSL
	char						m_pszCrtFilename[256];			///< Crt证书
	char						m_pszPfxFilename[256];			///< Pfx证书
	char						m_pszPfxPassword[32];			///< Pfx证书密码
	unsigned int				m_nNetworkTimeOut;				///< 网络通讯超时，单位：秒
	unsigned int				m_nHeartbeatInterval;			///< 心跳间隔，单位：秒
	bool						m_nbIsOutputDebugLog;			///< 调试日志
} T_Auth_Cfg;					///< 认证模块初始化参数结构


typedef struct
{
	char						m_pszUser[64];					///< 账号
	char						m_pszPassword[64];				///< 密码
	unsigned int				m_nPasswordType;				///< 密码类型
	char						m_pszExtraPassword[64];			///< 扩展密码
} T_Login_Cfg;

/**
 * @class						Configuration
 * @brief						配置信息
 * @date						2018/6/12
 * @author						barry
 */
class Configuration
{
protected:
	Configuration();

public:
	/**
	 * @brief					获取配置对象的单键引用对象
	 */
	static Configuration&		GetConfig();

	/**
	 * @brief					加载配置项
	 * @return					==0					成功
								!=					出错
	 */
    int							Initialize();

	/**
	 * @brief					释放资源
	 */
	void						Release();

public:
	/**
	 * @brief					获取认证插件初始化配置信息
	 * @param[out]				objCfg					获取更新配置信息结构
	 */
	void						FetchAuthConfig( CQAuthClientInput& objCfg );

protected:
	T_Auth_Cfg					m_tagAuthConfig;				///< 认证模块初始化参数结构
};


#endif









