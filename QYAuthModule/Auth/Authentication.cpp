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
	CQAuthClientInput		objInput;						///< �������
	int						nErrorCode = 0;					///< ������
	char					pszErrMsg[1024*8] = { 0 };		///< ���������Ϣ

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




