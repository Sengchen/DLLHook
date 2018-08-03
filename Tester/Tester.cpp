// Tester.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <windows.system.h>
#include <TlHelp32.h>
#include <conio.h>
#include <Psapi.h>
#include "process.h"
#include <ProcessSnapshot.h>

#ifndef __linux__
#include <Windows.h>
#else
#include <unistd.h>
#endif // !__linux__

using namespace std;

int main()
{
	#ifndef __linux__
	printf("now pid is %d \n", GetCurrentProcessId());
	printf("now tid is %d \n", GetCurrentThreadId());
	#else
	printf("now pid is %d \n", getpid());
	printf("now tid is %d \n", gettid());
	#endif

	// 基址 minesweeper.exe+AAA38, Timer 0019E2D0 + 18, Time 0019E510 + 20
	LPCWSTR processName = _T("MineSweeper.exe");
	HANDLE hProcess = Process::GetProcessByName(processName);
	if (INVALID_HANDLE_VALUE == hProcess)
	{
		printf("get process %s failed!\n", processName);
	}
	else
	{
		DWORD pid = GetProcessId(hProcess);
		printf("process %s pid is %d!\n", processName, pid);

		// 此方法只能用于注入获取
		/*HMODULE hModule = GetModuleHandle(processName);
		DWORD baseAddr = (DWORD)hModule;*/
		
		/*HMODULE hModules;
		DWORD need;
		EnumProcessModulesEx(hProcess, &hModules, sizeof(hModules), &need, 1);
		DWORD base = (DWORD)hModules;
		cout << base << endl;*/

		//WriteProcessMemory(hProcess, , 0, 0, NULL);

		//// 存放快照信息的结构体, 使用之前先设置大小
		//MODULEENTRY32 me32 = { sizeof(me32) };
		//// 创建进程快照
		//HANDLE phSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		//if (INVALID_HANDLE_VALUE == phSnapshot)
		//{
		//	cout << "get module failed!" << endl;
		//}
		//else
		//{
		//	for (BOOL fOk = Module32First(phSnapshot, &me32); fOk; fOk = Module32Next(phSnapshot, &me32))
		//	{
		//		cout << me32.th32ProcessID << endl;
		//		if (me32.th32ProcessID == pid)
		//		{
		//			cout << "base address: 0x" << me32.modBaseAddr << endl;
		//			//printf("base address: 0x%08X\n", me32.szExePath, (DWORD)me32.modBaseAddr);
		//			WriteProcessMemory(hProcess, me32.modBaseAddr + 0x000AAA38 + 0x18 + 0x20, 0, 0, NULL);
		//			CloseHandle(phSnapshot);
		//		}
		//	}
		//}
		
		int ch;
		while (true)
		{
			// 如果有按键按下，则_kbhit()函数返回真
			if (_kbhit())
			{
				// 使用_getch()函数获取按下的键值
				ch = _getch();
				// 'ESC'
				if (ch == 27)
				{
					break;
				}
				// '/'
				else if (ch == 47)
				{
					
				}
			}
		}
		
		CloseHandle(hProcess);
	}

	return 0;
}

