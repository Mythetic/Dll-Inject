// 线程注入.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>

BOOL LoadDll(DWORD dwProcessID, char* szDllPathName)
{
	BOOL bRet;
	HANDLE hProcess; //存储进程的句柄
	HANDLE  hThread; //线程句柄
	DWORD dwLength; //获取路径的长度
	DWORD dwLoadAddr;
	LPVOID lpAllocAddr;
	HMODULE hModule;

	bRet = 0;
	hProcess = 0;
	dwLoadAddr = 0;

	//1. 获取进程的句柄
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hProcess==NULL)
	{
		OutputDebugString(TEXT("hProcess open failed!\n"));
		return FALSE;
	}

	//2. 计算DLL路径的长度，考虑结尾\0要叫1
	dwLength = strlen(szDllPathName) + 1;

	//3. 在目标进程分配内存
	lpAllocAddr = VirtualAllocEx(hProcess,NULL,dwLength, MEM_COMMIT, PAGE_READWRITE);

	if (lpAllocAddr==NULL)
	{
		OutputDebugString(TEXT("lpAllocAddr alloc failed!\n"));
		CloseHandle(hProcess);
		return FALSE;
	}

	//4. 拷贝DLL路径到目标进程的内存
	bRet = WriteProcessMemory(hProcess, lpAllocAddr, szDllPathName, dwLength, NULL);

	if (!bRet)
	{
		OutputDebugString(TEXT("WriteProcessMemory error\n"));
		CloseHandle(hProcess);
		return FALSE;
	}

	//5. 获取模块地址
	hModule = GetModuleHandle(TEXT("KERNEL32.dll"));
	if (!hModule)
	{
		OutputDebugString(TEXT("hModule error!\n"));
		CloseHandle(hProcess);
		return FALSE;
	}

	//6. 获取LoadLibrary函数地址
	dwLoadAddr = (DWORD)GetProcAddress(hModule, "LoadLibraryA");

	if (!dwLoadAddr)
	{
		OutputDebugString(TEXT("dwLoadAddr error!\n"));
		CloseHandle(hProcess);
		CloseHandle(hModule);
		return FALSE;
	}

	//7. 创建远程线程，加载dll
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)dwLoadAddr, lpAllocAddr, 0, NULL);

	if (!hThread)
	{
		OutputDebugString(TEXT("hThread error!\n"));
		CloseHandle(hProcess);
		CloseHandle(hModule);
		return FALSE;
	}

	//8. 关闭程序句柄
	CloseHandle(hProcess);

	return TRUE;

}

//E:\\VS工程\\MyDll\\Debug\\MyDll.dll
int main()
{
	LoadDll(9620, "C:\\Users\\Administrator\\Desktop\\MyDll.dll");

	system("pause");
    return 0;
}

