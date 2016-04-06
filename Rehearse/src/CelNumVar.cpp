#include "CelNumVar.h"
#include "CoinBuild.hpp"

using namespace std;

void CelNumVar::initMe(){
    this->deletable = false;
    this->node_type = CelExpression::NODE_VARIABLE;
}


CelNumVar::CelNumVar(string &name) : CelVariable(name, -COIN_DBL_MAX, COIN_DBL_MAX) {
    // printf("CelNumVar(string &name)\n");
    initMe();
}

CelNumVar::CelNumVar(const char *namestr) : CelVariable(namestr, -COIN_DBL_MAX, COIN_DBL_MAX) {
    // printf("CelNumVar(const char *namestr)\n");
    initMe();
}

CelNumVar::CelNumVar(string &name, double lower_bound, double upper_bound) : CelVariable(name, lower_bound, upper_bound) {
    initMe();
}

CelNumVar::CelNumVar(const char *namestr, double lower_bound, double upper_bound) : CelVariable(namestr, lower_bound, upper_bound)  {
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


