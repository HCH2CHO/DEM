
// MyTestView.h : CMyTestView 类的接口
//

#pragma once
#include "delaunay.h"

class CMyTestView : public CView
{
protected: // 仅从序列化创建
	CMyTestView();
	DECLARE_DYNCREATE(CMyTestView)

// 特性
public:
	CMyTestDoc* GetDocument() const;

// 操作
public:
	bool OpenFile(char *szFile);
	void turnTIN();
// 重写
public:
	bool m_bPress;
	bool m_bDrawDEM;
	bool m_bTurnTIN;

	float m_fLeftTopX;//左上角x坐标
	float m_fLeftTopY;//左上角y坐标
	float m_fGridX;//x方向网格边长
	float m_fGridY;//y方向网格边长
	int m_nRow;//y方向网格数
	int m_nCol;//x方向网格数
	int m_nScale;//高程放大倍率
	int *m_pEle;//高程矩阵指针



	struct TIN{
		double x;
		double y;
		double z;
	};
	TIN *tinEle;
	int pointCount; //记录保留的高程点的数量
	PointArray myPts;//m_Pts用于存储所有点 
    EdgeArray myEdges;//m_Edges用于存储所有边 

	void DrawDEM(CDC* pDC);
	void DrawTIN(CDC* pDC);
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMyTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // MyTestView.cpp 中的调试版本
inline CMyTestDoc* CMyTestView::GetDocument() const
   { return reinterpret_cast<CMyTestDoc*>(m_pDocument); }
#endif

