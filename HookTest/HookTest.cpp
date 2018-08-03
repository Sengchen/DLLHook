// HookTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.system.h>
#include <iostream>

using namespace std;

/*
 提权
*/
void up_privileges()
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tp;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;
	AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
}

int main()
{
	DWORD oldProject;
	BYTE jmpBYTE[5];
	BYTE oldBYTE[5];
	void*OpenProcessAddr;

	HMODULE hModule = LoadLibrary(L"kernelbase.dll");
	void*addr = (void*)GetProcAddress(hModule, "OpenProcess");
	if (addr != NULL)
	{
		cout << "load success!" << endl;
	}
    return 0;
}

