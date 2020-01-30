// Move.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "Move.h"
#include "afxdialogex.h"


// Move �Ի���

IMPLEMENT_DYNAMIC(Move, CDialogEx)

Move::Move(CWnd* pParent /*=NULL*/)
	: CDialogEx(Move::IDD, pParent)
	, place(0)
	, Movement(_T(""))
{}

void Move::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, place);
	DDV_MinMaxInt(pDX, place, -100000, 100000);
	DDX_CBString(pDX, IDC_COMBO1, Movement);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
}


BEGIN_MESSAGE_MAP(Move, CDialogEx)
	ON_BN_CLICKED(IDOK, &Move::OnBnClickedOk)
END_MESSAGE_MAP()


// Move ��Ϣ�������



void Move::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CDialogEx::OnOK();
}


BOOL Move::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
