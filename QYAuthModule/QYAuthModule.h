#ifndef __QYAUTHMODULE_ENTRY_H__
#define	__QYAUTHMODULE_ENTRY_H__


#include "Auth\Authentication.h"


/**
 * @brief				DLL�����ӿ�
 * @author				barry
 * @date				2017/4/1
 */
extern "C"
{
	/**
	 * @brief										��ȡ��֤ģ�����ӿ�ָ��
	 * @return										���ع�����������
	 */
	__declspec(dllexport) I_AuthApi*				GetSingletonAuthApi();
}




#endif





