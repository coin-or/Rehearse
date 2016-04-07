#include "CelVariable.h"

#include <stdio.h>

using namespace std;

namespace rehearse {

void CelVariable::displayMeOnly() const{
    printf("%s", name.c_str());
    if (column_index != -1){
        printf(" (%d)", column_index);
    }
#ifdef DEBUG_ADDRESS
    printf("(%p)", this);
#endif
}

string &CelVariable::getName(){
    return name;
}


CelVariable::CelVariable(){
}

CelVariable::CelVariable(string &name, double lower_bound, double upper_bound) :
    name(name), lower_bound(lower_bound), upper_bound(upper_bound) {
}

CelVariable::CelVariable(const char *namestr,
                         double lower_bound,
                         double upper_bound) :
                         name(namestr),
                         lower_bound(lower_bound), upper_bound(upper_bound) {
}

CelVariable::~CelVariable(){
}

const bool CelVariable::isInteger()
{
   return false;
}

}   // namespace rehearse
