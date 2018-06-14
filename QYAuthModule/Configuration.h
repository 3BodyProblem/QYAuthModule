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
 * @brief						��־��
 * @author						barry
 */
class AuthLog : public MLogIOInterface
{
public:
	typedef	enum	EnumLogLevel
	{
		LV_INFO = 0,			///< ��ͨ��־
		LV_WARN,				///< ������־
		LV_ERR,					///< ������־
	};
private:
	AuthLog();

public:
	/**
	 * @brief					��ȡ����
	 */
	static AuthLog&				GetLogger();

public:
	/**
	 * @brief					�����Ϣ
	 */
	virtual void				WriteInfo( const char* szFormat, ... );

	/**
	 * @brief					�������
	 */
	virtual void				WriteWarning( const char* szFormat, ... );

	/**
	 * @brief					�������
	 */
	virtual void				WriteError( const char* szFormat, ... );

protected:
	CriticalObject				m_objLock;				///< �ٽ���
	std::ofstream				m_objLogFile;			///< ��־�ļ�
};


extern	HMODULE					g_oModule;						///< ��ǰdllģ�����


typedef struct
{
	unsigned int				m_nClientType;					///< �ͻ�������
	unsigned int				m_nClientVersion;				///< �ͻ��˰汾�����汾���Ӱ汾��Build�ţ�32λ����8��12��12λ
	unsigned int				m_nCustomerID;					///< ȯ��ID
	unsigned int				m_nServerCount;					///< ��֤����������
	CQAuthServerConfig			m_vctServers[AUTHSERVER_MAX];	///< ��֤���������ã����8��
	bool						m_bIsUseSSL;					///< �Ƿ�ʹ��SSL
	char						m_pszCrtFilename[256];			///< Crt֤��
	char						m_pszPfxFilename[256];			///< Pfx֤��
	char						m_pszPfxPassword[32];			///< Pfx֤������
	unsigned int				m_nNetworkTimeOut;				///< ����ͨѶ��ʱ����λ����
	unsigned int				m_nHeartbeatInterval;			///< �����������λ����
	bool						m_nbIsOutputDebugLog;			///< ������־
} T_Auth_Cfg;					///< ��֤ģ���ʼ�������ṹ


typedef struct
{
	char						m_pszUser[64];					///< �˺�
	char						m_pszPassword[64];				///< ����
	unsigned int				m_nPasswordType;				///< ��������
	char						m_pszExtraPassword[64];			///< ��չ����
} T_Login_Cfg;

/**
 * @class						Configuration
 * @brief						������Ϣ
 * @date						2018/6/12
 * @author						barry
 */
class Configuration
{
protected:
	Configuration();

public:
	/**
	 * @brief					��ȡ���ö���ĵ������ö���
	 */
	static Configuration&		GetConfig();

	/**
	 * @brief					����������
	 * @return					==0					�ɹ�
								!=					����
	 */
    int							Initialize();

	/**
	 * @brief					�ͷ���Դ
	 */
	void						Release();

public:
	/**
	 * @brief					��ȡ��֤�����ʼ��������Ϣ
	 * @param[out]				objCfg					��ȡ����������Ϣ�ṹ
	 */
	void						FetchAuthConfig( CQAuthClientInput& objCfg );

protected:
	T_Auth_Cfg					m_tagAuthConfig;				///< ��֤ģ���ʼ�������ṹ
};


#endif









