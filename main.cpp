#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;
#include "linkQueue.h"
#include "train.h"
#define CITY_NUM 60
#define INF 63533
int visited[CITY_NUM]={0};
typedef string ElemType_name;
typedef int ElemType_loc;

//////////////////////////////
/*
 *边链表
 */
typedef struct eNode{
    int num; //在顶点表中的编号
    ElemType_name city_name;
    ElemType_loc city_distance;
   // Train_time train_table; //时刻表
    struct eNode * next;
} EdgeNode;
/*
 *顶点表
 */
 typedef struct vNode{
     int num;//在顶点表中的编号
    ElemType_name city_name;
    ElemType_loc city_distance;
  //  Train_time train_table; //时刻表
    EdgeNode * firstEdge;
 } VerNode;
 /*
  *图的整体结构
  */
 typedef struct GraphAdjLinkList
 {
    VerNode VerList[CITY_NUM];
    int VerNum;
    int ArcNum;
 } Graph;
 int located(Graph G,ElemType_name name);
/////////////////////////
void initial_Grapg(Graph &G)
{
    G.ArcNum=0;
    G.VerNum=0;
    for(int i=0;i<CITY_NUM;i++)
    {
        G.VerList[i].city_distance=-1;
        G.VerList[i].city_name="";
        G.VerList[i].firstEdge=NULL;
    }
    cout<<"<-----initial Graph success----->\n";
}
void create_Graph(Graph &G)
{
    int i=0,j,distan;
    ElemType_name city,city1,city2,flag1,flag2;

    EdgeNode * p;
    ////从文件中读取顶点
    ifstream in("city.txt");
    while(!in.eof())
    {
        in>>city;
        if(city == G.VerList[i-1].city_name)
            continue;
        G.VerList[i].num=i;
        G.VerList[i].city_name=city;
     //   cout<<G.VerList[i].city_name<<endl;
        i++;
    }
    G.VerNum=i;
    in.close();
    /*顶点读入完成*/
    ////////////////////
    /*从文件中读边*/
    ifstream fin("basedata.txt");
    while(!fin.eof())
    {
       fin>>city1>>city2>>distan;
       ///////////////////
       i = located(G,city1);
       j = located(G,city2);
        if(flag1==city1&&flag2==city2)
            continue;
       //  cout<<i<<"-->"<<G.VerList[i].city_name<<" "<<j<<"-->"<<G.VerList[j].city_name<<endl;
        flag1 = city1;
        flag2 = city2;
       ///////////////////
       p = new EdgeNode;
       p ->num=j;
       p ->city_name=city2;
       p ->city_distance=distan;
       p ->next=G.VerList[i].firstEdge;
       G.VerList[i].firstEdge=p;
       G.ArcNum++;
    }
    //cout<<G.ArcNum;
    fin.close();
    cout<<"<-----create Grate success----->\n";
}
///////////////////////////////////
void BFS(Graph G,int verID)
{
    int u;
    EdgeNode * p;
    linkQueue Q;
    initQueue(Q);
    cout<<G.VerList[verID].city_name<<" ";
    visited[verID]=1;
    enterQueue(Q,verID);
    while(!queueEmpty(Q))
    {
        outQueue(Q,u);
        p = G.VerList[u].firstEdge;
        while(p)
        {
            if(!visited[(p->num)])
            {
                cout<<G.VerList[p->num].city_name<<" ";
                visited[p->num]=1;
                enterQueue(Q,p->num);
            }
            p = p->next;
        }
    }
}
/////////////////////////
 /*
  *从文件中读取数据
  */
  void read()
  {
      ifstream in("basedata.txt");
      string start,end_,loc;
      while(!in.eof())
        {
            in>>start>>end_>>loc;
            cout<<start<<" "<<end_<<" "<<loc<<endl;
        }
        in.close();
  }
////////////////////////////
/*
 *给定顶点，求解次点与网中其他顶点的最短路径
　*vid指定起始顶点的编号
 *path，返回最短路径信息
 *dist  返回最短距离值
 */
void Dijkstra(Graph G,int path[],int dist[],int vID)
{
    int solved[CITY_NUM]={0}; //标记是否求出最短路径
    int i,j,v;
    ElemType_loc minDist;//保存最短距离值
    EdgeNode * p; //指向边链表结点的指针，edgenode为边链表结点类型
    //初始化距离数组dist[]，路径数组path[]
    for(i=0;i<G.VerNum;i++)
    {
        path[i]=-1; //所有顶点的前驱置为-1
        dist[i]=INF;    //所有顶点初始距离置为无穷大
    }
    //处理顶点vId
    solved[vID]=1;  //标记vID已经处理
    dist[vID]=0;
    path[vID]=-1;
    //从邻接表初始化dist[]和path[]
    p = G.VerList[vID].firstEdge; //顶点vID的边链表头指针
    while(p)
    {
        v = p->num; //取得顶点vID的邻接顶点编号
        dist[v]=p->city_distance; //取得viD与v之间的权值，赋值给dist[v]
        path[v]=vID;  //顶点v的前驱为vID
        p = p->next;
    }
    //依次找出余下n-1个顶点加入已求解集合S中
    for(i=1;i<G.VerNum;i++)
    {
        minDist=INF;
        //在未解顶点中寻找据vID距离最近的顶点，编号保存到v;
        for(j=0;j<G.VerNum;j++)
        {
            if(!solved[j]&&dist[j]<minDist)
            {
                v = j; //j为未解顶点集V-S中候选的距离vid最近的顶点
                minDist=dist[j];
            }
        }
        //在已解顶点集S中与未解顶点集Ｖ-Ｓ没有相邻的顶点，算法退出
        if(minDist==INF)
            return;
        //输出本次选择的顶点距离
        cout<<"选择顶点:"
    }

}
////////////////////////////
 int main()
 {
     Graph City;
     Train_time train_table;
     initial_train(train_table);
     initial_Grapg(City);

     create_train_start(train_table);
     creat_train_time(train_table);
     create_Graph(City);
   //  BFS(City,0);
    //read();
    return 0;
 }
int located(Graph G,ElemType_name name)
{
    int key=-1,i=0;
    while(G.VerList[i].city_name!=name&&i!=G.VerNum)
    {
        i++;
    }
    key=i;
    return key;
}
