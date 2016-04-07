#ifndef INC_CELNUMVAR_H
#define INC_CELNUMVAR_H

#include "CelVariable.h"

namespace rehearse {

class CelNumVar : public CelVariable {
    void initMe();
public:

    CelNumVar();
    CelNumVar(std::string &name);
    CelNumVar(const char *namestr);
    CelNumVar(std::string &name, double lower_bound, double upper_bound);
    CelNumVar(const char *namestr, double lower_bound, double upper_bound);
    CelNumVar& operator= (const CelNumVar other);

    ~CelNumVar();


};

}   // namespace rehearse

#endif
