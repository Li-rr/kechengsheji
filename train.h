#define MAX_station 50
#define TIME_TABLE 150
/*
 *定义时刻表
 */
typedef struct {
    string station_No[TIME_TABLE];
    string station_arrive[TIME_TABLE];
    string station_start_time[TIME_TABLE];
    string station_arrive_time[TIME_TABLE];
    string station_through_time[TIME_TABLE];
} Arrive;
typedef struct{
   Arrive arrive_station[MAX_station];
   string station_start[MAX_station];
   int station_num;
} Train_time;
//  void read()
//  {
//      ifstream in("basedata.txt");
//      string start,end_,loc;
//      while(!in.eof())
//        {
//            in>>start>>end_>>loc;
//            cout<<start<<" "<<end_<<" "<<loc<<endl;
//        }
//        in.close();
//  }
void initial_train(Train_time &time)
{

    for(int i=0;i<MAX_station;i++)
    {
        time.station_start[i]="";
        for(int j=0;j<TIME_TABLE;j++)
        {
        time.arrive_station[i].station_No[j]="";
        time.arrive_station[i].station_arrive[j]="";
        time.arrive_station[i].station_start_time[j]="";
        time.arrive_station[i].station_arrive_time[j]="";
        time.arrive_station[i].station_through_time[j]="";
        }
    }
    cout<<"\n<-----initial train time success----->\n";
}
void create_train_start(Train_time &traintime)
{
    int i=0;
    ifstream in("city.txt");
    string city;
    while(!in.eof())
    {
        in>>city;
        if(city == traintime.station_start[i-1])
            continue;
        //cout<<city<<endl;
        traintime.station_start[i]=city;
        //cout<<traintime.station_start[i]<<endl;
        i++;
    }
    traintime.station_num=i;
    //cout<<traintime.station_num<<endl;
    in.close();
}
int located_trainTable_startCity(Train_time traintime,string city)
{
    int loc=-1;
    for(int i=0;i< traintime.station_num;i++)
    {
        if(traintime.station_start[i]==city)
        {
            loc = i;
            break;
        }
    }
    return loc;
}
void creat_train_time(Train_time &traintime)
{
    cout<<"程序正在运行，请耐心等待!"<<endl;
    int i=0,j=0,k=0;
    string no,start,arrive,time_start,time_arrive,time_through,flag;
    ifstream in("time.txt");
    while(!in.eof())
    {
        in>>no>>start>>arrive>>time_start>>time_arrive>>time_through;
        /*start代表出发的城市*/
        i = located_trainTable_startCity(traintime,start);
        if(start!=flag)
        {
            j=0;
        }
        flag = start;   //flag代表上一个start
        if(i!=-1) //若找到则进行赋值
        {
            traintime.arrive_station[i].station_No[j]=no;
            traintime.arrive_station[i].station_arrive[j]=arrive;
            traintime.arrive_station[i].station_start_time[j]=time_start;
            traintime.arrive_station[i].station_arrive_time[j]=time_arrive;
            traintime.arrive_station[i].station_through_time[j]=time_through;
//            cout<<traintime.arrive_station[i].station_No[j]<<" - "
//                <<traintime.station_start[i]<<" - "
//                <<traintime.arrive_station[i].station_arrive[j]<<" - "
//                <<traintime.arrive_station[i].station_start_time[j]<<" - "
//                <<traintime.arrive_station[i].station_arrive_time[j]<<" - "
//                <<traintime.arrive_station[i].station_through_time[j]<<endl;
            j++;
        }
    }

}
