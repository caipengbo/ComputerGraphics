
// LineClipView.h : interface of the CLineClipView class
//

#pragma once


class CLineClipView : public CView
{
protected: // create from serialization only
	CLineClipView();
	DECLARE_DYNCREATE(CLineClipView)

// Attributes
public:
	CLineClipDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLineClipView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LineClipView.cpp
inline CLineClipDoc* CLineClipView::GetDocument() const
   { return reinterpret_cast<CLineClipDoc*>(m_pDocument); }
#endif

#define  LEFT  1
#define  RIGHT  2
#define  BOTTOM  4
#define  TOP  8