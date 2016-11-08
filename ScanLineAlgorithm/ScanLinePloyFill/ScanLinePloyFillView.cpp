
// ScanLinePloyFillView.cpp : CScanLinePloyFillView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ScanLinePloyFill.h"
#endif

#include "ScanLinePloyFillDoc.h"
#include "ScanLinePloyFillView.h"
#include<algorithm>
#include <vector>
#include<list>
#include <iterator>
#include<iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScanLinePloyFillView

IMPLEMENT_DYNCREATE(CScanLinePloyFillView, CView)

BEGIN_MESSAGE_MAP(CScanLinePloyFillView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScanLinePloyFillView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CScanLinePloyFillView 构造/析构

CScanLinePloyFillView::CScanLinePloyFillView()
{
	// TODO: 在此处添加构造代码

	vertexNum = 0;
}

CScanLinePloyFillView::~CScanLinePloyFillView()
{
}

BOOL CScanLinePloyFillView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CScanLinePloyFillView 绘制
Edge * creatAET(int y,Edge **NET,int ymin,Edge *oldAET,CDC* pDC);


//Using STL *************************************************************************

vector<list<Edge>> createNET(CPoint point[], int num, int& ymax, int& ymin)
{
	int len = 0;
	ymax = point[0].y;
	ymin = point[0].y;
	for (int i=0; i<num; i++)
	{
		//寻找最低，最高y
		if (point[i].y > ymax) {
			ymax = point[i].y;
		}
		if (point[i].y < ymin)
		{
			ymin = point[i].y;
		}
	}
	len = ymax - ymin + 1;
	// 新边表头数组 并初始化
	vector<list<Edge>> NET(len);
	//在表头数组插入新边
	for (int i=0; i<num; i++)
	{
		int temp = (i+1)%num;
		Edge newedge;
		CPoint top,bottom;
		bool isdispose = false; //是否处理
		if (point[i].y > point[temp].y)
		{
			top = point[i];
			bottom = point[temp];
			//处理特殊点
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
		newedge.x = bottom.x;
		newedge.m = (float)(top.x - bottom.x)/(float)(top.y - bottom.y);
		//if(panduan)newedge.ymax;;int 改变
		int subscript;// 求新边要插入的下标
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

list<Edge> creatAET(int y,vector<list<Edge>> NET,int ymin,list<Edge> oldAET,CDC* pDC)
{
	list<Edge> newAET = oldAET;
	int subscript = y - ymin;
	//将NET上的边 插入AET
	newAET.insert(newAET.end(),NET[subscript].begin(),NET[subscript].end());
	//排序AET
	newAET.sort(comp);
	//填充AET
	list<Edge>::iterator it;
	list<Edge>::iterator it2;
	list<Edge>::iterator itend = newAET.end();
	--itend;
	for(it = newAET.begin(); it != itend; it++){
		it2 = it;
		++it2;
		for(int i=it->x;i<it2->x;i++)
			pDC->SetPixel(i,y,RGB(255,0,0));
		}
	
	//遍历AET,删除AET中ymax=y的边，把 ymax>y 的边的xmin 递增1/k,更新AET
	newAET.remove_if(del(y));
	for(it = newAET.begin(); it != newAET.end(); it++){
		it->x += (int)(it->m);
	}
	return newAET;
}

//************************************************************* STL end

void CScanLinePloyFillView::OnDraw(CDC* pDC)
{
	CScanLinePloyFillDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	/*
	vertexNum = 5;
	vertex[0].x = 10;
	vertex[0].y = 10;
	vertex[1].x = 16;
	vertex[1].y = 15;
	vertex[2].x = 12;
	vertex[2].y = 20;
	vertex[3].x = 8;
	vertex[3].y = 16;
	vertex[4].x = 6;
	vertex[4].y = 14;*/
	// TODO: 在此处为本机数据添加绘制代码
	vertexNum = 3;
	vertex[0].x = 200;
	vertex[0].y = 200;
	vertex[1].x = 200;
	vertex[1].y = 400;
	vertex[2].x = 0;
	vertex[2].y = 350;
	vertex[3].x = 0;
	vertex[3].y = 300;
	for(int i=0;i<vertexNum;i++) {
		pDC->MoveTo(vertex[i]);
		pDC->LineTo(vertex[(i+1)%vertexNum]);
	}

	//

	int ymax=0,ymin=0;
	vector<list<Edge>> NET =  createNET(vertex, vertexNum, ymax, ymin);
	list<Edge> AET;
	for(int i=ymin;i<=ymax;i++){
		AET = creatAET(i,NET,ymin,AET,pDC);
	}
}


// CScanLinePloyFillView 打印


void CScanLinePloyFillView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CScanLinePloyFillView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CScanLinePloyFillView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CScanLinePloyFillView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CScanLinePloyFillView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CScanLinePloyFillView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CScanLinePloyFillView 诊断

#ifdef _DEBUG
void CScanLinePloyFillView::AssertValid() const
{
	CView::AssertValid();
}

void CScanLinePloyFillView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CScanLinePloyFillDoc* CScanLinePloyFillView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScanLinePloyFillDoc)));
	return (CScanLinePloyFillDoc*)m_pDocument;
}
#endif //_DEBUG


// CScanLinePloyFillView 消息处理程序


void CScanLinePloyFillView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	vertex[vertexNum] = point;
	vertexNum++;
	Invalidate(TRUE);
	CView::OnLButtonDown(nFlags, point);
}

