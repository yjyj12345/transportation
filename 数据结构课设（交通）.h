#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<limits>
#include<algorithm>
#include <sstream>  

using namespace std;

const int MAXV = 50;
const int INF = 5000;//无穷大

struct City {
    int no;//城市编号
    string name;//城市名称
    int transferTime;//中转时间（分钟）
};

struct Edge {
    string departureTime;  //出发时间 (分钟)
    string arrivalTime;    //到达时间 (分钟)
    int price;          //票价
    int duration;          // 时长 (分钟)
};

class MatGraph {
    City cities[MAXV];                 //城市数组
    int priceMatrix[MAXV][MAXV];       //票价邻接矩阵
    int timeMatrix[MAXV][MAXV];        //时间邻接矩阵
    Edge edgeInfo[MAXV][MAXV];         //边信息矩阵（存储最优列车信息）
    int n;                             //顶点数

public:
    MatGraph();//默认构造函数
    void addCity(int no, const string& name, int transferTime);//添加城市
    void deleteCity(int no);//删除城市
    void addTrain(int start, int end, const string& departureStr, const string& arrivalStr, int price);//添加列车（更新邻接矩阵）
    void queryTrains(int start, int end);//查询列车
    void readFromFile(const string& filename);//从文件读取列车信息
    void saveToFile(const string& filename);//保存列车信息到文件
    void findOptimalPath(int start, int end, int strategy);//最优路径查
    void findFastestPath(int start, int end);//查找最快路径
    void findCheapestPath(int start, int end);//查找最省钱路径
};

bool isValidTime(const string& timeStr);//验证时间格式是否正确
int timeDifference(const string& startTime, const string& endTime);//计算两个时间之间的分钟差
