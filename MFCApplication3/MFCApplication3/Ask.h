#pragma once


// Ask �Ի���

class Ask : public CDialogEx
{
	DECLARE_DYNAMIC(Ask)

public:
	Ask(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Ask(){}
	afx_msg void OnBnClickedOk();
	int dimension;

								// �Ի�������
	enum { IDD = IDD_ASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
