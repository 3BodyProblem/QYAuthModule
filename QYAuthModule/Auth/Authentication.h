#ifndef __QYAUTHMODULE_AUTHENTICATION_H__
#define	__QYAUTHMODULE_AUTHENTICATION_H__


#pragma warning(disable: 4099)
#include "../targetver.h"
#include "../Configuration.h"
#include "../QAuthClientLibs/QAuthClientApi.h"


/**
 * @class					Authentication
 * @brief					主认证类
 * @author					barry
 */
class Authentication
{
public:
	Authentication();
	~Authentication();

	/**
	 * @brief				初始化
	 * @return				0				成功
	 */
	int						Initialize();

	/**
	 * @brief				释放资源
	 */
	void					Release();

protected:
	CQAClientApi*			m_pCQAClientApi;			///<  认证插件

};


#endif





