// Ask.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "Ask.h"
#include "afxdialogex.h"


// Ask �Ի���

IMPLEMENT_DYNAMIC(Ask, CDialogEx)

Ask::Ask(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ask::IDD, pParent)
	, dimension(0)
{}

void Ask::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, dimension);
	DDV_MinMaxInt(pDX, dimension, 2, 50);
}


BEGIN_MESSAGE_MAP(Ask, CDialogEx)
	ON_BN_CLICKED(IDOK, &Ask::OnBnClickedOk)
END_MESSAGE_MAP()


// Ask ��Ϣ�������


void Ask::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
	UpdateData();
}
