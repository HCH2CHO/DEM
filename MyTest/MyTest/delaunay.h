#ifndef DELAUNAY_H_INCLUDED 
#define DELAUNAY_H_INCLUDED 
#include <cstdlib> 
#include <iostream> 
#include <cstring> 
#include <string> 
#include <fstream> 
#include <math.h> 
#include <vector>


using namespace std;

typedef struct 
{ 
    double x; 
    double y; 
    double z; 
}Point;//定义点类


typedef vector<Point> PointArray;//定义点类的vector容器

typedef struct 
{ 
    int left; 
    int right; 
    int count;//边的计数，如果计数为0，则删除此边 
}Edge;//定义边类


typedef vector<Edge> EdgeArray;//定义边类的vector容器

typedef struct 
{ 
    int v[3];//三角形的三个顶点 
    Edge s[3];//三角形的三条边 
    double xc;//三角形外接圆圆心的x坐标 
    double yc;//三角形外接圆圆心的y坐标 
    double r;//三角形外接圆的半径 
}Triangle;//定义三角形类


typedef vector<Triangle> TriangleArray;//定义三角形类的vector容器

typedef vector<int> intArray;//定义int类的vector容器

class Delaunay//定义Delaunay类 
{ 
public: 
    Delaunay(Point p1,Point p2,Point p3,Point p4); //Delaunay类的构造函数，创建外边框 
    ~Delaunay();//Delaunay类的析构函数

	PointArray m_Pts;//m_Pts用于存储所有点 
    EdgeArray m_Edges;//m_Edges用于存储所有边 
    TriangleArray m_Tris;//m_Tris用于存储所有三角形 

    bool AddPoint(double xx,double yy,double zz);//向已有剖分图形中加点的函数 
    void Delete_Frame();//删除外边框 
    void Boundary_Recover(int fromPoint,int toPoint);//边界恢复 
    void output();//输出ANSYS命令流文件 
private: 
    void Cal_Centre(double &x_centre,double &y_centre,double &radius,int n1,int n2,int n3);//计算三角形的外接圆圆心坐标和半径 
    void MakeTriangle(int n1,int n2,int n3);//生成指定顶点的三角形 
    bool inCircle(double xx,double yy,Triangle currentTris);//判断点是否在圆内 
    void DelTriangle(int n,EdgeArray &BoundEdges);//删除指定的三角形


}; 


#endif // DELAUNAY_H_INCLUDED