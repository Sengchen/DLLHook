
// DllExcutor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDllExcutorApp: 
// �йش����ʵ�֣������ DllExcutor.cpp
//

class CDllExcutorApp : public CWinApp
{
public:
	CDllExcutorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDllExcutorApp theApp;