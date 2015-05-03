#ifndef INC_CELINTVAR_H
#define INC_CELINTVAR_H

#include "CelNumVar.h"


class CelIntVar : public CelNumVar {
public:

    CelIntVar();
    CelIntVar(string &name);
    CelIntVar(const char *namestr);
    CelIntVar(string &name, double lower_bound, double upper_bound);
    CelIntVar(const char *namestr, double lower_bound, double upper_bound);


    ~CelIntVar();

    bool isInteger();
};


#endif
