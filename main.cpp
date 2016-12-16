#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;
#include "linkQueue.h"
#define CITY_NUM 60
#define INF 63533
#define MAX_station 200
#define TIME_TABLE 2000
int visited[CITY_NUM]={0};
typedef string ElemType_name;
typedef int ElemType_loc;

//////////////////////////////
/*
 *定义时刻表
 */
 typedef struct{
    string station_no;
    string station_start_time;
    string station_arrive_time;
    string through_time;
 }Train_table;

/*
 *边链表
 */
typedef struct eNode{
    int num; //在顶点表中的编号
    Train_table train_table[TIME_TABLE];
    int trainNum;
    ElemType_name city_name;
    ElemType_loc city_distance;
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

void initial_Grapg(Graph &G)
{
    G.ArcNum=0;
    G.VerNum=0;
    for(int i=0; i<CITY_NUM; i++)
    {
        G.VerList[i].firstEdge=NULL;

    }
    cout<<"<-----initial Graph success----->\n";
}
void create_Graph(Graph &G)
{

    int i=0,j,distan,k=0;
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
////////////////////////////////////////////
int locatedGraphStartCity(Graph G,string city)
{
    for(int i=0;i<G.VerNum;i++)
    {
        if(inclus(G.VerList[i].city_name,city))
            return i;
    }
    return -1;
}
void writeInTime(EdgeNode* &fistedge,int order,string no,string arrive_city,string start_time,string end_time,string through_time)
{
    EdgeNode *p=fistedge;
    while(p)
    {
        if(inclus(p->city_name,arrive_city))
        {
            p->train_table[order].station_no=no;
            p->train_table[order].station_start_time=start_time;
            p->train_table[order].station_arrive_time=end_time;
            p->train_table[order].through_time=through_time;
            p->num=order;
                cout<<order<<" "<<p->train_table[order].station_no<<" "
        <<p->train_table[order].station_start_time<<" "
        <<p->train_table[order].station_arrive_time<<" "
        <<p->train_table[order].through_time<<endl;

            break;
        }
        p=p->next;
    }

}
/*
 *创建时刻表
 */
 void create_Graph_timeTable(Graph &G)
 {
    cout<<"----runing----"<<endl;
    int i=0,j=0,k=0,flag=0,count=0;
    string no,start,arrive,time_start,time_arrive,time_through;
    ifstream in("time.txt");
    while(!in.eof())
    {
        in>>no>>start>>arrive>>time_start>>time_arrive>>time_through;
        in.get();
        if(in.peek()=='\n')
            break;
        i = locatedGraphStartCity(G,start);
        if(i!=flag)
        {
            //当找到一个新城市时让j归０
            j=0;
        }
        flag=i; //记录开始城市的坐标
        //若没找到则提前结束这次循环
        if(i==-1)
            continue;
        else{
//                if(j>100)
//                    continue;
            writeInTime(G.VerList[i].firstEdge,j,no,arrive,time_start,time_arrive,time_through);
            //cout<<no<<" "<<start<<" "<<arrive<<" "<<time_start<<" "<<time_arrive<<" "<<time_through<<endl;
        }
        j++;
        count++;
    }
    cout<<count<<endl;

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
    create_Graph_timeTable(City);
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
