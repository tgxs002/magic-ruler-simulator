#pragma once
#include "afxwin.h"


// Move 对话框

class Move : public CDialogEx
{
	DECLARE_DYNAMIC(Move)

public:
	Move(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Move() {}

// 对话框数据
	enum { IDD = IDD_MOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int place;
	CString Movement;
	CComboBox m_combo;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
