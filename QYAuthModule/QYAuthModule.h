#ifndef __QYAUTHMODULE_ENTRY_H__
#define	__QYAUTHMODULE_ENTRY_H__


/**
 * @brief				DLL�����ӿ�
 * @author				barry
 * @date				2017/4/1
 */
extern "C"
{
	/**
	 * @brief								ȡ�ù�����������,���ڴ���/����/�ջ��ڴ����ݿ����
	 * @return								���ع�����������
	 */
	__declspec(dllexport) void		__stdcall		GetFactoryObject();

	/**
	 * @brief								��Ԫ���Ե�������
	 */
	__declspec(dllexport) void		__stdcall		ExecuteUnitTest();
}




#endif





