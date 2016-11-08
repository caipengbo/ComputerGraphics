
// ScanLinePloyFillView.h : CScanLinePloyFillView ��Ľӿ�
//

#pragma once
//�߽ṹ
struct Edge {
	int ymax;
	int x;//��͵��x
	float m;//б�ʵĵ���
	//Edge * next;
};

class CScanLinePloyFillView : public CView
{
protected: // �������л�����
	CScanLinePloyFillView();
	DECLARE_DYNCREATE(CScanLinePloyFillView)

// ����
public:
	CScanLinePloyFillDoc* GetDocument() const;

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
	virtual ~CScanLinePloyFillView();
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
	CPoint vertex[100];
	int vertexNum;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ScanLinePloyFillView.cpp �еĵ��԰汾
inline CScanLinePloyFillDoc* CScanLinePloyFillView::GetDocument() const
   { return reinterpret_cast<CScanLinePloyFillDoc*>(m_pDocument); }
#endif

