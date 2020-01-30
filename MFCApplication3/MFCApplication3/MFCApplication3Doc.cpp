
// MFCApplication3Doc.cpp : CMFCApplication3Doc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFCApplication3.h"
#endif
#include "Ask.h"
#include "MFCApplication3Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCApplication3Doc

IMPLEMENT_DYNCREATE(CMFCApplication3Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFCApplication3Doc, CDocument)
END_MESSAGE_MAP()


// CMFCApplication3Doc ����/����

CMFCApplication3Doc::CMFCApplication3Doc()
{
	// TODO: �ڴ����һ���Թ������
	initFlag = false;
}

CMFCApplication3Doc::~CMFCApplication3Doc()
{
}

BOOL CMFCApplication3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	pos = Vector3f(10, 10, 10);
	dir = Vector3f(-1, -1, -1);
	Ask a;

	if (initFlag)
		a.DoModal();
	
	while (initFlag && a.dimension % 2 == 1)
	{
		AfxMessageBox(_T("ħ�߽�������Ϊż����"));
		a.DoModal();
	}

	if (!initFlag)
		ru = new ruler(0);
	else
		{
			ru = new ruler(a.dimension);
			seq.dimension = a.dimension;
			seq.totalNumber = 0;
			seq.present = 0;
	}
	
	initFlag = true;

	return TRUE;
}

// CMFCApplication3Doc ���л�

void CMFCApplication3Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
		
		ar << pos.x;
		ar << pos.y;
		ar << pos.z;
		ar << dir.x;
		ar << dir.y;
		ar << dir.z;
		ar << seq.dimension;
		ar << seq.totalNumber;
		ar << seq.present;
		for (int i = 0; i < seq.totalNumber; i++)
		{
			ar << seq.movement[i];
			ar << seq.place[i];
		}		
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		ar >> pos.x;
		ar >> pos.y;
		ar >> pos.z;
		ar >> dir.x;
		ar >> dir.y;
		ar >> dir.z;
		ar >> seq.dimension;
		ar >> seq.totalNumber;
		ar >> seq.present;
		
		seq.movement.clear();
		seq.place.clear();

		int temp;
		for (int i = 0; i < seq.totalNumber; i++)
		{
			ar >> temp;
			seq.movement.push_back(temp);
			ar >> temp;
			seq.place.push_back(temp);
		}

		ru = new ruler(seq.dimension);

		seq.present = 0;
		
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CMFCApplication3Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CMFCApplication3Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CMFCApplication3Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCApplication3Doc ���

#ifdef _DEBUG
void CMFCApplication3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCApplication3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG