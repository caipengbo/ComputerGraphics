
// ScanLinePloyFillView.cpp : CScanLinePloyFillView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ScanLinePloyFill.h"
#endif

#include "ScanLinePloyFillDoc.h"
#include "ScanLinePloyFillView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScanLinePloyFillView

IMPLEMENT_DYNCREATE(CScanLinePloyFillView, CView)

BEGIN_MESSAGE_MAP(CScanLinePloyFillView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScanLinePloyFillView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CLEAR, &CScanLinePloyFillView::OnClear)
END_MESSAGE_MAP()

// CScanLinePloyFillView ����/����

CScanLinePloyFillView::CScanLinePloyFillView()
{
	// TODO: �ڴ˴���ӹ������

	vertexNum = 0;
	flag = false;
}

CScanLinePloyFillView::~CScanLinePloyFillView()
{
}

BOOL CScanLinePloyFillView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CScanLinePloyFillView ����



void CScanLinePloyFillView::OnDraw(CDC* pDC)
{
	CScanLinePloyFillDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	/*vertexNum = 5;
	vertex[0].x = 10;
	vertex[0].y = 10;
	vertex[1].x = 16;
	vertex[1].y = 15;
	vertex[2].x = 12;
	vertex[2].y = 20;
	vertex[3].x = 8;
	vertex[3].y = 16;
	vertex[4].x = 6;
	vertex[4].y = 14;
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	vertexNum = 3;
	vertex[0].x = 50;
	vertex[0].y = 0;
	vertex[1].x = 100;
	vertex[1].y = 50;
	vertex[2].x = 40;
	vertex[2].y = 70;*/
	
	for(int i=0;i<vertexNum;i++) {
		pDC->MoveTo(vertex[i]);
		pDC->LineTo(vertex[(i+1)%vertexNum]);
	}

	if (flag)
	{
		ployFill(vertex, vertexNum,pDC);
	}


	
}


// CScanLinePloyFillView ��ӡ


void CScanLinePloyFillView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CScanLinePloyFillView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CScanLinePloyFillView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CScanLinePloyFillView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}
/**/
void CScanLinePloyFillView::OnRButtonUp(UINT  nFlags , CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CScanLinePloyFillView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CScanLinePloyFillView ���

#ifdef _DEBUG
void CScanLinePloyFillView::AssertValid() const
{
	CView::AssertValid();
}

void CScanLinePloyFillView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CScanLinePloyFillDoc* CScanLinePloyFillView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScanLinePloyFillDoc)));
	return (CScanLinePloyFillDoc*)m_pDocument;
}
#endif //_DEBUG


// CScanLinePloyFillView ��Ϣ�������


void CScanLinePloyFillView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	vertex[vertexNum] = point;
	vertexNum++;
	Invalidate(TRUE);
	CView::OnLButtonDown(nFlags, point);
}


//Using STL *************************************************************************

vector<list<Edge>> CScanLinePloyFillView::createNET(CPoint point[], int num, int& ymax, int& ymin)
{
	int len = 0;
	ymax = point[0].y;
	ymin = point[0].y;
	for (int i=0; i<num; i++)
	{
		//Ѱ����ͣ����y
		if (point[i].y > ymax) {
			ymax = point[i].y;
		}
		if (point[i].y < ymin)
		{
			ymin = point[i].y;
		}
	}
	len = ymax - ymin + 1;
	// �±߱�ͷ���� ����ʼ��
	vector<list<Edge>> NET(len);
	//�ڱ�ͷ��������±�
	for (int i=0; i<num; i++)
	{
		int temp = (i+1)%num;
		Edge newedge;
		CPoint top,bottom;
		bool isdispose = false; //�Ƿ���
		if (point[i].y > point[temp].y)
		{
			top = point[i];
			bottom = point[temp];
			//���������
			if (point[(temp+1)%num].y < bottom.y)
				isdispose = true;
		} else {
			top = point[temp];
			bottom = point[i];
			int temppri;
			if (i-1<0)
				temppri = num-1;
			else
				temppri = i-1;
			if (point[temppri].y < bottom.y)
				isdispose = true;
		}
		newedge.ymax = top.y;
		newedge.x = (float)bottom.x;
		newedge.m = (float)(top.x - bottom.x)/(float)(top.y - bottom.y);
		//if(panduan)newedge.ymax;;int �ı�
		int subscript;// ���±�Ҫ������±�
		if (isdispose)
		{
			newedge.x += (int)newedge.m;
			subscript = bottom.y + 1 - ymin;
		} else {
			subscript = bottom.y - ymin;
		}
		NET[subscript].push_back(newedge);
	}
	return NET;
}

bool comp(const Edge & m1, const Edge & m2) {
	return m1.x < m2.x;
}
class del
{
private:
	int want_del;
public:
	del(int val){
		want_del = val;
	}
	bool operator() (const Edge& val) {
		return val.ymax == want_del; 
	}
};

list<Edge> CScanLinePloyFillView::creatAET(int y,vector<list<Edge>> NET,int ymin,list<Edge> oldAET,CDC* pDC)
{
	list<Edge> newAET = oldAET;
	int subscript = y - ymin;
	//��NET�ϵı� ����AET
	newAET.insert(newAET.end(),NET[subscript].begin(),NET[subscript].end());
	//����AET
	newAET.sort(comp);
	//���AET
	list<Edge>::iterator it;
	list<Edge>::iterator it2;
	list<Edge>::iterator itend = newAET.end();
	--itend;
	for(it = newAET.begin(); it != itend; it++){
		it2 = it;
		++it2;
		for(int i=(int)it->x;i<(int)it2->x;i++)
			pDC->SetPixel(i,y,RGB(255,0,0));
	}

	//����AET,ɾ��AET��ymax=y�ıߣ��� ymax>y �ıߵ�xmin ����1/k,����AET
	newAET.remove_if(del(y));
	for(it = newAET.begin(); it != newAET.end(); it++){
		it->x += it->m;
	}
	return newAET;
}

//************************************************************* STL end

void CScanLinePloyFillView::ployFill(CPoint point[], int num,CDC* pDC)
{

	if (num <= 0)
		return;

		int ymax=0,ymin=0;
		vector<list<Edge>> NET =  createNET(point, num, ymax, ymin);
		list<Edge> AET;
		for(int i=ymin;i<=ymax;i++){
			AET = creatAET(i,NET,ymin,AET,pDC);
		}
}

void CScanLinePloyFillView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	flag = true;
	Invalidate(TRUE);
	CView::OnRButtonDown(nFlags, point);
}


void CScanLinePloyFillView::OnClear()
{
	// TODO: Add your command handler code here
	flag = false;
	vertexNum = 0;
	Invalidate(TRUE);
}
