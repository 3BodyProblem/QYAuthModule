#ifndef __QYAUTHMODULE_AUTHENTICATION_H__
#define	__QYAUTHMODULE_AUTHENTICATION_H__


#pragma warning(disable: 4099)
#include "../targetver.h"
#include "../Configuration.h"
#include "../QAuthClientLibs/QAuthClientApi.h"


/**
 * @class					Authentication
 * @brief					����֤��
 * @author					barry
 */
class Authentication
{
public:
	Authentication();
	~Authentication();

	/**
	 * @brief				��ʼ��
	 * @return				0				�ɹ�
	 */
	int						Initialize();

	/**
	 * @brief				�ͷ���Դ
	 */
	void					Release();

protected:
	CQAClientApi*			m_pCQAClientApi;			///<  ��֤���

};


#endif





