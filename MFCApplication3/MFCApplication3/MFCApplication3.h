
// MFCApplication3.h : MFCApplication3 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "MFCApplication3View.h"


// CMFCApplication3App:
// �йش����ʵ�֣������ MFCApplication3.cpp
//

class CMFCApplication3App : public CWinAppEx
{
public:
	CMFCApplication3App();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CMFCApplication3View* m_pView;

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication3App theApp;
