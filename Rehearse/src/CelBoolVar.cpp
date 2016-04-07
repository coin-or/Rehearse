#include "CelBoolVar.h"

using namespace std;

namespace rehearse {

CelBoolVar::CelBoolVar(string &name) : CelIntVar(name, 0.0, 1.0) {
}

CelBoolVar::CelBoolVar(const char *namestr) : CelIntVar(namestr, 0.0, 1.0) {
}


CelBoolVar::CelBoolVar() : CelIntVar("", 0.0, 1.0) {
}

CelBoolVar& CelBoolVar::operator= (const CelBoolVar other){
    *this = other;
    return *this;
}

CelBoolVar::~CelBoolVar(){
}



}   // namespace rehearse
