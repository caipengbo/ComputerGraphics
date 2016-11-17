
// LineClipView.cpp : implementation of the CLineClipView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LineClip.h"
#endif

#include "LineClipDoc.h"
#include "LineClipView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLineClipView

IMPLEMENT_DYNCREATE(CLineClipView, CView)

BEGIN_MESSAGE_MAP(CLineClipView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLineClipView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_CS, &CLineClipView::OnCs)
	ON_COMMAND(ID_LB, &CLineClipView::OnLb)
END_MESSAGE_MAP()

// CLineClipView construction/destruction

CLineClipView::CLineClipView()
{
	// TODO: add construction code here
	vertexNum = 0;
	flag = 0;
}

CLineClipView::~CLineClipView()
{
}

BOOL CLineClipView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLineClipView drawing

void CLineClipView::OnDraw(CDC* pDC)
{
	CLineClipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	//设置裁剪窗口
	CRect rect(CPoint(100,100),CPoint(400,300));
	CPoint pointA(0,0);
	CPoint pointB(420,310);
	pDC->Rectangle(rect);
	if (vertexNum==2)
	{
		if (flag == 0)
		{
			SC_LineClip(clickedpoint[0],clickedpoint[1],100,400,100,300,pDC);
		}
		else {
			LB_LineClip(clickedpoint[0],clickedpoint[1],100,400,100,300,pDC);
		}
		
		
	}
	

}


// CLineClipView printing


void CLineClipView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLineClipView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLineClipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLineClipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLineClipView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLineClipView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLineClipView diagnostics

#ifdef _DEBUG
void CLineClipView::AssertValid() const
{
	CView::AssertValid();
}

void CLineClipView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLineClipDoc* CLineClipView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLineClipDoc)));
	return (CLineClipDoc*)m_pDocument;
}
#endif //_DEBUG

//Cohen - Sutherland  begin 
int CLineClipView::encode(CPoint point, int xl, int xr, int yb, int yt)
{
	int c = 0;
	if (point.x < xl) c |= LEFT;
	if (point.x > xr) c |= RIGHT;
	if (point.y < yb) c |= BOTTOM;
	if (point.y > yt) c |= TOP;
	return c;
}

void CLineClipView::displayLine(CPoint pointA, CPoint pointB, COLORREF color, CDC* pDC)
{
	CPen pen;
	pen.CreatePen(PS_SOLID ,1,color); 
	CPen* poldPen=pDC->SelectObject(&pen);
	pDC->MoveTo(pointA);
	pDC->LineTo(pointB);
	pDC->SelectObject(poldPen);
}
//SC_LineClip
void CLineClipView::SC_LineClip(CPoint pointA, CPoint pointB, int xl, int xr, int yb, int yt, CDC* pDC)
{
	CPoint oldPointA = pointA;
	CPoint oldPointB = pointB;
	int codeA = 0,codeB = 0,tempCode = 0;
	int newX = 0,newY = 0;
	codeA = encode(pointA,xl,xr,yb,yt);
	codeB = encode(pointB,xl,xr,yb,yt);
	while (codeA != 0 || codeB != 0)
	{
		if ((codeA & codeB) != 0) return;//在裁剪区域外
		if (codeA != 0)
			tempCode = codeA;
		else 
			tempCode = codeB;
		if ((LEFT & tempCode) !=0)
		{
			newX = xl;
			newY = pointA.y + (pointB.y - pointA.y) * (xl - pointA.x) / (pointB.x - pointA.x);
		}
		else if ((RIGHT & tempCode) !=0)
		{
			newX = xr;
			newY = pointA.y + (pointB.y - pointA.y) * (xr - pointA.x) / (pointB.x - pointA.x);
		}
		else if ((BOTTOM & tempCode) !=0)
		{
			newY = yb;
			newX = pointA.x + (pointB.x - pointA.x) * (yb - pointA.y) / (pointB.y - pointA.y);
		}else if ((TOP & tempCode) !=0)
		{
			newY = yt;
			newX = pointA.x + (pointB.x - pointA.x) * (yt - pointA.y) / (pointB.y - pointA.y);
		}

		if (tempCode == codeA)
		{
			pointA.x = newX;
			pointA.y = newY;
			codeA = encode(pointA,xl,xr,yb,yt);
		}else {
			pointB.x = newX;
			pointB.y = newY;
			codeB = encode(pointB,xl,xr,yb,yt);
		}
	} //while end
	//外
	displayLine(oldPointA,pointA,RGB(0,255,0),pDC);
	displayLine(oldPointB,pointB,RGB(0,255,0),pDC);
	//内
	displayLine(pointA,pointB,RGB(255,0,0),pDC);
}
//Cohen - Sutherland  end

//liang - barskey begin
bool CLineClipView::ClipT(float p,float q,float& u1,float& u2)
{
	float r;
	if (p<0)
	{
		r = q / p;
		if (r > u2)
			return false;
		if (r > u1)
			u1 = r;
	} 
	else if (p > 0)
	{
		r = q / p;
		if (r < u1)
			return false;
		if (r < u2)
			u2 = r;
	}
	else
		return (q>=0);
	return true;
}
void CLineClipView::LB_LineClip(CPoint pointA,CPoint pointB, int xl, int xr, int yb, int yt,CDC* pDC)
{
	float dx,dy,u1=0,u2=1;
	dx = (float)(pointB.x - pointA.x);
	dy = (float)(pointB.y - pointA.y);
	if (ClipT(-dx,pointA.x-xl,u1,u2))
		if (ClipT(dx,xr-pointA.x,u1,u2))
			if(ClipT(-dy,pointA.y-yb,u1,u2))
				if(ClipT(dy,yt-pointA.y,u1,u2))
					{
						displayLine(pointA,CPoint((long)(pointA.x+u1*dx),(long)(pointA.y+u1*dy)),RGB(0,255,0),pDC);
						displayLine(pointB,CPoint((long)(pointA.x+u2*dx),(long)(pointA.y+u2*dy)) ,RGB(0,255,0),pDC);
						displayLine(CPoint((long)(pointA.x+u1*dx),(long)(pointA.y+u1*dy)),CPoint((long)(pointA.x+u2*dx),(long)(pointA.y+u2*dy)) ,RGB(0,0,255),pDC);
					}
	
}
//liang - barskey end

void CLineClipView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (vertexNum>=2){
		vertexNum = 0;
	}
	clickedpoint[vertexNum] = point;
	vertexNum++;
	Invalidate(TRUE);
	CView::OnLButtonDown(nFlags, point);
}


void CLineClipView::OnCs()
{
	flag = 0;
	MessageBox(_T("已选择Cohen-Sutherland算法"));
	// TODO: 在此添加命令处理程序代码
}


void CLineClipView::OnLb()
{
	flag = 1;
	MessageBox(_T("已选择梁友栋-Barskey算法"));
	// TODO: 在此添加命令处理程序代码
}
