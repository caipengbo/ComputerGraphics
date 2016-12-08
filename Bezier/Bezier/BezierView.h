
// BezierView.h : CBezierView ��Ľӿ�
//

#pragma once
#include<vector>
using namespace std;

class CBezierView : public CView
{
protected: // �������л�����
	CBezierView();
	DECLARE_DYNCREATE(CBezierView)

// ����
public:
	CBezierDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	vector<CPoint> vertex;
	bool flag;
	CPoint deCasteljau(vector<CPoint> vertex,int i,int k,float t);
protected:

// ʵ��
public:
	virtual ~CBezierView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // BezierView.cpp �еĵ��԰汾
inline CBezierDoc* CBezierView::GetDocument() const
   { return reinterpret_cast<CBezierDoc*>(m_pDocument); }
#endif

