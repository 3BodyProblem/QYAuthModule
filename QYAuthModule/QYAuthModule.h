#ifndef __QYAUTHMODULE_ENTRY_H__
#define	__QYAUTHMODULE_ENTRY_H__


/**
 * @brief				DLL导出接口
 * @author				barry
 * @date				2017/4/1
 */
extern "C"
{
	/**
	 * @brief										获取认证模块管理接口指针
	 * @return										返回管理对象的引用
	 */
	__declspec(dllexport) void		__stdcall		GetAuthApi();

	/**
	 * @brief								单元测试导出函数
	 */
	__declspec(dllexport) void		__stdcall		ExecuteUnitTest();
}




#endif





