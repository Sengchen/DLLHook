// DLLTest.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "DLLTest.h"

int __stdcall add(int a, int b) 
{
	return a + b;
}