#include "CelModel.h"
#include "CelCoefficients.fwd.h"

#include "OsiSolverInterface.hpp"
#include "CoinFinite.hpp"


using namespace std;

namespace rehearse {


CelModel::CelModel(OsiSolverInterface &solver) : solver(solver) {
}


CelModel::~CelModel(){
}


void CelModel::discoverVariables(CelExpression &expression,
                                 bool set_objective_coefficients){
    int old_column_index = model_variables.size();
    expression.attributeColumnIndex(model_variables);
    int current_column_index = model_variables.size();

    LinearCoefficientMap linear_coefficients;
    if (set_objective_coefficients){
        expression.fillExpressionLinearCoefficients(linear_coefficients);
    }

    if (current_column_index != old_column_index){

        for (int i = old_column_index; i<current_column_index; i++){

            CelVariable *variable = model_variables[i];
            solver.addCol(0, NULL, NULL,
                          variable->getLowerBound(),
                          variable->getUpperBound(),
                          set_objective_coefficients ?
                            linear_coefficients[i] :
                            0);

            if (variable->isInteger()){
                solver.setInteger(i);
            }
        }

    }

}


void CelModel::setObjective(CelExpression &expression){
    if (!expression.isLinear()){
        printf("Expression or constraint is not linear\n");
        expression.displayFlat();
    } else {
        discoverVariables(expression, true);
        if (expression.isDeletable())
            delete &expression;
    }
}


void CelModel::addConstraint(CelExpression &expression){
    bool hasError = false;

    if (expression.isConsistentConstraint()){
        if (expression.isLinear()){
        } else {
            printf("Expression or constraint is not linear\n");
            expression.displayFlat();
            hasError = true;
        }
    } else {
        printf("Expression is not a valid constraint\n");
        expression.displayFlat();
        expression.display();
        hasError = true;
    }

    if (!hasError){
        discoverVariables(expression, false);

        int current_column_index = model_variables.size();
        LinearCoefficientMap linear_coefficients;

        double infinity = solver.getInfinity();
        double lower = 0.0;
        double upper = 0.0;

        expression.fillConstraintLinearCoefficients(linear_coefficients,
                                                    infinity,
                                                    lower, upper);

        int num_variables_in_constraint = linear_coefficients.size();
        int *index_array_ptr = new int[num_variables_in_constraint];
        double *coef_array_ptr = new double[num_variables_in_constraint];
        int i = 0;
        for (auto &coef_info : linear_coefficients){
            index_array_ptr[i] = coef_info.first;
            coef_array_ptr[i] = coef_info.second;
            i++;
        }

        coin_build.addRow(num_variables_in_constraint,
                          index_array_ptr,
                          coef_array_ptr,
                          lower, upper);

// #define DEBUG_COEFS
#ifdef DEBUG_COEFS
        printf("\n");
        for (int i=0; i<num_variables_in_constraint; i++){
            printf("%d -> %f ", index_array_ptr[i], coef_array_ptr[i]);
        }
        printf("\n");
#endif

        delete[] index_array_ptr;
        delete[] coef_array_ptr;

    }

    if (expression.isDeletable())
        delete &expression;
}


void CelModel::builderToSolver(){
    solver.addRows(coin_build);
}

double CelModel::getSolutionValue(CelVariable &variable,
                                  OsiSolverInterface &external_solver) const {
    int column_index = variable.column_index;
    if (column_index == -1){
        return 0;
    }

    const double *solution = external_solver.getColSolution();

    return solution[column_index];
}

double CelModel::getSolutionValue(CelVariable &variable) const{
    int column_index = variable.column_index;
    if (column_index == -1){
        return 0;
    }

    const double *solution = solver.getColSolution();

#ifdef DEBUG_COEFS
    int cols = solver.getNumCols();
    printf("cols = %d \n", cols);
    printf("column_index = %d \n", column_index);

    for (int i=0; i<cols; i++){
        printf("%f ", solution[i]);
    }
    printf("\n");
#endif

    return solution[column_index];
}



}   // namespace rehearse







