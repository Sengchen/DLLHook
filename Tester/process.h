#pragma once
#include "stdafx.h"
#include <iostream>
#include <windows.system.h>
#include <TlHelp32.h>
#ifndef __linux__
#include <Windows.h>
#else
#include <unistd.h>
#endif

class Process
{
public:
	Process();
	~Process();
	static HANDLE GetProcessByName(LPCWSTR name);
private:

};

Process::Process()
{
}

Process::~Process()
{
}

HANDLE Process::GetProcessByName(LPCWSTR name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return NULL;
	}

	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
	{
		if (!_tcscmp(pe.szExeFile, name))
		{
			CloseHandle(hSnapshot);
			return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
		}
	}
	return NULL;
}