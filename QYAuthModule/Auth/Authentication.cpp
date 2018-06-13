#include "Authentication.h"


Authentication::Authentication()
 : m_pCQAClientApi( NULL )
{
}

Authentication::~Authentication()
{
	Release();
}

int Authentication::Initialize()
{
	CQAuthClientInput		objInput;						///< 输入参数
	int						nErrorCode = 0;					///< 错误码
	char					pszErrMsg[1024*8] = { 0 };		///< 输出错误信息

	Release();
	Configuration::GetConfig().FetchAuthConfig( objInput );
	nErrorCode = QAuthInit( &objInput, pszErrMsg );

	return nErrorCode;
}

void Authentication::Release()
{
	if( NULL != m_pCQAClientApi ) {


		m_pCQAClientApi = NULL;
	}
}




