#pragma once
#include "afxwin.h"


// Move �Ի���

class Move : public CDialogEx
{
	DECLARE_DYNAMIC(Move)

public:
	Move(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Move() {}

// �Ի�������
	enum { IDD = IDD_MOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int place;
	CString Movement;
	CComboBox m_combo;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
