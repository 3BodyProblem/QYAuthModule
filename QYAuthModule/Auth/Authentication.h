#ifndef __QYAUTHMODULE_AUTHENTICATION_H__
#define	__QYAUTHMODULE_AUTHENTICATION_H__


#pragma warning(disable: 4099)
#include "../targetver.h"
#include "../AuthInterface.h"
#include "../Configuration.h"
#include "MQWinSecurityPlugin.h"
#include "QWinSecurityPlugin.hpp"
#include "../QAuthClientLibs/QAuthClientApi.h"


class CQWinSecurityInterface
{
public:
	virtual MQWinSecurityPlugin_Interface * GetQWinSecurityPluginApi() = 0;
	virtual bool String_Encrypt(const char * lpIn,string& lpOut,const char * lpKey,char * lpErrorInfo,unsigned int uiErrorSize) = 0;
	virtual bool String_Decrypt(const char * lpIn,string& lpOut,const char * lpKey,char * lpErrorInfo,unsigned int uiErrorSize, int minLen_Ciphertext = 12) = 0;
	virtual int PBKDF2_SHA256(const char * lpIn, const char * lpKey, int num, char * lpOut, int outlen, char * lpErrorInfo,unsigned int uiErrorSize) = 0;
};


class CQWinSecurity : public CQWinSecurityInterface
{
public:
	CQWinSecurity()
	{
		m_Inited = false;
	};
	~CQWinSecurity()
	{
		Release();
	};

	bool Init();
	void Release();

	MQWinSecurityPlugin_Interface * GetQWinSecurityPluginApi();

	bool String_Encrypt(const char * lpIn, string& lpOut,const char * lpKey,char * lpErrorInfo,unsigned int uiErrorSize);
	bool String_Decrypt(const char * lpIn, string& lpOut,const char * lpKey,char * lpErrorInfo,unsigned int uiErrorSize, int minLen_Ciphertext = 12);
	int PBKDF2_SHA256(const char * lpIn, const char * lpKey, int num, char * lpOut, int outlen, char * lpErrorInfo,unsigned int uiErrorSize);

	MQWinSecurityPlugin m_MQWinSecurity;

	bool m_Inited;
};


/**
 * @class					AuthEventCB
 * @brief					��֤��Ӧ�ص���
 * @author					barry
 */
class AuthEventCB : public CQAClientSpi
{
public:
	/**
	 * @brief				���ӳɹ��¼�
	 */
	void OnConnected();

	/**
	 * @brief				���ӶϿ��¼�
	 */
	void OnDisconnected( int nErrorCode, const char *szErrMsg );

	/**
	 * @brief				��¼��Ӧ�¼�
	 */
	void OnRspLogin( CQAuthRspInfo* pRspInfo, CQAuthAuthentication* pField, int nRequestID );

	/**
	 * @brief				�ǳ���Ӧ�¼�
	 */
	void OnRspLogout( CQAuthRspInfo* pRspInfo, int nRequestID );

	/**
	 * @brief				�޸�������Ӧ�¼�
	 */
	void OnRspChangePassword( CQAuthRspInfo* pRspInfo, int nRequestID );

	/**
	 * @brief				������Դ��ѯ��Ӧ�¼�
	 */
	void OnRspQryPublicResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, int nRequestID, bool isLast );

	/**
	 * @brief				������Դ������Ӧ�¼�
	 */
	void OnRspDownloadPublicResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, const char* szData, int nRequestID );

	/**
	 * @brief				�û���Դ��ѯ��Ӧ�¼�
	 */
	void OnRspQryPrivateResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, int nRequestID, bool isLast );

	/**
	 * @brief				�û���Դ������Ӧ�¼�
	 */
	void OnRspDownloadPrivateResource( CQAuthRspInfo* pRspInfo, CQAuthResourceInfo* pField, const char* szData, int nRequestID );

	/**
	 * @brief				�û���Դ������Ӧ�¼�
	 */
	void OnRspUploadPrivateResource( CQAuthRspInfo* pRspInfo, int nRequestID );

	/**
	 * @brief				������Ϣ
	 */
	void OnRtnMessage( CQAuthMessage* pField );
};


/**
 * @class					Authentication
 * @brief					����֤��
 * @author					barry
 */
class Authentication : public I_AuthApi
{
public:
	typedef enum
	{
		ST_Unconnected = 0,		///< δ����
		ST_Connected,			///< ������
		ST_Logined,				///< �ѵ�¼
		ST_Error = 100,			///< ������
	} EnumModuleStatus;
private:
	Authentication();
public:
	~Authentication();

	/**
	 * @brief				��ȡ����
	 */
	static Authentication&	GetAuth();

	/**
	 * @brief				��ʼ��
	 * @param[in]			pEvent			��֤�Ự�ص��ӿ�
	 * @return				0				�ɹ�
	 */
	int						Initialize( I_AuthSessionEvent* pEvent );

	/**
	 * @brief				�ͷ���Դ
	 */
	void					Release();

	/**
	 * @brief				��������
	 * @return				<0				����
	 */
	int						Connect();

	/**
	 * @brief				�Ͽ�����
	 */
	void					Disconnect();

public:
	/**
	 * @brief				��ȡģ�鵱ǰ״̬
	 */
	EnumModuleStatus		GetStatus();

	/**
	 * @brief				����ģ�鵱ǰ״̬
	 */
	void					SetStatus( EnumModuleStatus eStatus );

	/**
	 * @brief				��ȡ�ص�ָ��
	 */
	I_AuthSessionEvent*		operator->();

public:
	/**
	 * @brief				��¼������������֤
	 * @param[in]			nReqNo			����ID
	 * @param[in]			pszUser			�û���
	 * @param[in]			pszPswd			����
	 * @param[in]			pszExtraPswd	��չ����
	 * @param[in]			nPswdType		��������
	 * @return				<0				����
							>=0				���������(RequestID)
	 */
	int						ReqLogin( unsigned int nReqNo, const char* pszUser, const char* pszPswd, const char* pszExtraPswd, unsigned int nPswdType );

	/**
	 * @brief				�ӷ������ǳ�
	 * @param[in]			nReqNo			����ID
	 * @return				<0				����
	 */
	int						ReqLogout( unsigned int nReqNo );

	/**
	 * @brief				�ĵ�¼����
	 * @param[in]			nReqNo			����ID
	 * @param[in]			pszUserID		�û��ʺ�
	 * @param[in]			pszOldPswd		������
	 * @param[in]			pszNewPswd		������
	 * @param[in]			nPswdType		��������
	 * @return				<0				����
	 */
	int						ReqChgPassword( unsigned int nReqNo, const char* pszUserID, const char* pszOldPswd, const char* pszNewPswd, unsigned int nPswdType );

protected:
	/**
	 * @brief				���õ�¼����
	 */
	void					SetAccount( const char* User, const char* Password );

protected:
	EnumModuleStatus		m_eCurStatus;				///< ģ�鵱ǰ״̬

protected:
	CriticalObject			m_objLock;					///< �ٽ���
	CQAClientApi*			m_pCQAClientApi;			///< ��֤���
	AuthEventCB				m_objEventCallBack;			///< ��֤�¼��ص�
	I_AuthSessionEvent*		m_pUserNotify;				///< �û�֪ͨ
protected:
	std::string				m_sLoginUser;				///< ��¼��
	std::string				m_sLoginPswd;				///< ��¼����
	std::string				m_sLoginMd5Pwd;				///< ��¼MD5����
};


#endif





