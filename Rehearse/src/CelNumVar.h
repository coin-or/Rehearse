#ifndef INC_CELNUMVAR_H
#define INC_CELNUMVAR_H

#include "CelVariable.h"


class CelNumVar : public CelVariable {
    void initMe();
public:

    CelNumVar();
    CelNumVar(string &name);
    CelNumVar(const char *namestr);
    CelNumVar(string &name, double lower_bound, double upper_bound);
    CelNumVar(const char *namestr, double lower_bound, double upper_bound);
    CelNumVar& operator= (const CelNumVar other);

    ~CelNumVar();


};


#endif
