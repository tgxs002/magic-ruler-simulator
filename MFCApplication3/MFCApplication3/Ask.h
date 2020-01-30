#pragma once


// Ask 对话框

class Ask : public CDialogEx
{
	DECLARE_DYNAMIC(Ask)

public:
	Ask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Ask(){}
	afx_msg void OnBnClickedOk();
	int dimension;

								// 对话框数据
	enum { IDD = IDD_ASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
