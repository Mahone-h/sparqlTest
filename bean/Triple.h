
#ifndef TESTANTLR_TRIPLE_H
#define TESTANTLR_TRIPLE_H
#include <string>
#include <vector>
#include "../serial/shine_serial.hpp"

using namespace std;
class Triple{
public:
    string s;
    string p;
    string o;
    int sVAR;
    int pVAR;
    int oVAR;
    int IRINum;
    Triple(string s, string p, string o);

    Triple(string s, string p, string o, int sVar, int pVar, int oVar ,int IRINum);
    SHINE_SERIAL(Triple,s,p,o,sVAR,pVAR,oVAR,IRINum);
};

class Turtle{
public:
    vector<Triple> triples;
    SHINE_SERIAL(Turtle, triples);
};


class OC_Data{
public:
    vector<Turtle> datas;  //对应req
    SHINE_SERIAL(OC_Data,datas);
};


#endif //TESTANTLR_TRIPLE_H
