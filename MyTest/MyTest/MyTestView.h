
// MyTestView.h : CMyTestView ��Ľӿ�
//

#pragma once
#include "delaunay.h"

class CMyTestView : public CView
{
protected: // �������л�����
	CMyTestView();
	DECLARE_DYNCREATE(CMyTestView)

// ����
public:
	CMyTestDoc* GetDocument() const;

// ����
public:
	bool OpenFile(char *szFile);
	void turnTIN();
// ��д
public:
	bool m_bPress;
	bool m_bDrawDEM;
	bool m_bTurnTIN;

	float m_fLeftTopX;//���Ͻ�x����
	float m_fLeftTopY;//���Ͻ�y����
	float m_fGridX;//x��������߳�
	float m_fGridY;//y��������߳�
	int m_nRow;//y����������
	int m_nCol;//x����������
	int m_nScale;//�̷߳Ŵ���
	int *m_pEle;//�߳̾���ָ��



	struct TIN{
		double x;
		double y;
		double z;
	};
	TIN *tinEle;
	int pointCount; //��¼�����ĸ̵߳������
	PointArray myPts;//m_Pts���ڴ洢���е� 
    EdgeArray myEdges;//m_Edges���ڴ洢���б� 

	void DrawDEM(CDC* pDC);
	void DrawTIN(CDC* pDC);
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMyTestView();
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
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnDrawline();
	afx_msg void OnReadDEM();
	afx_msg void OnTurnTIN();
	afx_msg void OndrawDEM();
};

#ifndef _DEBUG  // MyTestView.cpp �еĵ��԰汾
inline CMyTestDoc* CMyTestView::GetDocument() const
   { return reinterpret_cast<CMyTestDoc*>(m_pDocument); }
#endif

