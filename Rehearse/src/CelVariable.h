#ifndef INC_CELVARIABLE_H
#define INC_CELVARIABLE_H

#include <string>

#include "CelExpression.h"

namespace rehearse {

class CelVariable : public CelExpression {
private:
protected:
    std::string name;
    virtual void displayMeOnly() const;


private:
    double lower_bound;
    double upper_bound;

public:
    CelVariable();
    CelVariable(std::string &name, double lower_bound, double upper_bound);
    CelVariable(const char *namestr, double lower_bound, double upper_bound);
    virtual ~CelVariable();
    virtual const bool isInteger();

    double getLowerBound() const { return lower_bound; }
    double getUpperBound() const { return upper_bound; }

    std::string &getName();
};

}   // namespace rehearse

#endif
