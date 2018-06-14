#include "targetver.h"
#include "QYAuthModule.h"


extern "C"
{
	__declspec(dllexport) I_AuthApi* __stdcall  GetSingletonAuthApi()
	{
		return &(Authentication::GetAuth());
	}

}




