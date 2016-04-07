#ifndef INC_CELMODEL_H
#define INC_CELMODEL_H

#include "CelExpression.h"
#include "CelVariable.h"
#include "CoinBuild.hpp"


class OsiSolverInterface;


namespace rehearse {


class CelModel {
private:
    OsiSolverInterface &solver;
    CoinBuild coin_build;

    std::vector<CelVariable *> model_variables;
    void discoverVariables(CelExpression &expression,
                           bool set_objective_coefficients);

public:
    CelModel(OsiSolverInterface &solver);
    ~CelModel();

    void addConstraint(CelExpression &expression);
    void setObjective(CelExpression &expression);
    void builderToSolver();
    double getSolutionValue(CelVariable &variable) const;
    double getSolutionValue(CelVariable &variable,
                            OsiSolverInterface &solver) const;

};


}   // namespace rehearse

#endif
