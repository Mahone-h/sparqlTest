
#ifndef TESTANTLR_BASICQUERY_H
#define TESTANTLR_BASICQUERY_H


#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <deque>
#include "../test/testQuery.h"
#include "../serial/shine_serial.hpp"


using namespace std;
class Var;
typedef pair<string, int> reqNode;  //
typedef unordered_map<string, vector<string>> strMap;



class Req {
public:
    reqNode s;
    reqNode p;
    reqNode o;
    int IRINum;
    //添加其他约束条件。

    Req(reqNode s, reqNode p, reqNode o);
    Req(reqNode s, reqNode p, reqNode o,int IRINum);
    Req(string s, string p, string o);
};

class Var {
public:

    string str;
    unordered_set<string> values;
    list<string> res;
    vector<string> result;
    unordered_set<int> valueIDs;
    bool selected;
    //构成查询图
    unordered_map<string, Var *> nei;
    // key: other var name
    // value<this.var.value,other.var.value>
   // 相邻顶点之间的关系   为构建v2vMapR
    unordered_map<string, strMap> v2vMap;
    // key: other var name
    // value<other.var.value,this.var.value>
    //不同var之间的关系   只有selected is true 才建立 包含var和其他有path的var之间的map 只为最终结果
    unordered_map<string, strMap> v2vMapR;   //反向map


    //key 终点var name  deque 路径
    unordered_map<string, deque<Var *>> toVarPath;


    void v2vMapAdd(unordered_map<string, strMap> &map,string var,string v1,string v2);

    Var();
    Var(string str);

    Var(string str, bool tag);


};


class ParseInfo {
public:
    enum {
        IRI = 0, VAR = 1, STR = 2, LITERAL = 3,
    };
    vector<Req> oriReqs;
    //oriReqs 转成 reqs 2 pcie 获取数据 因为oc设备只支持查主语  req需转换spo pso osp
    vector<Req> reqs;
    vector<Var *> reqVar;  //需要的var 保持顺序  最前面的是isSelected。
    unordered_map<string, Var> varMap; //parse解析后结果
//    unordered_map<int, string> id2str;

};


class GroupGraph{
public:
    vector<ParseInfo> pInfos;
    Turtle oc_reqs;
    bool ifUsePrefix;
    string BASE;
    unordered_map<string, string> prefix;
    vector<string> reqVar;
    unordered_set<string> varSet;
};

void parseGroupGraph(ParseInfo &pInfo,Turtle &oc_reqs);

void parseData(ParseInfo &pInfo,OC_Data &data,int offset);

bool compTriple(const Req &a, const Req &b);

void parseReq(Req &req, ParseInfo &pInfo);

void getDataByOneIRI( ParseInfo &pInfo, Req &req, vector<Triple> &data);

void getDataByTwoIRI( int vPos, ParseInfo &pInfo, Req &req, vector<Triple> &data);

void constructQGraphNei(ParseInfo &pInfo);  //构建查询图var之间的nei关系

void buildQGraph(ParseInfo &pInfo);

int findNodeBFS(Var &v1, Var &v2, deque<Var *> &path, int i,unordered_set<string> &mem);

bool findNode(Var &v1,Var &v2, deque<Var *> &path);


bool buildV2VMap(ParseInfo &pInfo);

bool buildV2VMapByPath(Var &start, Var &mid, Var &end);

void resultJoin(ParseInfo &pInfo);

void joinVars(vector<Var *> &vars);

void crossJoinVars(vector<Var *> &vList1, vector<Var *> &vList2);

void crossJoinVarsByGroup(vector<Var *> &vList, int num);
void crossJoinVarsByGroup(Var &v, int num);


void crossJoinVarsByValue(vector<Var *> &vList, int num);

void crossJoinVarsByValue(Var &v, int num);

void resultOut(GroupGraph &groupGraph);

template<typename T>
void mergeTwoDequeToBack(deque<T> &d1, deque<T> &d2);

#endif //TESTANTLR_BASICQUERY_H
