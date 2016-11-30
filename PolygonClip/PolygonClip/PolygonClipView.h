
// PolygonClipView.h : CPolygonClipView 类的接口
//
#include <vector>
using namespace std;
struct Vertex 
{
	float x;
	float y;
	Vertex(){}
	Vertex(float a,float b){
		x = a;
		y = b;
	}
};
struct Edge
{
	Vertex start;
	Vertex end;
	Edge(){}
	Edge(Vertex s,Vertex e){
		start = s;
		end = e;
	}
};
#pragma once


class CPolygonClipView : public CView
{
protected: // 仅从序列化创建
	CPolygonClipView();
	DECLARE_DYNCREATE(CPolygonClipView)

// 特性
public:
	CPolygonClipDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CPolygonClipView();
	vector<Vertex> inVertexArray;
	bool CPolygonClipView::inside(Vertex& point,Edge clipBoundary);
	Vertex CPolygonClipView::intersect(Vertex& S,Vertex& P,Edge clipBoundary);
	vector<Vertex> CPolygonClipView::SHClip(vector<Vertex> inVertexArray,Edge clipBoundary);
	bool flag;
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
	afx_msg void OnClear();
};

#ifndef _DEBUG  // PolygonClipView.cpp 中的调试版本
inline CPolygonClipDoc* CPolygonClipView::GetDocument() const
   { return reinterpret_cast<CPolygonClipDoc*>(m_pDocument); }
#endif

