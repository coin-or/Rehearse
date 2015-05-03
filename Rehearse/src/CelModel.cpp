#include "CelModel.h"
#include "CelCoefficients.fwd.h"

#include "OsiSolverInterface.hpp"
#include "CoinFinite.hpp"


using namespace std;

CelModel::CelModel(OsiSolverInterface &solver) : solver(solver) {
}


CelModel::~CelModel(){
}


void CelModel::discoverVariables(CelExpression &expression, bool set_objective_coefficients){
    int old_column_index = model_variables.size();
    expression.attributeColumnIndex(model_variables);
    int current_column_index = model_variables.size();

    LinearCoefficients linear_coefficients;
    if (set_objective_coefficients){
        linear_coefficients.resize(current_column_index);
        expression.fillExpressionLinearCoefficients(linear_coefficients);
    }

    if (current_column_index != old_column_index){

        for (int i = old_column_index; i<current_column_index; i++){

            CelVariable *variable = model_variables[i];
            solver.addCol(0, NULL, NULL, variable->getLowerBound(), variable->getUpperBound(), set_objective_coefficients ? linear_coefficients[i] : 0);

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
        LinearCoefficients linear_coefficients;
        linear_coefficients.resize(current_column_index);

        double infinity = solver.getInfinity();
        double lower = 0.0;
        double upper = 0.0;

        expression.fillConstraintLinearCoefficients(linear_coefficients, infinity, lower, upper);

//#define DEBUG_COEFS
#ifdef DEBUG_COEFS
        printf("\n\n");
        //expression.display();
        printf("lincoef size = %lu \n", linear_coefficients.size());
        for (LinearCoefficients::iterator it = linear_coefficients.begin(); it != linear_coefficients.end(); ++it){
            printf("%f ", *it);
        }

        printf(" current_column_index = %d", current_column_index);
        printf(" lower = %f", lower);
        printf(" upper = %f", upper);
        printf("\n\n");
#endif

        int *index_array_ptr = new int[current_column_index];
        for (int i=0; i<current_column_index; i++){
            index_array_ptr[i] = i;
        }
        double *coef_array_ptr = &linear_coefficients[0];

#ifdef DEBUG_COEFS
        printf("\n");
        for (int i=0; i<current_column_index; i++){
            printf("%f ", coef_array_ptr[i]);
        }
        printf("\n");
#endif

        coin_build.addRow(current_column_index, index_array_ptr, coef_array_ptr, lower, upper);

        delete index_array_ptr;

    }

    if (expression.isDeletable())
        delete &expression;
}


void CelModel::builderToSolver(){
    solver.addRows(coin_build);
}

double CelModel::getSolutionValue(CelVariable &variable, OsiSolverInterface &external_solver){
    int column_index = variable.column_index;
    if (column_index == -1){
        return 0;
    }

    const double *solution = external_solver.getColSolution();

    return solution[column_index];
}

double CelModel::getSolutionValue(CelVariable &variable){
    int column_index = variable.column_index;
    if (column_index == -1){
        return 0;
    }

    const double *solution = solver.getColSolution();
    int cols = solver.getNumCols();

    // printf("cols = %d \n", cols);
    // printf("column_index = %d \n", column_index);

    // for (int i=0; i<cols; i++){
    //     printf("%f ", solution[i]);
    // }
    // printf("\n");

    return solution[column_index];
}











