#ifndef __QYAUTHMODULE_AUTH_INTERFACE_H__
#define __QYAUTHMODULE_AUTH_INTERFACE_H__


/**
 * @class					I_AuthSessionEvent
 * @brief					��֤�Ự�¼��ص�
 * @detail					�����ָ����I_AuthApi�ӿڽ��г�ʼ����ʱ����dll,���ص�ʹ��
 * @author					barry
 * @date					2018/6/14
 */
class I_AuthSessionEvent
{
public:
	/**
	 * @brief				���ӽ����ɹ�
	 */
	virtual void			OnConnected() = 0;

	/**
	 * @brief				���ӶϿ�
	 * @param[in]			nErrorCode<0	��ʾ����
	 * @param[in]			szErrMsg		������Ϣ��
	 */
	virtual void			OnDisconnected( int nErrorCode, const char *szErrMsg ) = 0;

	/**
	 * @brief				��¼��Ӧ
	 * @param[in]			nRequestID		�����
	 * @param[in]			nErrorCode<0	��ʾ����
	 * @param[in]			szErrMsg		������Ϣ��
	 */
	virtual void			OnRspLogin( int nRequestID, int nErrorCode, const char *szErrMsg ) = 0;

	/**
	 * @brief				�ǳ���Ӧ
	 * @param[in]			nRequestID		�����
	 * @param[in]			nErrorCode<0	��ʾ����
	 * @param[in]			szErrMsg		������Ϣ��
	 */
	virtual void			OnRspLogout( int nRequestID, int nErrorCode, const char *szErrMsg ) = 0;

	/**
	 * @brief				�޸�������Ӧ
	 * @param[in]			nRequestID		�����
	 * @param[in]			nErrorCode<0	��ʾ����
	 * @param[in]			szErrMsg		������Ϣ��
	 */
	virtual void			OnRspChangePassword( int nRequestID, int nErrorCode, const char *szErrMsg ) = 0;
};


///< ------------------------------------------------------------------------------------------------------------
///< ����API����
// __declspec(dllexport) I_AuthApi*		GetSingletonAuthApi();


/**
 * @class					I_AuthApi
 * @brief					��֤ģ�����api�ӿ�
 * @detail					�ýӿ���DLL����
 * @author					barry
 * @date					2018/6/14
 */
class I_AuthApi
{
public:
	/**
	 * @brief				��ʼ����֤��
	 * @param[in]			pEvent			��֤�Ự�¼��ص��ӿ�
	 * @return				0				�ɹ�
	 */
	virtual int				Initialize( I_AuthSessionEvent* pEvent ) = 0;

	/**
	 * @brief				�ͷ���֤����Դ
	 */
	virtual void			Release() = 0;

	/**
	 * @brief				��������
	 * @detail				���Ӳ����������ļ�(.ini)
	 * @return				<0				����
	 */
	virtual int				Connect() = 0;

	/**
	 * @brief				�Ͽ�����
	 */
	virtual void			Disconnect() = 0;

	/**
	 * @brief				��¼������������֤
	 * @param[in]			nReqNo			����ID
	 * @param[in]			pszUser			�û���
	 * @param[in]			pszPswd			����
	 * @param[in]			pszExtraPswd	��չ����
	 * @param[in]			nPswdType		��������
	 * @return				<0				����
	 */
	virtual int				ReqLogin( unsigned int nReqNo, const char* pszUser, const char* pszPswd, const char* pszExtraPswd, unsigned int nPswdType ) = 0;

	/**
	 * @brief				�ӷ������ǳ�
	 * @param[in]			nReqNo			����ID
	 * @return				<0				����
	 */
	virtual int				ReqLogout( unsigned int nReqNo ) = 0;

	/**
	 * @brief				�޸ĵ�¼����
	 * @param[in]			nReqNo			����ID
	 * @param[in]			pszUserID		�ʺ���
	 * @param[in]			pszOldPswd		������
	 * @param[in]			pszNewPswd		������
	 * @param[in]			nPswdType		��������
	 * @return				<0				����
	 */
	virtual int				ReqChgPassword( unsigned int nReqNo, const char* pszUserID, const char* pszOldPswd, const char* pszNewPswd, unsigned int nPswdType ) = 0;

};


#endif









