

#include "BasicQuery.h"

Req::Req(reqNode s, reqNode p, reqNode o) {
    this->s = s;
    this->p = p;
    this->o = o;
}

Req::Req(reqNode s, reqNode p, reqNode o, int IRINum) {
    this->s = s;
    this->p = p;
    this->o = o;
    this->IRINum = IRINum;
}

Req::Req(string s, string p, string o) {
    this->s.first = s;
    this->p.first = p;
    this->o.first = o;
    this->s.second = 0;
    this->p.second = 0;
    this->o.second = 0;
}


/**
 * 建立 this var与 var之间的值的关系。   v1为this.value  v2为var.value
 * @param var
 * @param v1
 * @param v2
 */
void Var::v2vMapAdd(unordered_map<string, strMap> &map,string var, string v1, string v2) {
    auto iter = map.find(var);
    if (iter != map.end()) {
        strMap &sMap = iter->second;
        auto iter2 = sMap.find(v1);
        if (iter2 != sMap.end()) {
            iter2->second.emplace_back(v2);
        } else {
            sMap.emplace(v1, vector<string>{v2});
        }
    } else {
        map.emplace(var, strMap{{v1,vector<string>{v2}}});
    }
}


Var::Var(string str) {
    this->str = str;
    this->selected = false;
}

Var::Var(string str, bool tag) {
    this->str = str;
    this->selected = tag;
}


bool compTriple(const Req &a, const Req &b) {
    vector<string> sss;
    if ((a.s.first == b.s.first) && (a.p.first == b.p.first)) {
        return a.o.first < b.o.first;
    }
    if (a.s.first == b.s.first) {
        return a.p.first < b.p.first;
    }

    return a.s.first < b.s.first;
}

/** 查询时第一个保持 iri
 *  三张表 s_po p_so o_sp
 *  构建qGraph  pre和post
*/
void parseReq(Req &req, ParseInfo &pInfo) {
    if (req.s.second == pInfo.VAR) {
        //s==var
        if (req.p.second == pInfo.VAR) {
            //p == var
            if (req.o.second == pInfo.VAR) {
                cout << "query error: no IRI" << endl;
                return;
            }
            Var &v1 = pInfo.varMap.find(req.s.first)->second;
            Var &v2 = pInfo.varMap.find(req.p.first)->second;
            v1.nei.emplace(v2.str, &v2);
            v2.nei.emplace(v1.str, &v1);
            pInfo.reqs.emplace_back(Req(req.o, req.s, req.p, 1));
            return;
        } else {
            //p != var
            if (req.o.second == pInfo.VAR) {
                Var &v1 = pInfo.varMap.find(req.s.first)->second;
                Var &v2 = pInfo.varMap.find(req.o.first)->second;
                v1.nei.emplace(v2.str, &v2);
                v2.nei.emplace(v1.str, &v1);
                pInfo.reqs.emplace_back(Req(req.p, req.s, req.o, 1));
                return;
            } else {
                //po s
                pInfo.reqs.emplace_back(Req(req.p, req.s, req.o, 2));
                return;
            }
        }
    } else {
        //s!=var
        if (req.p.second == pInfo.VAR) {
            //p == var
            if (req.o.second == pInfo.VAR) {
                Var &v1 = pInfo.varMap.find(req.p.first)->second;
                Var &v2 = pInfo.varMap.find(req.o.first)->second;
                v1.nei.emplace(v2.str, &v2);
                v2.nei.emplace(v1.str, &v1);
                pInfo.reqs.emplace_back(Req(req.s, req.p, req.o, 1));
                return;
            } else {
                //so p
                pInfo.reqs.emplace_back(Req(req.s, req.p, req.o, 2));
                return;
            }
        } else {
            //p != var
            if (req.o.second == pInfo.VAR) {
                //sp o
                pInfo.reqs.emplace_back(Req(req.s, req.p, req.o, 2));
                return;
            } else {
                cout << "query error: no var" << endl;
                return;
            }
        }
    }
}

void getDataByOneIRI(ParseInfo &pInfo, Req &req, vector<Triple> &data) {
    getDataByReq(req.s.first, data);
    /*   for (auto iter = data.begin(); iter != data.end(); ++iter) {
           cout << iter->s<<"  "<<iter->p<<"  "<<iter->o<<"  "<< endl;
       }*/
    for (int i = 0; i < data.size(); ++i) {
        string tmp1 = data[i].p;
        string tmp2 = data[i].o;
        Var &v1 = pInfo.varMap.find(req.p.first)->second;
        Var &v2 = pInfo.varMap.find(req.o.first)->second;
        v1.values.emplace(tmp1);
        v2.values.emplace(tmp2);
        v1.v2vMapAdd(v1.v2vMap,v2.str, tmp1, tmp2);
        v2.v2vMapAdd(v2.v2vMap,v1.str, tmp2, tmp1);
        v1.v2vMapAdd(v1.v2vMapR,v2.str, tmp2, tmp1);
        v2.v2vMapAdd(v2.v2vMapR,v1.str, tmp1, tmp2);
    }
}

/**
 *
 * @param s
 * @param v
 * @param vPos    s 0 p 1 o 2
 * @param pInfo
 * @return
 */
void getDataByTwoIRI(int vPos, ParseInfo &pInfo, Req &req, vector<Triple> &data) {
    //cout << req.s.first+" "<<req.p.first+" "<<req.o.first+" "<< endl;
    //iri sp  var o
    if (vPos == 1) {
        getDataByReq(req.s.first, req.p.first, vPos, data);
        for (int i = 0; i < data.size(); ++i) {
            pInfo.varMap.find(req.o.first)->second.values.emplace(data[i].o);
        }
    }
    //iri so var p
    if (vPos == 2) {
        getDataByReq(req.s.first, req.o.first, vPos, data);
        for (int i = 0; i < data.size(); ++i) {
            pInfo.varMap.find(req.p.first)->second.values.emplace(data[i].p);
        }
    }
/*    for (auto iter = data.begin(); iter != data.end(); ++iter) {
        cout << iter->s<<"  "<<iter->p<<"  "<<iter->o<<"  "<< endl;
    }*/
}

//void constructQGraphNei(ParseInfo &pInfo) {
//    for (auto iter = pInfo.varMap.begin(); iter != pInfo.varMap.end(); ++iter) {
//        if (iter->second.post.size()>1) {
//            auto tmp1 = iter->second.post.begin();
//            for (auto tmp2 = tmp1 ; tmp2!=iter->second.post.end(); tmp2++) {
//                tmp2++;
//                tmp1->second->bro.emplace(tmp2->first, tmp2->second);
//                tmp2->second->bro.emplace(tmp1->first, tmp1->second);
//            }
//        }
//    }
//}


void buildQGraph(ParseInfo &pInfo) {
    deque<Var *> path;
    unordered_set<string> mem;
    for (int i=0;i<pInfo.reqVar.size()-1;i++) {
        Var *v1 = pInfo.reqVar[i];
        for (int j=i+1; j< pInfo.reqVar.size(); j++) {
            Var *v2 = pInfo.reqVar[j];
                findNodeBFS(*v1, *v2, path, 0, mem);
                if (path.size() > 0) {
                    v1->toVarPath.emplace(v2->str, path);
                    path.clear();
                    cout << v1->str << "->" << v2->str << endl;
                    deque<Var *>tmp=v1->toVarPath.find(v2->str)->second;
                    for (int i = 0; i < tmp.size(); ++i) {
                        cout << tmp[i]->str << " ";
                    }
                    cout << endl;
                    cout << "-------------------" << endl;
                }
                mem.clear();

        }

    }
}

int findNodeBFS(Var &v1, Var &v2, deque<Var *> &path, int i,
                unordered_set<string> &mem) {
    if (findNode(v1, v2, path)) {
        return i;
    }
    mem.emplace(v1.str);
    //没找到。
    int r = 0;
    if (!v1.nei.empty()) {
        for (auto iter = v1.nei.begin(); iter != v1.nei.end(); ++iter) {
            //已经找过或者只有v1这一个nei 没必要进入。
            if (mem.find(iter->first) != mem.end() || iter->second->nei.size() == 1) {
                continue;
            }
            path.emplace_back(iter->second);
            r = findNodeBFS(*iter->second, v2, path, i + 1, mem);
            if (r != 0) {
                break;
            }
            path.pop_back();
        }
    }
    return r;
}


bool findNode(Var &v1, Var &v2, deque<Var *> &path) {
    if (!v1.nei.empty()) {
        for (auto iter = v1.nei.begin(); iter != v1.nei.end(); ++iter) {
            if (iter->first == v2.str) {
                path.emplace_back(&v2);
                return true;
            }
        }
    }
    return false;
}

template<typename T>
void mergeTwoDequeToBack(deque<T> &d1, deque<T> &d2) {
    for (auto iter = d2.begin(); iter != d2.end(); ++iter) {
        d1.template emplace_back(*iter);
    }
}

//根据path找v2vmap  但是不一定有   TODO  To test
bool buildV2VMap(ParseInfo &pInfo) {
    bool out; //构建map的过程中断了后续的就没必要找了。
    for (int i=0;i<pInfo.reqVar.size();i++) {
        Var *v1 = pInfo.reqVar[i];
        if (v1->toVarPath.size() > 0) {
            out = true;
            for (auto path = v1->toVarPath.begin(); path != v1->toVarPath.end(); ++path) {
                for (int i = 0; i < path->second.size() - 1; ++i) {
                    out = buildV2VMapByPath(*v1, *path->second[i], *path->second[i + 1]);
                    if (!out) {
                        break;
                    }
                }
                if (!out) {
                    //如果req var 之间有path但是没有map 后续不用做了。查询结果为空
                    //空集 crossJoin为空。
                    if (path->second.back()->selected == true) {
                        return false;
                    }
                    break;
                }
            }
        }
    }
    return true;
}

bool buildV2VMapByPath(Var &start, Var &mid, Var &end) {
    if (start.v2vMap.find(end.str) != start.v2vMap.end()) {
        return true;
    }
    strMap s2m = start.v2vMap.find(mid.str)->second;
    strMap m2e = mid.v2vMap.find(end.str)->second;
    int ifHasV2V = 0;
    //i1 (a,b)
    for (auto i1 = s2m.begin(); i1 != s2m.end(); i1++) {
        //i2 (b,c)
        for (int i = 0; i < i1->second.size(); ++i) {
            auto i2 = m2e.find(i1->second[i]);
            if (i2 != m2e.end()) {
                ifHasV2V++;
                //成对的添加
                for (int i = 0; i < i2->second.size(); ++i) {
                    start.v2vMapAdd(start.v2vMap,end.str,i1->first, i2->second[i]);
                    end.v2vMapAdd(end.v2vMap,start.str,i2->second[i], i1->first);
                    start.v2vMapAdd(start.v2vMapR,end.str, i2->second[i],i1->first);
                    end.v2vMapAdd(end.v2vMapR,start.str, i1->first,i2->second[i]);
                }
            }
        }

    }
    if (ifHasV2V > 0) {
        return true;
    }
    return false;
}

/**
 * req 之间有path的join  有path但是没有关联值的也join
 * 两条不同的path之间是crossJoin
 * 没有的crossJoin
 * @param pInfo
 * @param res
 */
void resultJoin(ParseInfo &pInfo) {
    vector<vector<Var *>> join;
    vector<Var *> crossJoin;
    unordered_set<string> mem;
    //找有关系的 join
    for (int i = 0; i < pInfo.reqVar.size()-1; ++i) {
        Var *v1 = pInfo.reqVar[i];
        //先看v1加了没 v1加了 相关的已经都进去了
        if (mem.find(v1->str)!=mem.end()) {
            continue;
        }
        vector<Var *> toJoin;
        vector<Var *> toCrossJoin;
        toJoin.emplace_back(v1);
        for (int j = i+1; j <pInfo.reqVar.size(); ++j) {
            Var *v2=pInfo.reqVar[j];
            //
            if (v1->v2vMap.find(v2->str) != v1->v2vMap.end()) {
                toJoin.emplace_back(v2);
                mem.emplace(v2->str);
            }
        }
        if (toJoin.size()>1){
            join.emplace_back(toJoin);
            mem.emplace(v1->str);
        }
    }
    //找单
    for (int i = 0; i < pInfo.reqVar.size(); ++i) {
        if (mem.find(pInfo.reqVar[i]->str) == mem.end()) {
            crossJoin.emplace_back(pInfo.reqVar[i]);
        }
    }
    if (join.size()>0){
        //joinVars vector<vector<Var *>> join;
        for (int i = 0; i < join.size(); ++i) {
            joinVars(join[i]);
        }
    }
    //    vector<Var *> 有关联的之间crossJoin
    if (join.size()>1){
        for (int i = 1; i < join.size(); ++i) {
            int res1 = join[0][0]->res.size() - 1;
            int res2 = join[i][0]->res.size() - 1;
            for (int j = 0; j < i; ++j) {
                crossJoinVarsByValue(join[j], res2);
            }
            crossJoinVarsByGroup(join[i],res1 );
        }
    }
    // 所有的有关系的和 单个的crossJoin
    if (crossJoin.size() > 0) {
/*//        for (int i = 0; i < crossJoin.size(); ++i) {
//            Var *v = crossJoin[i];
//            v->res.insert(v->res.end(), v->values.begin(), v->values.end());
//            for (int j = 0; j < join.size(); ++j) {
//                crossJoinVarsByGroup(join[j], v->values.size()-1);
//            }
//            crossJoinVarsByValue(*v, ((join[0][0]->res.size())/v->values.size())-1);
//            if (i != 0) {
//                for (int j = 0; j < i; ++j) {
//                    crossJoinVarsByGroup(*crossJoin[j], v->values.size() - 1);
//                }
//            }
//        }*/
        Var *v = crossJoin[0];
        v->res.insert(v->res.end(), v->values.begin(), v->values.end());
        for (int i = 1; i < crossJoin.size(); ++i) {
            Var *v = crossJoin[i];
            v->res.insert(v->res.end(), v->values.begin(), v->values.end());
            int res1 = crossJoin[0]->res.size() - 1;
            int res2 = crossJoin[i]->res.size() - 1;
            for (int j = 0; j < i; ++j) {
                crossJoinVarsByValue(*crossJoin[j], res2);
            }
            crossJoinVarsByGroup(*crossJoin[i],res1 );
        }
    }
    if (join.size() > 0 && crossJoin.size() > 0) {
        int res1 = join[0][0]->res.size();
        int res2 = crossJoin[0]->res.size();
        for (int i = 0; i < join.size(); ++i) {
            crossJoinVarsByGroup(join[i],res2-1);
        }
        crossJoinVarsByValue(crossJoin, res1-1);
    }
}
//
void joinVars(vector<Var *> &vars){
    Var *start=vars[0];
    int vSize = start->values.size();
    for (int i = 1; i < vars.size() - 1; ++i) {
        Var *v1 = vars[i];
        int tmp = v1->values.size();
        if (tmp < vSize) {
            start = v1;
            vSize = tmp;
        }
    }
  /*  for (auto v = start->values.begin(); v != start->values.end(); ++v) {
        string tmp = *v;
        for (int i = 0; i < vars.size(); ++i) {
            Var *var = vars[i];
            if (var->str == start->str) {
                continue;
            }
            strMap &varMap = var->v2vMapR.find(start->str)->second;
            auto iter = varMap.find(tmp);
            if (iter != varMap.end()) {
                vector<string> &vec=iter->second;
                for (int j = 0; j < vec.size(); ++j) {
                    if (var->str != end->str) {
                        strMap inEndMap = end->v2vMapR.find(var->str)->second;
                        if (inEndMap.find(vec[j]) != inEndMap.end()) {
                            var->res.emplace_back(vec[j]);
                        }
                    } else{
                        var->res.emplace_back(vec[j]);
                    }
                }
            }
        }
        strMap &endMap = end->v2vMapR.find(start->str)->second;
        auto iter = endMap.find(tmp);
        if ( iter!= endMap.end()) {
            for (int i = 0; i < iter->second.size(); ++i) {
                start->res.emplace_back(tmp);
            }
        }
    }*/
    //join
    vector<string> mem;
    for (auto v = start->values.begin(); v != start->values.end(); ++v) {
        string tmp = *v;
        bool allHasVMap = true;
        for (int i = 0; i < vars.size(); ++i) {
            string var = vars[i]->str;
            if (var == start->str) {
                continue;
            }
            strMap &v1Map = start->v2vMap.find(var)->second;  //vmap
            if (v1Map.find(tmp) == v1Map.end()) {
                allHasVMap = false;
                break;
            }
        }
        if (allHasVMap) {
            mem.emplace_back(tmp);
        }
    }
    //最少一个  结果添加

    if (mem.size() > 0) {
        for (int i = 0; i < mem.size(); ++i) {
            string tmp = mem[i];
            int startToAdd = 0;
            for (int j = 0; j < vars.size(); ++j) {
                Var *v2 = vars[j];
                if (v2->str== start->str) {
                    continue;
                }
                vector<string> &vec = start->v2vMap.find(v2->str)->second.find(tmp)->second;
                for (int k = 0; k < vec.size(); ++k) {
                    int tag = true;
                    for (int l = 0; l < vars.size(); ++l) {
                        Var *v3 = vars[l];
                        if (v3->str == v2->str) {
                            continue;
                        }
                        strMap &v3Map = v3->v2vMapR.find(v2->str)->second;
                        if (v3Map.find(vec[k]) == v3Map.end()) {
                            tag = false;
                            break;
                        }
                    }
                    if (tag) {
                        v2->res.emplace_back(vec[k]);
                        startToAdd++;
                    }
                }
            }
            for (int j = 0; j < startToAdd / (vars.size() - 1); ++j) {
                start->res.emplace_back(tmp);
            }
        }
    }

    if (mem.size()==0){
        //map 没找对
        cout << "joinVars error "<< endl;
    }
}

//笛卡尔积. join 之后的有关联的var 组 之间
void crossJoinVars(vector<Var *> &vList1,vector<Var *> &vList2){
    int res1 = vList1[0]->res.size();
    int res2 = vList2[0]->res.size();
    if (res2 > 1) {
        crossJoinVarsByValue(vList1, res2-1);
    }
    if (res1 > 1) {
        crossJoinVarsByGroup(vList2, res1-1);
    }
}

//笛卡尔积.    var 组 和单Var crossJoin
void crossJoinVarsByGroup(vector<Var *> &vList, int num){
    for (int i = 0; i < vList.size(); ++i) {
        Var *v = vList[i];
        crossJoinVarsByGroup(*v,num);
    }
}

void crossJoinVarsByGroup(Var &v, int num){
    list<string> tmp = v.res;
    for (int j = 0; j < num; ++j) {
        v.res.insert(v.res.begin(), tmp.begin(),tmp.end());
    }
}

void crossJoinVarsByValue(vector<Var *> &vList, int num){
    for (int i = 0; i < vList.size(); ++i) {
        Var *v = vList[i];
        for (auto iter = v->res.begin(); iter != v->res.end(); ++iter) {
            for (int j = 0; j < num; ++j) {
                v->res.insert(iter, *iter);
            }
        }
    }
}

void crossJoinVarsByValue(Var &v, int num){
    for (auto iter = v.res.begin(); iter != v.res.end(); ++iter) {
        for (int j = 0; j < num; ++j) {
            v.res.insert(iter, *iter);
        }
    }
}
/**
 * 结果输出
 * @param groupGraph
 */
void resultOut(GroupGraph &groupGraph) {
    int maxLine = 0;
    vector<int> lines;
    for (int i = 0; i < groupGraph.pInfos.size(); ++i) {
        ParseInfo &pInfo = groupGraph.pInfos[i];
        for (int j = 0; j < pInfo.reqVar.size(); ++j) {
            Var *v = pInfo.reqVar[j];
            if (v->selected == false) {
                continue;
            }
            if (i==lines.size()) {
                lines.emplace_back(v->res.size());
                maxLine += v->res.size();
            }
            v->result.insert(v->result.end(), v->res.begin(), v->res.end());
        }
    }
    cout<<left<<setw(5)<<"0| ";
    for (int i = 0; i < groupGraph.reqVar.size(); ++i) {
        cout <<left<<setw(15) <<groupGraph.reqVar[i];
    }
    cout<< endl;
    int line = 1;
    for (int i = 0; i < groupGraph.pInfos.size(); ++i) {
        ParseInfo &pInfo = groupGraph.pInfos[i];
        for (int j = 0; j < lines[i]; ++j) {
            cout<<left<<setw(5) << to_string(line++)+"| ";
            for (int k = 0; k < groupGraph.reqVar.size(); ++k) {
                string tmp = groupGraph.reqVar[k];
                auto iter = pInfo.varMap.find(tmp);
                if (iter != pInfo.varMap.end()) {
                    Var *v = &iter->second;
                    cout << left << setw(15) << v->result[j];
                } else{
                    cout << left << setw(15) << "";
                }
            }
            cout<< endl;
        }
    }
    //旧版本  单独pinfo
    /*cout<<left<<setw(5)<<"0| ";
    vector<int> width;
    for (int i = 0; i < pInfo.reqVar.size(); ++i) {
        Var *v = pInfo.reqVar[i];
        if (v->selected == false) {
            continue;
        }
        cout <<left<<setw(15) <<v->str;
        v->result.insert(v->result.end(), v->res.begin(), v->res.end());
        if (v->res.size() > maxLine) {
            maxLine = v->res.size();
        }
    }
    cout<< endl;
    for (int i = 0; i < maxLine; ++i) {
        cout<<setw(5) << to_string(i+1)+"| ";
        for (int j = 0; j < pInfo.reqVar.size(); ++j) {
            Var *v = pInfo.reqVar[j];
            if (v->selected == false) {
                continue;
            }
            if (i<v->result.size()) {
                cout << left << setw(15) << v->result[i];
            } else{
                cout << left << setw(15) << "";
            }
        }
        cout << endl;
    }*/


}
/**
 * 先将所有req获取  不同pInfo内有各自的req 对应自己内部的  主要是union会这么用
 * @param pInfo
 * @param oc_reqs
 */

void parseGroupGraph(ParseInfo &pInfo,Turtle &oc_reqs) {
    //解析原始req
    for (int i = 0; i < pInfo.oriReqs.size(); ++i) {
        parseReq(pInfo.oriReqs[i], pInfo);
    }
    //req
    for (int i = 0; i < pInfo.reqs.size(); ++i) {
        Req &req = pInfo.reqs[i];
        oc_reqs.triples.emplace_back(Triple(req.s.first, req.p.first, req.o.first, req.s.second, req.p.second, req.o.second,req.IRINum));
    }
}
/**
 * 获取所有数据后在解析对应不同pInfo的数据
 * @param pInfo
 * @param data
 * @param offset
 */
void parseData(ParseInfo &pInfo,OC_Data &data,int offset){
    int res=0;
    for (int i = offset; i < data.datas.size(); ++i) {
        res += data.datas[i].triples.size();
    }
    if (res == 0) {
        cout << "no data"<< endl;
        return;
    }
    for (int i = 0; i < pInfo.reqs.size(); ++i) {
        vector<Triple> &dataTmp = data.datas[i+offset].triples;
        if (dataTmp.empty()) {
            continue;
        }
        Req &tmp = pInfo.reqs[i];
        if (tmp.IRINum == 1) {
            getDataByOneIRI(pInfo, tmp, dataTmp);
        } else{
            if (tmp.p.second == pInfo.IRI) {
                getDataByTwoIRI(1,pInfo,tmp,dataTmp);
            } else{
                getDataByTwoIRI(2,pInfo,tmp,dataTmp);
            }
        }
    }
    for (int i = 0; i < pInfo.reqVar.size(); ++i) {
        Var *v = pInfo.reqVar[i];
        if (v->values.empty()) {
            cout <<v->str<< "no data"<< endl;
            return;
        }
    }

    //构建同post父节点相邻关系
//    constructQGraphNei(this->pInfo);
/*    for (auto iter = pInfo.varMap.begin(); iter != pInfo.varMap.end(); ++iter) {
        cout << "-----------------------------------"<< endl;
        cout << iter->first<< endl;
        cout << "post:  ";
        for (auto i1 = iter->second.post.begin(); i1 != iter->second.post.end(); ++i1) {
            cout << i1->first<<"  ";
        }
        cout << endl;
        cout << "pre:   ";
        for (auto i2 = iter->second.pre.begin(); i2 != iter->second.bro.end(); ++i2) {
            cout << i2->first<<"  ";
        }
        cout << endl;
        cout << "bro:   ";
        for (auto i3 = iter->second.bro.begin(); i3 != iter->second.bro.end(); ++i3) {
            cout << i3->first<<"  ";
        }
        cout << endl;
        if (iter->second.selected==true){
            Var &var = iter->second;
            if (var.v2vMap.size()>0){
                for (auto i = var.v2vMap.begin(); i != var.v2vMap.end(); ++i) {
                    cout << i->first<<" v2vMap"<< endl;
                    for (auto j = i->second.begin(); j != i->second.end(); ++j) {
                        cout << j->first<<" ";
                        cout << j->second<<"   ";
                    }
                    cout << endl;
                }
            }
        }
    }*/

    buildQGraph(pInfo);
    if(!buildV2VMap(pInfo)){
        cout << "null res set"<< endl;
    } else{
        resultJoin(pInfo);
    }
}




