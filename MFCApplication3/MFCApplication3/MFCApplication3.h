
// MFCApplication3.h : MFCApplication3 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "MFCApplication3View.h"


// CMFCApplication3App:
// 有关此类的实现，请参阅 MFCApplication3.cpp
//

class CMFCApplication3App : public CWinAppEx
{
public:
	CMFCApplication3App();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CMFCApplication3View* m_pView;

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication3App theApp;
