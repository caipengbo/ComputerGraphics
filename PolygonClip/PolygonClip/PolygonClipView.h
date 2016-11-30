
// PolygonClipView.h : CPolygonClipView ��Ľӿ�
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
protected: // �������л�����
	CPolygonClipView();
	DECLARE_DYNCREATE(CPolygonClipView)

// ����
public:
	CPolygonClipDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
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

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClear();
};

#ifndef _DEBUG  // PolygonClipView.cpp �еĵ��԰汾
inline CPolygonClipDoc* CPolygonClipView::GetDocument() const
   { return reinterpret_cast<CPolygonClipDoc*>(m_pDocument); }
#endif

