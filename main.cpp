#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;
#include "linkQueue.h"
#define CITY_NUM 60
#define INF 63533
int visited[CITY_NUM]={0};
typedef string ElemType_name;
typedef int ElemType_loc;

//////////////////////////////
#define MAX_station 50
#define TIME_TABLE 50
/*
 *定义时刻表
 */
 typedef struct{
    string station_No;
    string station_start_time;
    string station_arrive_time;
    string station_through_time;
    int train_num;
 } Train_time;
/*
 *边链表
 */
typedef struct eNode{
    int num; //在顶点表中的编号
    ElemType_name city_name;
    ElemType_loc city_distance;
    Train_time train_table[TIME_TABLE]; //时刻表
    struct eNode * next;
} EdgeNode;
/*
 *顶点表
 */
 typedef struct vNode{
     int num;//在顶点表中的编号
    ElemType_name city_name;
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
    for(int i=0; i<CITY_NUM; i++)
    {
        G.VerList[i].firstEdge=NULL;
//        for(int j=0; j<TIME_TABLE; j++)
//        {
//            G.VerList[i].firstEdge->city_distance=INF;
//            G.VerList[i].firstEdge->city_name="";
//            G.VerList[i].firstEdge->num=0;
//            G.VerList[i].firstEdge->next=NULL;
//            G.VerList[i].firstEdge->train_table[j].station_No="";
//            G.VerList[i].firstEdge->train_table[j].station_start_time="";
//            G.VerList[i].firstEdge->train_table[j].station_arrive_time="";
//            G.VerList[i].firstEdge->train_table[j].station_through_time="";
//        }
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
//////////////////////////////////////////////
/*
 *中文字符串模糊匹配
 */
vector<int> stringToVecInt(const string &str)
{
    union {
        char c[2];
        int  i;
    } convert;

    // 段位清零
    convert.i = 0;

    vector<int> vec;

    for (unsigned i = 0; i < str.length(); i++) {
        // GBK编码首字符大于0x80
        if ((unsigned)str[i] > 0x80) {
            // 利用union进行转化，注意是大端序
            convert.c[1] = str[i];
            convert.c[0] = str[i + 1];
            vec.push_back(convert.i);
            i++;
        } else
            // 小于0x80，为ASCII编码，一个字节
            vec.push_back(str[i]);
    }
    return vec;
}
bool inclus(const string &str,const string &msg)
{
    auto sour = stringToVecInt(str);
    auto find = stringToVecInt(msg);
    for(int i=0;i<sour.size();i++)
    {
      // cout<<sour[i]<<"<-->"<<find[i]<<endl;
       if(sour[i]==find[i]&&i==2)
        return true;
    }

    return false;
}
/////////////////////////////////////
/*
 *在给定的头指针的边链表内查找名字相同的城市
 */
void find_VerList_edgenode(EdgeNode * &head,string name,int order,string no,string start_time,
                           string end_time,string through_time)
{
    EdgeNode * p = head;
    while(p)
    {
           cout<<p->city_name<<" "<<name<<endl;
        if(inclus(p->city_name,name))
        {
            //p = new EdgeNode;
            p->train_table[order].station_No =no;
            p->train_table[order].station_start_time=start_time;
            p->train_table[order].station_arrive_time=end_time;
            p->train_table[order].station_through_time=through_time;
            return;
        }
        p = p->next;
    }
}
///////////////////////////////////
void create_City_trainTable(Graph &G)
{
    int i=0,j=0,k=0;
    string no,start,arrive,time_start,time_end,time_through,flag;
    ifstream in("time.txt");
    while(!in.eof())
    {
        in>>no>>start>>arrive>>time_start>>time_end>>time_through;
        //start代表要出发的城市
        i = located(G,start);
        if(i==-1)
            continue;
        if(start!=flag)
        {
            j=0;
        }
        flag = start;//flag代表上一个start

      //  if(i!=-1)
        {
            /*
             *执行完此步后，相应的车次信息已存到边链表中
             *如北京->上海
             *存放的是北京到上海的所有车次信息
             */
            //cout<<no<<" "<<start<<" "<<arrive<<" "<<time_start<<" "<<time_end<<" "<<time_through<<endl;
           find_VerList_edgenode(G.VerList[i].firstEdge,arrive,j,no,time_start,time_end,time_through);
//           cout<<G.VerList[i].firstEdge->train_table[j].station_No<<" "
//                <<G.VerList[i].firstEdge->train_table[j].station_start_time<<" "
//                <<G.VerList[i].firstEdge->train_table[j].station_arrive_time<<" "
//                <<G.VerList[i].firstEdge->train_table[j].station_through_time<<" "
//                <<endl;
            j++;
        }
    }

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
      //  cout<<"距离: "<<G.VerList[v].city_name<<" -- : "<<minDist<<" KM"<<endl;
        solved[v]=1; //标记顶点v以找到最短距离，加入集合Ｓ中
        //对选中的顶点v，更新集合Ｖ—S中所有与Ｖ邻接的顶点距离vID的距离
        p = G.VerList[v].firstEdge;
        while(p)
        {
            j = p -> num; //取得v的邻接顶点编号
            if(!solved[j] &&minDist+p->city_distance < dist[j])
            {
                dist[j] = minDist + p->city_distance;  //更新顶点j到vID的最小距离
                path[j] = v;    //j的前驱顶点改为v
            }
            p = p->next;
        }
    }
}
/*输出最短路径*/
void PrintDijkstra(Graph &G,int path[],int dist[],int vID,int endID)
{
    int sPath[CITY_NUM];  //保存vID到目标顶点i的最短路径顶点
    int vPre;   //前驱顶点编号
    int top=-1; //保存最短路径上的顶点个数，以控制输出最短路径
    int i,j;
    for(i=0; i<G.VerNum; i++)
    {
       // cout<<G.VerList[vID].city_name<<" to "<<G.VerList[i].city_name;
        if(endID == G.VerList[i].num)
        {
            if(dist[i]==INF)
                cout<<"无可达路径。"<<endl;
            else
            {
                cout<<"　最短距离　"<<dist[i]<<endl;
            }
            top++;
            sPath[top]=i;   //sPath[0]保存目标顶点编号i
            vPre = path[i]; //取得顶点i的直接前驱编号，赋给vPre
            //从第i个顶点，迭代求前驱顶点，直到vID，保存最短路径到sPath[]
            while(vPre!=-1)
            {
                top++;
                sPath[top]=vPre;
                vPre = path[vPre];
            }
            //如果最短路径存在，依次打印vID到i顶点的最短路径顶点序列
            if(dist[i]!=INF)
            {
                for(j = top; j>=0; j--) //sPath[top]为指定的起始顶点vID
                {
                    cout<<G.VerList[sPath[j]].city_name<<" -> ";
                }
            }
            top = -1;
            cout<<endl;
        }
    }
}
////////////////////////////
void find_city_distance(Graph G)
{
    int vID =-1,endID=-1,path[CITY_NUM],dist[CITY_NUM];

    string start_city,end_city;
    cout<<"Start_city and ending_city : ";
    cin>> start_city;
    cin>>end_city;
    vID = located(G,start_city);
    endID=located(G,end_city);
    Dijkstra(G,path,dist,vID);
    PrintDijkstra(G,path,dist,vID,endID);

}

////////////////////////////
 int main()
 {
     Graph City;
     initial_Grapg(City);
     create_Graph(City);
    create_City_trainTable(City);
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
