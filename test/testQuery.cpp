
#include "testQuery.h"

void Trim(string & str)
{
    string blanks("\f\v\r\t\n ");
    str.erase(0,str.find_first_not_of(blanks));
    str.erase(str.find_last_not_of(blanks) + 1);
}

void readData(string path,vector<Triple> &data) {
    ifstream ifs;
    ifs.open(path, ios::in);
    string buf;
    vector<string> tmp;
    while (getline(ifs, buf)) {
        if (buf.c_str()[0]=='#'||buf.empty()
            ||buf.c_str()[0]=='\r'||buf.c_str()[0]=='\n'){
            continue;
        }
        splitString(buf,tmp , "|");
        for (int i = 0; i < tmp.size(); ++i) {
            Trim(tmp[i]);
        }
        data.emplace_back(Triple(tmp[0], tmp[1], tmp[2]));
        tmp.clear();
    }
    ifs.close();
}

void getDataByReq(string s, vector<Triple> &data) {
    for (auto iter = data.begin(); iter != data.end();) {
        if (iter->s!=s){
            iter = data.erase(iter);
        } else{
            iter++;
        }
    }
}

void getDataByReq(string s, string v, int vPos, vector<Triple> &data) {
    for (auto iter = data.begin(); iter != data.end();) {
        if (iter->s!=s){
            iter = data.erase(iter);
            continue;
        }
        if (vPos == 1) {
            if (iter->p != v) {
                iter = data.erase(iter);
                continue;
            }
        }
        if (vPos == 2) {
            if (iter->o != v) {
                iter = data.erase(iter);
                continue;
            }
        }
        iter++;
    }
}

string readSql(string path) {
    ifstream ifs(path);
    stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    string tmp = buffer.str();
    return tmp.substr(0,tmp.find_last_of("}")+1);
}






