// DLLHook.h : DLLHook DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDLLHookApp
// �йش���ʵ�ֵ���Ϣ������� DLLHook.cpp
//

class CDLLHookApp : public CWinApp
{
public:
	CDLLHookApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
