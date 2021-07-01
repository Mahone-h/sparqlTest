

#include "Triple.h"



Triple::Triple(string s, string p, string o) {
    this->s = s;
    this->p = p;
    this->o = o;
}

Triple::Triple(string s, string p, string o, int sVar, int pVar, int oVar ,int IRINum) {
    this->s = s;
    this->p = p;
    this->o = o;
    this->sVAR = sVar;
    this->pVAR = pVar;
    this->oVAR = oVar;
    this->IRINum = IRINum;
}

