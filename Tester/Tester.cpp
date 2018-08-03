// Tester.cpp : �������̨Ӧ�ó������ڵ㡣
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

	// ��ַ minesweeper.exe+AAA38, Timer 0019E2D0 + 18, Time 0019E510 + 20
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

		// �˷���ֻ������ע���ȡ
		/*HMODULE hModule = GetModuleHandle(processName);
		DWORD baseAddr = (DWORD)hModule;*/
		
		/*HMODULE hModules;
		DWORD need;
		EnumProcessModulesEx(hProcess, &hModules, sizeof(hModules), &need, 1);
		DWORD base = (DWORD)hModules;
		cout << base << endl;*/

		//WriteProcessMemory(hProcess, , 0, 0, NULL);

		//// ��ſ�����Ϣ�Ľṹ��, ʹ��֮ǰ�����ô�С
		//MODULEENTRY32 me32 = { sizeof(me32) };
		//// �������̿���
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
			// ����а������£���_kbhit()����������
			if (_kbhit())
			{
				// ʹ��_getch()������ȡ���µļ�ֵ
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

