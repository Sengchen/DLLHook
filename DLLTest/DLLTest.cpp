// DLLTest.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "DLLTest.h"

int __stdcall add(int a, int b) 
{
	return a + b;
}