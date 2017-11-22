// �߳�ע��.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>

BOOL LoadDll(DWORD dwProcessID, char* szDllPathName)
{
	BOOL bRet;
	HANDLE hProcess; //�洢���̵ľ��
	HANDLE  hThread; //�߳̾��
	DWORD dwLength; //��ȡ·���ĳ���
	DWORD dwLoadAddr;
	LPVOID lpAllocAddr;
	HMODULE hModule;

	bRet = 0;
	hProcess = 0;
	dwLoadAddr = 0;

	//1. ��ȡ���̵ľ��
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hProcess==NULL)
	{
		OutputDebugString(TEXT("hProcess open failed!\n"));
		return FALSE;
	}

	//2. ����DLL·���ĳ��ȣ����ǽ�β\0Ҫ��1
	dwLength = strlen(szDllPathName) + 1;

	//3. ��Ŀ����̷����ڴ�
	lpAllocAddr = VirtualAllocEx(hProcess,NULL,dwLength, MEM_COMMIT, PAGE_READWRITE);

	if (lpAllocAddr==NULL)
	{
		OutputDebugString(TEXT("lpAllocAddr alloc failed!\n"));
		CloseHandle(hProcess);
		return FALSE;
	}

	//4. ����DLL·����Ŀ����̵��ڴ�
	bRet = WriteProcessMemory(hProcess, lpAllocAddr, szDllPathName, dwLength, NULL);

	if (!bRet)
	{
		OutputDebugString(TEXT("WriteProcessMemory error\n"));
		CloseHandle(hProcess);
		return FALSE;
	}

	//5. ��ȡģ���ַ
	hModule = GetModuleHandle(TEXT("KERNEL32.dll"));
	if (!hModule)
	{
		OutputDebugString(TEXT("hModule error!\n"));
		CloseHandle(hProcess);
		return FALSE;
	}

	//6. ��ȡLoadLibrary������ַ
	dwLoadAddr = (DWORD)GetProcAddress(hModule, "LoadLibraryA");

	if (!dwLoadAddr)
	{
		OutputDebugString(TEXT("dwLoadAddr error!\n"));
		CloseHandle(hProcess);
		CloseHandle(hModule);
		return FALSE;
	}

	//7. ����Զ���̣߳�����dll
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)dwLoadAddr, lpAllocAddr, 0, NULL);

	if (!hThread)
	{
		OutputDebugString(TEXT("hThread error!\n"));
		CloseHandle(hProcess);
		CloseHandle(hModule);
		return FALSE;
	}

	//8. �رճ�����
	CloseHandle(hProcess);

	return TRUE;

}

//E:\\VS����\\MyDll\\Debug\\MyDll.dll
int main()
{
	LoadDll(9620, "C:\\Users\\Administrator\\Desktop\\MyDll.dll");

	system("pause");
    return 0;
}

