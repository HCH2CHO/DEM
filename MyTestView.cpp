
// MyTestView.cpp : CMyTestView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MyTest.h"
#endif
#include "afxwin.h"
#include "MyTestDoc.h"
#include "MyTestView.h"
#include "math.h"
#include <fstream>
#include "delaunay.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyTestView

IMPLEMENT_DYNCREATE(CMyTestView, CView)

BEGIN_MESSAGE_MAP(CMyTestView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMyTestView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_DrawLine, &CMyTestView::OnDrawline)
	ON_COMMAND(ID_ReadDEM, &CMyTestView::OnReadDEM)
	ON_COMMAND(ID_TurnTIN, &CMyTestView::OnTurnTIN)
	ON_COMMAND(ID_OnDrawDEM, &CMyTestView::OndrawDEM)
END_MESSAGE_MAP()

// CMyTestView ����/����

CMyTestView::CMyTestView()
{
	// TODO: �ڴ˴���ӹ������
	m_bPress=FALSE;
	m_pEle=NULL;
	m_bDrawDEM=FALSE;
	m_bTurnTIN=FALSE;
}

CMyTestView::~CMyTestView()
{
	
	if(m_pEle)
	{
		delete[] m_pEle;
		m_pEle=NULL;
	}
	if(tinEle)
	{
		delete[] tinEle;
		tinEle=NULL;
	}
}

BOOL CMyTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMyTestView ����

void CMyTestView::OnDraw(CDC* pDC)
{
	CMyTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���

	
	if(m_bPress)
	{
		pDC->MoveTo(0,0);
		pDC->LineTo(10000,18000);
	}
	
	if(m_bDrawDEM)
	{
		DrawDEM(pDC);
	}

	if(m_bTurnTIN)
		DrawTIN(pDC);
}
// CMyTestView ��ӡ


void CMyTestView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMyTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMyTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMyTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMyTestView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMyTestView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMyTestView ���

#ifdef _DEBUG
void CMyTestView::AssertValid() const
{
	CView::AssertValid();
}

void CMyTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyTestDoc* CMyTestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyTestDoc)));
	return (CMyTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyTestView ��Ϣ�������


void CMyTestView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: �ڴ����ר�ô����/����û���
	CRect rc;
	GetClientRect(&rc);
	int nWidth=1200;
	int nHeight=900;
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportOrg(rc.left,rc.bottom);
	pDC->SetWindowOrg(0,0);
	pDC->SetViewportExt(rc.Width(),-rc.Height());
	pDC->SetWindowExt(nWidth,nHeight);

	CView::OnPrepareDC(pDC, pInfo);
}


void CMyTestView::OnDrawline()
{
	// TODO: �ڴ���������������
	m_bPress=TRUE;
	Invalidate();
}


void CMyTestView::OnReadDEM()
{
	// TODO: �ڴ���������������
	OpenFile("dem.dem");//ʹ�����·��,��ǰ�ļ��С���.vcproj�ļ���ͬһ�ļ����¡���exe�ļ���ͬһ�ļ�����

}

bool CMyTestView::OpenFile(char *szFile)
{
	
	ifstream is(szFile);
	if(is.bad())
		return FALSE;
	char message[300];
	is.seekg(0,ios::beg);

	for(;;)
	{
		char message[200];
		is.getline(message,sizeof(message)-1,'\n');//��ȡһ�У�����Ϊ�洢λ�ã���С��������
		CString s=CA2T(message);
		s.TrimLeft();      //�Զ�ȥ���ַ���ǰ��Ŀո�
		s.TrimRight();
		int nPos=s.Find(_T(":"));//���ص�һ�����ŵ�λ�ã�����ֵ
		int nLen=s.GetLength();
		if(nPos==-1)
			continue;

		CString s1=s.Left(nPos);
		CString s2=s.Right(nLen-nPos-1);

		if(s1.CompareNoCase(_T("DataMark"))==0)
		{
			if(s2.CompareNoCase(_T(" CNSDTF-DEM"))!=0)
			{
				AfxMessageBox(_T("��ǰDEM�ļ��ǹ��ҿռ����ݽ���ģʽ��"));
				return FALSE;
			}
		}
		else if(s1.CompareNoCase(_T("Version"))==0)   //�汾���б�
		{
		}
		else if(s1.CompareNoCase(_T("Unit"))==0)      //���굥λ
		{
		}
		else if(s1.CompareNoCase(_T("Alpha"))==0)
		{
		}
		else if(s1.CompareNoCase(_T("Compress"))==0)
		{
		}
		else if(s1.CompareNoCase(_T("X0"))==0)     //���Ͻ�x����
			m_fLeftTopX=atof(CT2A(s2));
		else if(s1.CompareNoCase(_T("Y0"))==0)     //y����
			m_fLeftTopY=atof(CT2A(s2));
		else if(s1.CompareNoCase(_T("DX"))==0)
			m_fGridX=atof(CT2A(s2));
		else if(s1.CompareNoCase(_T("DY"))==0)
			m_fGridY=atof(CT2A(s2));
		else if(s1.CompareNoCase(_T("Row"))==0)    //dem����
			m_nRow=atoi(CT2A(s2));
		else if(s1.CompareNoCase(_T("Col"))==0)    //dem����
			m_nCol=atoi(CT2A(s2));
		else if(s1.CompareNoCase(_T("ValueType"))==0)     //�߳�ֵ����
		{
		}
		else if(s1.CompareNoCase(_T("Hzoom"))==0)
		{
			m_nScale=atoi(CT2A(s2));
		}
		else if(s1.CompareNoCase(_T("Coordinate"))==0)
		{
			break;
		}
	}

	if(fabs(m_fGridX-m_fGridY)>0.001)
	{
		AfxMessageBox(_T("ϵͳҪ��DEMΪ����������"));
		return FALSE;
	}

	//����Ϊ�߳�ָ��m_pEle����ռ䣬����ȡ�߳�ֵ
	m_pEle=new int[m_nRow*m_nCol];
	int i,j;
	for(i=0;i<m_nRow;i++)
	{
		//����ѭ����DEM�����Ͻ������������ϵ�������
		LONG nRowPos=i*m_nCol;      //��ά��������Ӧ��һά��ţ�From 0��

		//����DEM�е�һ�����ļ��еĴ������� nCharRowofCol
		int nCharRowofCol;
		if(m_nCol%10==0)
			nCharRowofCol=m_nCol/10;
		else
			nCharRowofCol=m_nCol/10+1;
		int nCurZ[10];

		for(j=0;j<nCharRowofCol;j++)
		{
			is.getline(message,sizeof(message)-1,'\n');    //HEADER
			int nFields=sscanf_s(message,"%d%d%d%d%d%d%d%d%d%d", 
				&nCurZ[0],&nCurZ[1],&nCurZ[2],&nCurZ[3],&nCurZ[4],
				&nCurZ[5],&nCurZ[6],&nCurZ[7],&nCurZ[8],&nCurZ[9]);
			
			

 			for(int k=0;k<nFields;k++)
			{
				int nPos_of_Point;     //��ǰ��������DEM�е�λ��
				nPos_of_Point=nRowPos+j*10+k;
				m_pEle[nPos_of_Point]=nCurZ[k];//��λ����Ϊm
			}
		}
	}
	is.close();
	return TRUE;
	
}

void CMyTestView::OndrawDEM()
{
	// TODO: �ڴ���������������
	m_bDrawDEM=TRUE;
	if(m_bDrawDEM)
		Invalidate();
}

void CMyTestView::DrawDEM(CDC* pDC)
{
	CPen *createPen=new CPen[1000];
	int H,rgb;
	for(int k=0;k<1000;k++)
	{
		if(k<250)
			createPen[k].CreatePen(PS_SOLID,2,RGB(0,k,250));
		else if(k>=250&&k<500)
			createPen[k].CreatePen(PS_SOLID,2,RGB(0,250,500-k));
		else if(k>=500&&k<750)
			createPen[k].CreatePen(PS_SOLID,2,RGB(k-500,250,0));
		else if(k>=750&&k<1000)
			createPen[k].CreatePen(PS_SOLID,2,RGB(250,1000-k,0));
	}
	for(int i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
				H=m_pEle[i*m_nCol+j]/1000;//��λ�����m
				rgb=H/5;
				if(rgb<0)
					rgb=0;
				else if(rgb>1000)
					rgb=1000;//�ų��쳣
				pDC->SelectObject(&createPen[rgb]); 
				pDC->MoveTo(j,900-i);
				pDC->LineTo(j+1,900-(i+1));
		}
	}
	if(createPen)
	{delete[] createPen;createPen=NULL;}
}


void CMyTestView::OnTurnTIN()
{
	// TODO: �ڴ���������������
	m_bTurnTIN=TRUE;
	m_bDrawDEM=FALSE;
	turnTIN();
	if(m_bTurnTIN)
		Invalidate();
}

void CMyTestView::turnTIN()
{
	TIN *tinEle0=new TIN[m_nRow*m_nCol];
	//VIP��Ҫ�㱣����
	pointCount=0;
	double value=0;
	double AE,CG,BF,DH;

	for(int i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
			if(i==0||j==0||i==m_nRow-1||j==m_nCol-1)
				continue;
			if(j==1||j==2)
				continue;//ÿ��ǰ�������ݴ�������

			AE=abs((m_pEle[(i-1)*m_nCol+j-1]+m_pEle[(i+1)*m_nCol+j+1])/2-m_pEle[i*m_nCol+j])/1000;
			CG=abs((m_pEle[(i-1)*m_nCol+j+1]+m_pEle[(i+1)*m_nCol+(j-1)])/2-m_pEle[i*m_nCol+j])/1000;
			BF=abs((m_pEle[(i+1)*m_nCol+j]+m_pEle[(i-1)*m_nCol+j])/2-m_pEle[i*m_nCol+j])/1000;
			DH=abs((m_pEle[i*m_nCol+j+1]+m_pEle[i*m_nCol+j-1])/2-m_pEle[i*m_nCol+j])/1000;

			value=(AE+CG+BF+DH)/4;
			//m_fGridX,m_fGridY���������⣬��Ҫһ������ϵ��.�߳�Ϊ���ͣ���λmm
			if(value>25)
			{
				tinEle0[pointCount].x=j;
				tinEle0[pointCount].y=i;
				tinEle0[pointCount].z=0;
				//tinEle0[pointCount].z=m_pEle[i*m_nCol+j]/1000;
				pointCount+=1;
			}
		}
	}

	tinEle=new TIN[pointCount];
	for(int k=0;k<pointCount;k++)
	{
		tinEle[k]=tinEle0[k];
	}

	if(tinEle0)
	{
		delete[]tinEle0; tinEle0=NULL;
	}


	//delaunay�����ʷ��㷨
	TIN framePoint[4]={{0,0,0},{1200,0,0},{1200,900,0},{0,900,0}};

    PointArray p; 
    double xx,yy,zz; 
    int nodeSize; 
    for (int i=0;i<4;i++)//����4��߿�� 
    { 
		xx=framePoint[i].x;
		yy=framePoint[i].y;
		zz=framePoint[i].z;
		Point tmp={xx,yy,zz}; 
        p.push_back(tmp); 
    } 
	Delaunay MyMesh(p[0],p[1],p[2],p[3]);//ʵ����Delaunay�� 

    nodeSize=pointCount; 
    for (int i=0;i<nodeSize;i++)//����ÿ���ڵ������ 
    { 
        xx=tinEle[i].x;
		yy=tinEle[i].y;
		zz=tinEle[i].z;
        MyMesh.AddPoint(xx,yy,zz); 
    } 
    MyMesh.Delete_Frame();//ɾ����߿�

	myPts=MyMesh.m_Pts;
	myEdges=MyMesh.m_Edges;

	/*
    MyMesh.Boundary_Recover(203,466); //???
    MyMesh.Boundary_Recover(467,487); 
    MyMesh.Boundary_Recover(488,511); 
    MyMesh.Boundary_Recover(512,537);//���϶��ǻָ�ָ���߽� 
    MyMesh.output();//����ӦANSYS��������� 
	*/
}

void CMyTestView::DrawTIN(CDC* pDC)
{
	for(int k=0;k<int(myEdges.size());k++)
	{
		pDC->MoveTo(myPts[myEdges[k].left].x,900-myPts[myEdges[k].left].y);
		pDC->LineTo(myPts[myEdges[k].right].x,900-myPts[myEdges[k].right].y);
	}
}