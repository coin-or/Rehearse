#ifndef INC_CELBOOLVAR_H
#define INC_CELBOOLVAR_H

#include "CelIntVar.h"

namespace rehearse {

class CelBoolVar : public CelIntVar {
public:

    CelBoolVar();
    CelBoolVar(std::string &name);
    CelBoolVar(const char *namestr);
    CelBoolVar& operator= (const CelBoolVar other);

    ~CelBoolVar();

};

}   // namespace rehearse

#endif
