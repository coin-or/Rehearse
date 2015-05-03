#ifndef INC_CELVARIABLE_H
#define INC_CELVARIABLE_H

#include <string>

#include "CelExpression.h"

using namespace std;

class CelVariable : public CelExpression {
private:
protected:
    string name;
    virtual void displayMeOnly();


private:
    double lower_bound;
    double upper_bound;

public:
    CelVariable();
    CelVariable(string &name, double lower_bound, double upper_bound);
    CelVariable(const char *namestr, double lower_bound, double upper_bound);
    virtual ~CelVariable();
    virtual bool isInteger();

    double getLowerBound() { return lower_bound; }
    double getUpperBound() { return upper_bound; }

    string &getName();
};


#endif
