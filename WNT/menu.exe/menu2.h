
// menu2.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号


// Cmenu2App:
// 有关此类的实现，请参阅 menu2.cpp
//

class Cmenu2App : public CWinApp
{
public:
	Cmenu2App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern Cmenu2App theApp;
