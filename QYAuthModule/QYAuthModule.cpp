#include "targetver.h"
#include "QYAuthModule.h"


extern "C"
{
	__declspec(dllexport) void __stdcall  GetFactoryObject()
	{
		::printf( "[Version]  1.0.1 \n" );

	}

	__declspec(dllexport) void __stdcall	ExecuteUnitTest()
	{
		::printf( "\n\n---------------------- [Begin] -------------------------\n" );

		::printf( "----------------------  [End]  -------------------------\n\n\n" );
	}

}




