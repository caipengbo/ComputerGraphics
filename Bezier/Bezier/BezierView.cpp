
// BezierView.cpp : CBezierView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Bezier.h"
#endif

#include "BezierDoc.h"
#include "BezierView.h"
#include <cmath>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBezierView

IMPLEMENT_DYNCREATE(CBezierView, CView)

BEGIN_MESSAGE_MAP(CBezierView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CBezierView 构造/析构

CBezierView::CBezierView()
{
	// TODO: 在此处添加构造代码
	flag = false;

}

CBezierView::~CBezierView()
{
}

BOOL CBezierView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CBezierView 绘制

//求基函数(n个点,n-1次Bezier曲线)
int fac(int n)
{
	if(n==0)
		return 1;
	else 
		return n * fac(n-1);
}
double B(int i,int n,double t)
{
	double  c = (double)fac(n)/(double)(fac(i)*fac(n-i));
	return c*(pow(t,i))*pow(1-t,n-i);
}

void CBezierView::OnDraw(CDC* pDC)
{
	CBezierDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CPoint bezierP;
	int len = vertex.size();
	vector<CPoint> bezArray;//存放Bezier曲线点
	vector<CPoint> basicArray;//存放基函数点
	for(int i=0;i<len-1;i++)
	{
		pDC->MoveTo(vertex[i]);
		pDC->LineTo(vertex[i+1]);
	}
	if(flag){
		//画基函数（基函数的每个点都放大二百倍）
		for(int i=0;i<len;i++)
		{
			for(float t=0.0;t<=1.0;t=t+0.01){
				basicArray.push_back(CPoint(t*200,B(i,len-1,t)*200));
			}
		}
		for(int i=0;i<basicArray.size()-1;i++)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID ,1,RGB(0,255,0)); 
			CPen* poldPen=pDC->SelectObject(&pen);
			pDC->MoveTo(basicArray[i]);
			pDC->LineTo(basicArray[i+1]);
			pDC->SelectObject(poldPen);
		}
		//递归绘制Bezier曲线
		for(float t=0.0;t<=1.0;t=t+0.01){
			bezierP = deCasteljau(vertex,0,len-1,t);
			//pDC->SetPixel(bezierP,RGB(255,0,0));
			bezArray.push_back(bezierP);
		}
		for(int i=0;i<bezArray.size()-1;i++)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID ,1,RGB(255,0,0)); 
			CPen* poldPen=pDC->SelectObject(&pen);
			pDC->MoveTo(bezArray[i]);
			pDC->LineTo(bezArray[i+1]);
			pDC->SelectObject(poldPen);
		}
	}
}

void CBezierView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
}

void CBezierView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBezierView 诊断

#ifdef _DEBUG
void CBezierView::AssertValid() const
{
	CView::AssertValid();
}

void CBezierView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBezierDoc* CBezierView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBezierDoc)));
	return (CBezierDoc*)m_pDocument;
}
#endif //_DEBUG


// CBezierView 消息处理程序

//Bezier曲线递归算法
CPoint CBezierView::deCasteljau(vector<CPoint> vertex,int i,int k,float t)
{
	if(k==0)
		return vertex[i];
	else {
		float x =(1.0-t)*(float)(deCasteljau(vertex,i,k-1,t).x) + t * (float)(deCasteljau(vertex,i+1,k-1,t).x);
		float y = (1.0-t)*(float)(deCasteljau(vertex,i,k-1,t).y) + t * (float)(deCasteljau(vertex,i+1,k-1,t).y);
		return CPoint((long)x,(long)y);
	}		
}

void CBezierView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	vertex.push_back(point);
	Invalidate(TRUE);
	CView::OnLButtonDown(nFlags, point);
}


void CBezierView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	flag = true;
	Invalidate(TRUE);
	CView::OnRButtonDown(nFlags, point);
}
