
// MFCApplication3View.h : CMFCApplication3View ��Ľӿ�
//

#pragma once
#include "cube.h"
#include "MFCApplication3Doc.h"

class CMFCApplication3View : public CView
{
protected: // �������л�����
	CMFCApplication3View();
	DECLARE_DYNCREATE(CMFCApplication3View)

// ����
public:
	CMFCApplication3Doc* GetDocument() const;

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMFCApplication3View(){}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	bool SetWindowPixelFormat(HDC hDC);
    // �������ƻ���(RC)��ʹ֮��Ϊ��ǰ���ƻ���    
	bool CreateViewGLContext(HDC hDC);
    // ��ʼ��openGL
    bool InitGL(void);
    //>���ظ�ʽ������ֵ
    int m_GLPixelIndex;
    // ���ƻ�����HGLRC��һ��ָ��rendering context�ľ��
    HGLRC m_hGLContext;
	void RenderSceneCB();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditUndo();
	afx_msg void On32775();
	afx_msg void On32777();
	afx_msg void On32778();
};

#ifndef _DEBUG  // MFCApplication3View.cpp �еĵ��԰汾
inline CMFCApplication3Doc* CMFCApplication3View::GetDocument() const
   { return reinterpret_cast<CMFCApplication3Doc*>(m_pDocument); }
#endif

