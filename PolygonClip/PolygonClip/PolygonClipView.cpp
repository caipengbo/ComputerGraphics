
// PolygonClipView.cpp : CPolygonClipView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "PolygonClip.h"
#endif

#include "PolygonClipDoc.h"
#include "PolygonClipView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPolygonClipView

IMPLEMENT_DYNCREATE(CPolygonClipView, CView)

BEGIN_MESSAGE_MAP(CPolygonClipView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPolygonClipView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_CLEAR, &CPolygonClipView::OnClear)
END_MESSAGE_MAP()

// CPolygonClipView ����/����

CPolygonClipView::CPolygonClipView()
{
	// TODO: �ڴ˴���ӹ������
	flag = false;
}

CPolygonClipView::~CPolygonClipView()
{
}

BOOL CPolygonClipView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPolygonClipView ����

void CPolygonClipView::OnDraw(CDC* pDC)
{
	CPolygonClipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	CRect rect(CPoint(100,100),CPoint(500,400));
	pDC->Rectangle(rect);
	
	int len = inVertexArray.size();
	CPen pen;
	pen.CreatePen(PS_SOLID ,1,RGB(255,0,0)); 
	CPen* poldPen=pDC->SelectObject(&pen);
	for (int i=0;i<len;i++)
	{
		pDC->MoveTo(CPoint((int)inVertexArray[i].x,(int)inVertexArray[i].y));
		pDC->LineTo(CPoint((int)inVertexArray[(i+1)%len].x,(int)inVertexArray[(i+1)%len].y));
	}
	pDC->SelectObject(poldPen);
	if (flag)
	{
		Edge clipBoundary[4];
		vector<Vertex> outputArray;
		clipBoundary[0]= Edge(Vertex(100,100),Vertex(500,100));
		clipBoundary[1] = Edge(Vertex(500,100),Vertex(500,400));
		clipBoundary[2] = Edge(Vertex(500,400),Vertex(100,400));
		clipBoundary[3] = Edge(Vertex(100,400),Vertex(100,100));
		outputArray = inVertexArray;
		for (int i=0;i<4;i++)
		{
			outputArray = SHClip(outputArray,clipBoundary[i]);
		}
		int length = (int)outputArray.size();
		//���ü���Ķ����
		CPen pen_clip;
		pen_clip.CreatePen(PS_SOLID ,1,RGB(0,255,0)); 
		poldPen=pDC->SelectObject(&pen_clip);
		for (int i=0;i<length;i++)
		{
			pDC->MoveTo(CPoint((int)outputArray[i].x,(int)outputArray[i].y));
			pDC->LineTo(CPoint((int)outputArray[(i+1)%length].x,(int)outputArray[(i+1)%length].y));
		}
		pDC->SelectObject(poldPen);
	}
	
}


// CPolygonClipView ��ӡ


void CPolygonClipView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPolygonClipView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPolygonClipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPolygonClipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CPolygonClipView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	flag = true;
	Invalidate(TRUE);
}

void CPolygonClipView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPolygonClipView ���

#ifdef _DEBUG
void CPolygonClipView::AssertValid() const
{
	CView::AssertValid();
}

void CPolygonClipView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPolygonClipDoc* CPolygonClipView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPolygonClipDoc)));
	return (CPolygonClipDoc*)m_pDocument;
}
#endif //_DEBUG

//�жϵ��Ƿ��ڲü��ߵĿɼ���
bool CPolygonClipView::inside(Vertex& point,Edge clipBoundary)
{
	if (clipBoundary.start.x < clipBoundary.end.x){  //��
		if (point.y>=clipBoundary.start.y)
			return true;
	} 
	else if (clipBoundary.start.x > clipBoundary.end.x){ //��
		if (point.y<=clipBoundary.start.y)
			return true;
	}
	else if (clipBoundary.start.y < clipBoundary.end.y){ //��
			if (point.x<=clipBoundary.start.x)
				return true;
		} 
	else if(clipBoundary.start.y > clipBoundary.end.y){ //��
			if (point.x>=clipBoundary.start.x)
				return true;
		}
	return false;
}
//�󽻵�
Vertex CPolygonClipView::intersect(Vertex& S,Vertex& P,Edge clipBoundary)
{
	Vertex intersectPoint;
	if (clipBoundary.start.y == clipBoundary.end.y) //ˮƽ
	{
		intersectPoint.y = clipBoundary.start.y;
		intersectPoint.x = S.x + (clipBoundary.start.y - S.y)*(P.x - S.x)/(P.y - S.y);
	} else {  //��ֱ
		intersectPoint.x = clipBoundary.start.x;
		intersectPoint.y = S.y + (clipBoundary.start.x - S.x)*(P.y - S.y)/(P.x - S.x);
	}
	return intersectPoint;
}
//Sutherland-Hodgman Polygon Clip
vector<Vertex> CPolygonClipView::SHClip(vector<Vertex> inVertexArray,Edge clipBoundary)
{
	vector<Vertex> outVertexArray;
	Vertex S,P,intersectPoint;  // S->P
	//˳ʱ��ü�
	int length = (int)inVertexArray.size();
	S = inVertexArray[length-1]; 
	for (int i = 0;i<length;i++)
	{
		P = inVertexArray[i];
		if (inside(P,clipBoundary))
		{
			if (inside(S,clipBoundary)) //���(1) ֻ��� P
			{
				outVertexArray.push_back(P);
			} else { //���(4) ��������P
				intersectPoint = intersect(S,P,clipBoundary);
				outVertexArray.push_back(intersectPoint);
				outVertexArray.push_back(P);
			}
		} else if (inside(S,clipBoundary)){ //���(3) �������
			intersectPoint = intersect(S,P,clipBoundary);
			outVertexArray.push_back(intersectPoint);
		}
		S = P;
	}
	return outVertexArray;
}

void CPolygonClipView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	Vertex p;
	p.x = (float)point.x;
	p.y = (float)point.y;
	inVertexArray.push_back(p);
	Invalidate(TRUE);
	CView::OnLButtonDown(nFlags, point);
}


void CPolygonClipView::OnClear()
{
	// TODO: Add your command handler code here
	inVertexArray.clear();
	flag = false;
	Invalidate(TRUE);
}
