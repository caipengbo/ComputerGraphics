
// BezierView.h : CBezierView 类的接口
//

#pragma once
#include<vector>
using namespace std;

class CBezierView : public CView
{
protected: // 仅从序列化创建
	CBezierView();
	DECLARE_DYNCREATE(CBezierView)

// 特性
public:
	CBezierDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	vector<CPoint> vertex;
	bool flag;
	CPoint deCasteljau(vector<CPoint> vertex,int i,int k,float t);
protected:

// 实现
public:
	virtual ~CBezierView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // BezierView.cpp 中的调试版本
inline CBezierDoc* CBezierView::GetDocument() const
   { return reinterpret_cast<CBezierDoc*>(m_pDocument); }
#endif

