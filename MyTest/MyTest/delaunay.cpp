#include "delaunay.h"


Delaunay::Delaunay(Point p1,Point p2,Point p3,Point p4) 
{ 
    m_Pts.resize(4); 
    m_Pts[0]=p1; 
    m_Pts[1]=p2; 
    m_Pts[2]=p3; 
    m_Pts[3]=p4;//添加四个外边框点 
    m_Edges.resize(4); 
    Edge l1={0,1,-1}; 
    Edge l2={1,2,-1}; 
    Edge l3={0,3,-1}; 
    Edge l4={2,3,-1}; 
    m_Edges[0]=l1; 
    m_Edges[1]=l2; 
    m_Edges[2]=l3; 
    m_Edges[3]=l4;//添加四个外边框的边 
    MakeTriangle(0,1,2); 
    MakeTriangle(0,2,3);//添加初始的两个三角形 
}

Delaunay::~Delaunay()//清空Delaunay类的数据成员 
{ 
    m_Pts.resize(0); 
    m_Edges.resize(0); 
    m_Tris.resize(0); 
}

void Delaunay::MakeTriangle(int n1,int n2,int n3) 
{ 
    double x_centre,y_centre,radius; 
    Cal_Centre(x_centre,y_centre,radius,n1,n2,n3);//获得顶点为n1,n2,n3的三角形的外接圆圆心坐标和半径 
    Triangle newTriangle={{n1,n2,n3},{{n1,n2,1},{n2,n3,1},{n1,n3,1}},x_centre,y_centre,radius};//生成指定的三角形 
    m_Tris.push_back(newTriangle);//向m_Tris中添加新构造的三角形 
    int EdgeSzie=(int)m_Edges.size();//获得目前的边数 
    int flag; 
    for (int i=0;i<3;i++) 
    { 
        flag=1; 
        for(int j=0;j<EdgeSzie;j++)//通过循环判断新构造的三角形的各边是否已经存在于m_Edges中，如果存在则只增加该边的计数，否则添加新边 
        { 
            if (newTriangle.s[i].left==m_Edges[j].left&&newTriangle.s[i].right==m_Edges[j].right&&

            m_Edges[j].count!=-1) 

            {

                 flag=0;m_Edges[j].count+=1;break;

            } 
            else if(newTriangle.s[i].left==m_Edges[j].left&&newTriangle.s[i].right==m_Edges[j].right&& 

                      m_Edges[j].count==-1)

                   {

                       flag=0;

                       break;

                 } 
        } 
        if (flag==1) m_Edges.push_back(newTriangle.s[i]); 
    } 
}

void Delaunay::Cal_Centre(double &x_centre,double &y_centre,double &radius,int n1,int n2,int n3) 
{ 
    double x1,x2,x3,y1,y2,y3; 
    x1=m_Pts[n1].x; 
    y1=m_Pts[n1].y; 
    x2=m_Pts[n2].x; 
    y2=m_Pts[n2].y; 
    x3=m_Pts[n3].x; 
    y3=m_Pts[n3].y; 
    x_centre=((y2-y1)*(y3*y3-y1*y1+x3*x3-x1*x1)-(y3-y1)*(y2*y2-y1*y1+x2*x2-x1*x1))/(2*(x3-x1)*(y2-y1)-2*((x2-x1)*(y3-y1)));//计算外接圆圆心的x坐标 
    y_centre=((x2-x1)*(x3*x3-x1*x1+y3*y3-y1*y1)-(x3-x1)*(x2*x2-x1*x1+y2*y2-y1*y1))/(2*(y3-y1)*(x2-x1)-2*((y2-y1)*(x3-x1)));//计算外接圆圆心的y坐标 
    radius= sqrt((x1 - x_centre)*(x1 - x_centre) + (y1 - y_centre)*(y1 - y_centre));//计算外接圆的半径 
}

bool Delaunay::AddPoint(double xx,double yy,double zz) 
{ 
    EdgeArray BoundEdges;//BoundEdges用于存储在删除三角形后留下的边框，用于构造新的三角形 
    Point newPoint={xx,yy,zz}; 
    m_Pts.push_back(newPoint);//向m_Pts中添加新点 
    intArray badTriangle;//badTriangle用于存储不符合空圆规则的三角形的索引号 
    int TriSize=(int)m_Tris.size();//获得目前的三角形数 
    for (int i=0;i<TriSize;i++)//通过循环找到所有不符合空圆规则的三角形，并将其索引号存在badTriangle中 
    { 
        if (inCircle(xx,yy,m_Tris[i])==true) badTriangle.push_back(i); 
    } 
    for (int i=0;i<(int)badTriangle.size();i++)//通过循环删除所有不符合空圆规则的三角形，同时保留边框 
    { 
        DelTriangle(badTriangle[i],BoundEdges); 
        for (int j=i+1;j<(int)badTriangle.size();j++) badTriangle[j]-=1; 
    } 
    int PtSize=(int)m_Pts.size();//获得目前的点数 
    for (int i=0;i<(int)BoundEdges.size();i++)//生成新的三角形 
    { 
        if (PtSize-1<BoundEdges[i].left) MakeTriangle(PtSize-1,BoundEdges[i].left,BoundEdges[i].right); 
        else if (PtSize-1>BoundEdges[i].left && PtSize-1<BoundEdges[i].right) MakeTriangle(BoundEdges[i].left,PtSize-1,BoundEdges[i].right); 
        else MakeTriangle(BoundEdges[i].left,BoundEdges[i].right,PtSize-1); 
    } 
    return true; 
}

bool Delaunay::inCircle(double xx,double yy,Triangle currentTris)//判断点是否在三角形的外接圆内 
{ 
    double dis=sqrt((currentTris.xc-xx)*(currentTris.xc-xx)+(currentTris.yc-yy)*(currentTris.yc-yy)); 
    if (dis>currentTris.r) return false; 
    else return true; 
}

void Delaunay::DelTriangle(int n,EdgeArray &BoundEdges) 
{ 
    for (int i=0;i<3;i++) 
    { 
        for (int j=0;j<(int)m_Edges.size();j++) 
        { 
            if (m_Edges[j].left==m_Tris[n].s[i].left&&m_Edges[j].right==m_Tris[n].s[i].right) 
            { 
                if (m_Edges[j].count==2)//若要删除三角形的一边的计数为2，则将其计数减1，并将其压入BoundEdges容器中 
                { 
                    m_Edges[j].count=1; 
                    BoundEdges.push_back(m_Edges[j]); 
                } 
                else if (m_Edges[j].count==-1) BoundEdges.push_back(m_Edges[j]);//如果是外边框，则直接压入BoundEdges容器中 
                else if (m_Edges[j].count==1)//如果删除三角形的一边的计数为1，则删除该边，同时查看BoundEdges中是否有此边，若有，则删除 
                { 
                    for (int k=0;k<(int)BoundEdges.size();k++) 
                    { 
                        if (BoundEdges[k].left==m_Edges[j].left&&BoundEdges[k].right==m_Edges[j].right) 
                        { 
                            BoundEdges.erase(BoundEdges.begin()+k); 
                            break; 
                        } 
                    } 
                    m_Edges.erase(m_Edges.begin()+j); 
                    j--; 
                } 
                break; 
            } 
        } 
    } 
    m_Tris.erase(m_Tris.begin()+n);//删除该三角形 
}

void Delaunay::Delete_Frame()//删除外边框 
{ 
    EdgeArray BoundEdges; 
    for (int i=0;i<4;i++) m_Pts.erase(m_Pts.begin()); 
    for (int i=0;i<(int)m_Tris.size();i++) 
    { 
        if (m_Tris[i].v[0]==0||m_Tris[i].v[0]==1||m_Tris[i].v[0]==2||m_Tris[i].v[0]==3) 
        { 
            DelTriangle(i,BoundEdges); 
            BoundEdges.resize(0); 
            i--; 
        } 
        else 
        { 
            for (int j=0;j<3;j++) 
            { 
                m_Tris[i].v[j]-=4; 
                m_Tris[i].s[j].left-=4; 
                m_Tris[i].s[j].right-=4; 
            } 
        } 
    } 
    for (int i=0;i<4;i++) m_Edges.erase(m_Edges.begin()); 
    for (int i=0;i<(int)m_Edges.size();i++) 
    { 
        m_Edges[i].left-=4; 
        m_Edges[i].right-=4; 
    } 
}

void Delaunay::Boundary_Recover(int fromPoint,int toPoint)//恢复由指定点组成的边界 
{ 
    EdgeArray BoundEdges; 
    for (int i=0;i<(int)m_Tris.size();i++) 
    { 
        if (m_Tris[i].v[0]>=(fromPoint-1)&&m_Tris[i].v[2]<=(toPoint-1)) //???
        { 
            DelTriangle(i,BoundEdges); 
            BoundEdges.resize(0); 
            i--; 
        } 
    } 
}


