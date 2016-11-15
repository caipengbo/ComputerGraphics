
// ScanLinePloyFillView.h : CScanLinePloyFillView 类的接口
//

#include<algorithm>
#include <vector>
#include<list>
#include <iterator>
#include<iostream>
using namespace std;

#pragma once
//边结构
struct Edge {
	int ymax;
	float x;//最低点的x
	float m;//斜率的倒数
	//Edge * next;
};

class CScanLinePloyFillView : public CView
{
protected: // 仅从序列化创建
	CScanLinePloyFillView();
	DECLARE_DYNCREATE(CScanLinePloyFillView)

// 特性
public:
	CScanLinePloyFillDoc* GetDocument() const;

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
	virtual ~CScanLinePloyFillView();
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
	CPoint vertex[100];
	int vertexNum;
	vector<list<Edge>> createNET(CPoint point[], int num, int& ymax, int& ymin);
	list<Edge> creatAET(int y,vector<list<Edge>> NET,int ymin,list<Edge> oldAET,CDC* pDC);
	void ployFill(CPoint point[], int num,CDC* pDC);
	bool flag;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ScanLinePloyFillView.cpp 中的调试版本
inline CScanLinePloyFillDoc* CScanLinePloyFillView::GetDocument() const
   { return reinterpret_cast<CScanLinePloyFillDoc*>(m_pDocument); }
#endif

