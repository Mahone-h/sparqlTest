

#ifndef TESTANTLR_TESTQUERY_H
#define TESTANTLR_TESTQUERY_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "../bean/Triple.h"
#include "../parser/BasicQuery.h"
#include "../tool/tool.h"

using namespace std;



void Trim(string &str);

void readData(string path,vector<Triple> &data);

string readSql(string path);

void getDataByReq(string s,vector<Triple> &data);

void getDataByReq(string s,string v,int vPos,vector<Triple> &data);


class CoutSetw
{
    int n = 1;//输入序列
    int column;//显示列数
    int wide;//字符串占用显示宽度
public:
    CoutSetw(int c = 1, int w = 50) : column(c),wide(w){}
    void reset_n(){ n = 1;}
    void setColumn(int c){ column = c;}
    void setWide(int w){ wide = w;}
    void Couts(string str)
    {
        if(n++%column)
            cout << left << setw(wide) << str;
        else
            cout << left << setw(wide) << str << endl;
    }
};





#endif //TESTANTLR_TESTQUERY_H
