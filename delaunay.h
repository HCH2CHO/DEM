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
}Point;//�������


typedef vector<Point> PointArray;//��������vector����

typedef struct 
{ 
    int left; 
    int right; 
    int count;//�ߵļ������������Ϊ0����ɾ���˱� 
}Edge;//�������


typedef vector<Edge> EdgeArray;//��������vector����

typedef struct 
{ 
    int v[3];//�����ε��������� 
    Edge s[3];//�����ε������� 
    double xc;//���������ԲԲ�ĵ�x���� 
    double yc;//���������ԲԲ�ĵ�y���� 
    double r;//���������Բ�İ뾶 
}Triangle;//������������


typedef vector<Triangle> TriangleArray;//�������������vector����

typedef vector<int> intArray;//����int���vector����

class Delaunay//����Delaunay�� 
{ 
public: 
    Delaunay(Point p1,Point p2,Point p3,Point p4); //Delaunay��Ĺ��캯����������߿� 
    ~Delaunay();//Delaunay�����������

	PointArray m_Pts;//m_Pts���ڴ洢���е� 
    EdgeArray m_Edges;//m_Edges���ڴ洢���б� 
    TriangleArray m_Tris;//m_Tris���ڴ洢���������� 

    bool AddPoint(double xx,double yy,double zz);//�������ʷ�ͼ���мӵ�ĺ��� 
    void Delete_Frame();//ɾ����߿� 
    void Boundary_Recover(int fromPoint,int toPoint);//�߽�ָ� 
    void output();//���ANSYS�������ļ� 
private: 
    void Cal_Centre(double &x_centre,double &y_centre,double &radius,int n1,int n2,int n3);//���������ε����ԲԲ������Ͱ뾶 
    void MakeTriangle(int n1,int n2,int n3);//����ָ������������� 
    bool inCircle(double xx,double yy,Triangle currentTris);//�жϵ��Ƿ���Բ�� 
    void DelTriangle(int n,EdgeArray &BoundEdges);//ɾ��ָ����������


}; 


#endif // DELAUNAY_H_INCLUDED