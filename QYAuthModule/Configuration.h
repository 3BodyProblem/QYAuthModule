#ifndef __QYAUTHMODULE_CONFIGURATION_H__
#define __QYAUTHMODULE_CONFIGURATION_H__


#pragma warning(disable: 4786)
#include <vector>
#include <string>
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









