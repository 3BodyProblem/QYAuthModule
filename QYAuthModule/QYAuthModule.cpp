#include "targetver.h"
#include "QYAuthModule.h"


extern "C"
{
	__declspec(dllexport) I_AuthApi*   GetSingletonAuthApi()
	{
		return &(Authentication::GetAuth());
	}

}




