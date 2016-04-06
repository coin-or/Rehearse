#ifndef INC_CELVARIABLE_H
#define INC_CELVARIABLE_H

#include <string>

#include "CelExpression.h"


class CelVariable : public CelExpression {
private:
protected:
    std::string name;
    virtual void displayMeOnly();


private:
    double lower_bound;
    double upper_bound;

public:
    CelVariable();
    CelVariable(std::string &name, double lower_bound, double upper_bound);
    CelVariable(const char *namestr, double lower_bound, double upper_bound);
    virtual ~CelVariable();
    virtual bool isInteger();

    double getLowerBound() { return lower_bound; }
    double getUpperBound() { return upper_bound; }

    std::string &getName();
};


#endif
