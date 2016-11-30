
// PolygonClipView.cpp : CPolygonClipView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPolygonClipView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_CLEAR, &CPolygonClipView::OnClear)
END_MESSAGE_MAP()

// CPolygonClipView 构造/析构

CPolygonClipView::CPolygonClipView()
{
	// TODO: 在此处添加构造代码
	flag = false;
}

CPolygonClipView::~CPolygonClipView()
{
}

BOOL CPolygonClipView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPolygonClipView 绘制

void CPolygonClipView::OnDraw(CDC* pDC)
{
	CPolygonClipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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
		//画裁剪后的多边形
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


// CPolygonClipView 打印


void CPolygonClipView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPolygonClipView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPolygonClipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPolygonClipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CPolygonClipView 诊断

#ifdef _DEBUG
void CPolygonClipView::AssertValid() const
{
	CView::AssertValid();
}

void CPolygonClipView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPolygonClipDoc* CPolygonClipView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPolygonClipDoc)));
	return (CPolygonClipDoc*)m_pDocument;
}
#endif //_DEBUG

//判断点是否在裁剪边的可见侧
bool CPolygonClipView::inside(Vertex& point,Edge clipBoundary)
{
	if (clipBoundary.start.x < clipBoundary.end.x){  //下
		if (point.y>=clipBoundary.start.y)
			return true;
	} 
	else if (clipBoundary.start.x > clipBoundary.end.x){ //上
		if (point.y<=clipBoundary.start.y)
			return true;
	}
	else if (clipBoundary.start.y < clipBoundary.end.y){ //右
			if (point.x<=clipBoundary.start.x)
				return true;
		} 
	else if(clipBoundary.start.y > clipBoundary.end.y){ //左
			if (point.x>=clipBoundary.start.x)
				return true;
		}
	return false;
}
//求交点
Vertex CPolygonClipView::intersect(Vertex& S,Vertex& P,Edge clipBoundary)
{
	Vertex intersectPoint;
	if (clipBoundary.start.y == clipBoundary.end.y) //水平
	{
		intersectPoint.y = clipBoundary.start.y;
		intersectPoint.x = S.x + (clipBoundary.start.y - S.y)*(P.x - S.x)/(P.y - S.y);
	} else {  //垂直
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
	//顺时针裁剪
	int length = (int)inVertexArray.size();
	S = inVertexArray[length-1]; 
	for (int i = 0;i<length;i++)
	{
		P = inVertexArray[i];
		if (inside(P,clipBoundary))
		{
			if (inside(S,clipBoundary)) //情况(1) 只输出 P
			{
				outVertexArray.push_back(P);
			} else { //情况(4) 输出交点和P
				intersectPoint = intersect(S,P,clipBoundary);
				outVertexArray.push_back(intersectPoint);
				outVertexArray.push_back(P);
			}
		} else if (inside(S,clipBoundary)){ //情况(3) 输出交点
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
