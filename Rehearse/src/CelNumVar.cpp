#include "CelNumVar.h"
#include "CoinFinite.hpp"

using namespace std;

namespace rehearse {

void CelNumVar::initMe(){
    this->deletable = false;
    this->node_type = CelExpression::NODE_VARIABLE;
}


CelNumVar::CelNumVar(string &name) : CelVariable(name,
                                                 -COIN_DBL_MAX,
                                                 COIN_DBL_MAX) {
    initMe();
}

CelNumVar::CelNumVar(const char *namestr) : CelVariable(namestr,
                                                        -COIN_DBL_MAX,
                                                        COIN_DBL_MAX) {
    initMe();
}

CelNumVar::CelNumVar(string &name,
                     double lower_bound,
                     double upper_bound) : CelVariable(name,
                                                       lower_bound,
                                                       upper_bound) {
    initMe();
}

CelNumVar::CelNumVar(const char *namestr,
                     double lower_bound,
                     double upper_bound) : CelVariable(namestr,
                                                       lower_bound,
                                                       upper_bound)  {
    initMe();
}


CelNumVar::CelNumVar() : CelVariable("", -COIN_DBL_MAX, COIN_DBL_MAX) {
    initMe();
}

CelNumVar& CelNumVar::operator= (const CelNumVar other){
    *this = other;
    return *this;
}


CelNumVar::~CelNumVar(){
}

}   // namespace rehearse
