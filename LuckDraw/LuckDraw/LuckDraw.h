
// LuckDraw.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CLuckDrawApp: 
// �йش����ʵ�֣������ LuckDraw.cpp
//

class CLuckDrawApp : public CWinApp
{
public:
	CLuckDrawApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLuckDrawApp theApp;