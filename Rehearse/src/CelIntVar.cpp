#include "CelIntVar.h"

using namespace std;

namespace rehearse {

CelIntVar::CelIntVar(string &name) : CelNumVar(name) {
}

CelIntVar::CelIntVar(const char *namestr) : CelNumVar(namestr) {
}

CelIntVar::CelIntVar(string &name,
                     double lower_bound,
                     double upper_bound) : CelNumVar(name,
                                                     lower_bound,
                                                     upper_bound){
}

CelIntVar::CelIntVar(const char *namestr,
                     double lower_bound,
                     double upper_bound) : CelNumVar(namestr,
                                                     lower_bound,
                                                     upper_bound){
}


CelIntVar::CelIntVar() : CelNumVar() {
}

CelIntVar::~CelIntVar(){
}


const bool CelIntVar::isInteger(){
    return true;
}

}   // namespace rehearse
