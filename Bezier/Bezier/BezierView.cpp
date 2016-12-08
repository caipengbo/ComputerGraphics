
// BezierView.cpp : CBezierView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CBezierView ����/����

CBezierView::CBezierView()
{
	// TODO: �ڴ˴���ӹ������
	flag = false;

}

CBezierView::~CBezierView()
{
}

BOOL CBezierView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CBezierView ����

//�������(n����,n-1��Bezier����)
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

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	CPoint bezierP;
	int len = vertex.size();
	vector<CPoint> bezArray;//���Bezier���ߵ�
	vector<CPoint> basicArray;//��Ż�������
	for(int i=0;i<len-1;i++)
	{
		pDC->MoveTo(vertex[i]);
		pDC->LineTo(vertex[i+1]);
	}
	if(flag){
		//������������������ÿ���㶼�Ŵ���ٱ���
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
		//�ݹ����Bezier����
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


// CBezierView ���

#ifdef _DEBUG
void CBezierView::AssertValid() const
{
	CView::AssertValid();
}

void CBezierView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBezierDoc* CBezierView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBezierDoc)));
	return (CBezierDoc*)m_pDocument;
}
#endif //_DEBUG


// CBezierView ��Ϣ�������

//Bezier���ߵݹ��㷨
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	vertex.push_back(point);
	Invalidate(TRUE);
	CView::OnLButtonDown(nFlags, point);
}


void CBezierView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	flag = true;
	Invalidate(TRUE);
	CView::OnRButtonDown(nFlags, point);
}
