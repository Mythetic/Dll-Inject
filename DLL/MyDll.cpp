#include "MyDll.h"
#include<stdio.h>
#include <windows.h>

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	MessageBoxA(NULL, "Hello", "Fan", 0);
	for (;;)
	{
		Sleep(1000);
		
		printf("TestDll 中的代码在执行....\n");
	}
	return 0;
}



BOOL WINAPI DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString(TEXT("1:DLL_PROCESS_ATTACH"));
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, NULL, 0, NULL);
		break;
	case DLL_THREAD_ATTACH:
		OutputDebugString(TEXT("2:DLL_THREAD_ATTACH"));
		break;
	case DLL_PROCESS_DETACH:
		OutputDebugString(TEXT("3:DLL_PROCESS_DETACH"));
		break;
	case DLL_THREAD_DETACH:
		OutputDebugString(TEXT("4:DLL_THREAD_DETACH"));
		break;
	}
	return TRUE;
};


int _stdcall Plus(int x, int y)
{
	return x + y;
}

void _stdcall  print()
{
	printf("Ma FAN");
}