
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
END_MESSAGE_MAP()

// CLineClipView construction/destruction

CLineClipView::CLineClipView()
{
	// TODO: add construction code here

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

void CLineClipView::OnDraw(CDC* /*pDC*/)
{
	CLineClipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
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


// CLineClipView message handlers
//±‡¬Î
int encode(CPoint point,int xl,int xr,int yb,int yt)
{
	int c = 0;
	if (point.x < xl) c |= LEFT;
	if (point.x > xr) c |= RIGHT;
	if (point.y < yb) c |= BOTTOM;
	if (point.y > yt) c |= TOP;
	return c;
}
void displayClipedLine(CPoint pointA,CPoint pointB,CDC* pDC)
{
	pDC->MoveTo(pointA);
	pDC->LineTo(pointB);
}
//SC_LineClip
void SC_LineClip(CPoint &pointA,CPoint &pointB,int xl,int xr,int yb,int yt,CDC* pDC)
{
	int codeA = 0,codeB = 0,tempCode = 0;
	int newX = 0,newY = 0;
	codeA = encode(pointA,xl,xr,yb,yt);
	codeB = encode(pointB,xl,xr,yb,yt);
	while (codeA != 0 || codeB != 0)
	{
		if (codeA & codeB != 0) return;//‘⁄≤√ºÙ«¯”ÚÕ‚
		if (codeA != 0)
			tempCode = codeA;
		else 
			tempCode = codeB;
		if (LEFT & tempCode !=0)
		{
			newX = xl;
			newY = pointA.y + (pointB.y - pointA.y) * (xl - pointA.x) / (pointB.x - pointA.x);
		}
		else if (RIGHT & tempCode !=0)
		{
			newX = xr;
			newY = pointA.y + (pointB.y - pointA.y) * (xr - pointA.x) / (pointB.x - pointA.x);
		}
		else if (BOTTOM & tempCode !=0)
		{
			newY = yb;
			newX = pointA.x + (pointB.x - pointA.x) * (yb - pointA.y) / (pointB.y - pointA.y);
		}else if (TOP & tempCode !=0)
		{
			newY = yt;
			newX = pointA.x + (pointB.x - pointA.x) * (yt - pointA.y) / (pointB.y - pointA.y);
		}
		if (tempCode == codeA)
		{
			pointA.x = newX;
			pointA.y = newY;
			codeA = encode(pointA);
		}else {
			pointB.x = newX;
			pointB.y = newY;
			codeB = encode(pointB);
		}
	} //while end

}