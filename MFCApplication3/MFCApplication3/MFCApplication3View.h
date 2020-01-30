
// MFCApplication3View.h : CMFCApplication3View 类的接口
//

#pragma once
#include "cube.h"
#include "MFCApplication3Doc.h"

class CMFCApplication3View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication3View();
	DECLARE_DYNCREATE(CMFCApplication3View)

// 特性
public:
	CMFCApplication3Doc* GetDocument() const;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplication3View(){}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	bool SetWindowPixelFormat(HDC hDC);
    // 创建绘制环境(RC)并使之成为当前绘制环境    
	bool CreateViewGLContext(HDC hDC);
    // 初始化openGL
    bool InitGL(void);
    //>像素格式的索引值
    int m_GLPixelIndex;
    // 绘制环境，HGLRC是一个指向rendering context的句柄
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

#ifndef _DEBUG  // MFCApplication3View.cpp 中的调试版本
inline CMFCApplication3Doc* CMFCApplication3View::GetDocument() const
   { return reinterpret_cast<CMFCApplication3Doc*>(m_pDocument); }
#endif

