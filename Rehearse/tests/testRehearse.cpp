#include <iostream>
#include <cassert>
#include <vector>

#include <CelModel.h>
#include <CelNumVar.h>
#include <CelIntVar.h>
#include <CelBoolVar.h>
#include <CelNumVarArray.h>
#include <CelBoolVarArray.h>

#include "CbcModel.hpp"
#include "OsiClpSolverInterface.hpp"
#include "OsiCbcSolverInterface.hpp"


using namespace std;
using namespace rehearse;

inline void _assert2(const char* expression, const char* file, int line)
{
 fprintf(stderr, "Assertion '%s' failed, file '%s' line '%d'.", expression, file, line);
 abort();
}

#define assert2(EXPRESSION) ((EXPRESSION) ? (void)0 : _assert2(#EXPRESSION, __FILE__, __LINE__))



int test_counter = 0;

void exemple1(){
    OsiClpSolverInterface *solver = new OsiClpSolverInterface();
    CelModel model(*solver);

    CelNumVar x1("x1");
    CelNumVar x2("x2");

    model.setObjective (       7 * x1 + 9 * x2 );

    model.addConstraint(       1 * x1 +     x2 == 18  );
    model.addConstraint(                    x2 <= 14  );
    model.addConstraint(       2 * x1 + 3 * x2 <= 50  );

    solver->setObjSense(-1.0);
    model.builderToSolver();
    solver->setLogLevel(0);
    solver->initialSolve();

    printf("Solution for x1 : %g\n", model.getSolutionValue(x1));
    printf("Solution for x2 : %g\n", model.getSolutionValue(x2));
    printf("Solution objvalue = : %g\n", solver->getObjValue());

    assert2(fabs(4 - model.getSolutionValue(x1)) < 0.00000001);
    assert2(fabs(14 - model.getSolutionValue(x2)) < 0.00000001);
    assert2(fabs(18 - (model.getSolutionValue(x1) + model.getSolutionValue(x2)) ) < 0.00000001);
    assert2(fabs(154 - solver->getObjValue()) < 0.00000001);


    printf("Test %d : OK\n", ++test_counter);

    delete solver;
}


void exemple2(){
    OsiClpSolverInterface *solver = new OsiClpSolverInterface();
    CelModel model(*solver);

    CelNumVar x;
    CelNumVar y;

    model.setObjective (       0.2 * x + 0.3 * y );

    model.addConstraint(       24 <=  8 * x + 12 * y   );
    model.addConstraint(       36 <= 12 * x + 12 * y   );
    model.addConstraint(        4 <=  2 * x +  1 * y   );

    solver->setObjSense(1.0);
    model.builderToSolver();
    solver->setLogLevel(0);
    solver->initialSolve();

    printf("Solution for x : %g\n", model.getSolutionValue(x));
    printf("Solution for y : %g\n", model.getSolutionValue(y));
    printf("Solution objvalue = : %g\n", solver->getObjValue());

    assert2(fabs(3 - model.getSolutionValue(x)) < 0.00000001);
    assert2(fabs(0 - model.getSolutionValue(y)) < 0.00000001);
    assert2(fabs(0.6 - solver->getObjValue()) < 0.00000001);

    printf("Test %d : OK\n", ++test_counter);

    delete solver;
}


void exemple3(){
    OsiClpSolverInterface *solver = new OsiClpSolverInterface();

    CelModel model(*solver);

    CelNumVar x1("x1");
    CelNumVar x2("x2");

    model.setObjective (       7.3 * x1 + 9.2 * x2 );

    model.addConstraint(       1.6 * x1 +       x2 == 18.5  );
    model.addConstraint(                  2.9 * x2 <= 14.3  );
    model.addConstraint(       2.7 * x1 + 3.5 * x2 <= 56.3  );

    solver->setObjSense(-1.0);
    model.builderToSolver();
    solver->setLogLevel(0);
    solver->initialSolve();

    printf("Solution for x1 : %g\n", model.getSolutionValue(x1));
    printf("Solution for x2 : %g\n", model.getSolutionValue(x2));

    printf("Solution objvalue = : %g\n", solver->getObjValue());

    assert2(fabs(8.4806 - model.getSolutionValue(x1)) < 0.0001);
    assert2(fabs(4.93103 - model.getSolutionValue(x2)) < 0.0001);
    assert2(fabs(107.274 - solver->getObjValue()) < 0.0001);

    printf("Test %d : OK\n", ++test_counter);

    delete solver;
}

void exemple4(){
    OsiClpSolverInterface *solver = new OsiClpSolverInterface();

    CelModel model(*solver);

    CelIntVar x1("x1");
    CelIntVar x2("x2");

    model.setObjective (       7.3 * x1 + 9.2 * x2 );
    model.addConstraint(       1.6 * x1 +       x2 <= 18.5  );
    model.addConstraint(                  2.9 * x2 <= 14.3  );
    model.addConstraint(       2.7 * x1 + 3.5 * x2 <= 56.3  );

    solver->setObjSense(-1.0);
    model.builderToSolver();
    solver->setLogLevel(0);
    solver->initialSolve();

    CbcModel cbcModel(*solver);
    cbcModel.setLogLevel(1);
    cbcModel.solver()->setHintParam(OsiDoReducePrint,true,OsiHintTry);
    cbcModel.branchAndBound();

    printf("Solution for x1 : %g\n", model.getSolutionValue(x1, *cbcModel.solver()));
    printf("Solution for x2 : %g\n", model.getSolutionValue(x2, *cbcModel.solver()));

    printf("Solution objvalue = : %g\n", cbcModel.solver()->getObjValue());

    assert2(fabs(9 - model.getSolutionValue(x1, *cbcModel.solver())) < 0.0001);
    assert2(fabs(4 - model.getSolutionValue(x2, *cbcModel.solver())) < 0.0001);
    assert2(fabs(102.5 - cbcModel.solver()->getObjValue()) < 0.0001);

    printf("Test %d : OK\n", ++test_counter);

    delete solver;
}


void chineseRemainders(){
    OsiClpSolverInterface *solver = new OsiClpSolverInterface();

    CelModel model(*solver);

    CelIntVar x("x");
    CelIntVar t("t");
    CelIntVar u("u");
    CelIntVar v("v");

    model.setObjective ( x );

    model.addConstraint( x == 2 + 3 * t );
    model.addConstraint( x == 3 + 5 * u );
    model.addConstraint( x == 2 + 7 * v );

    model.addConstraint( 0 <= x  );
    model.addConstraint( 0 <= t  );
    model.addConstraint( 0 <= u  );
    model.addConstraint( 0 <= v  );

    solver->setObjSense(1.0);
    model.builderToSolver();
    solver->setLogLevel(0);

    solver->initialSolve();

    CbcModel cbcModel(*solver);
    cbcModel.setLogLevel(1);
    cbcModel.solver()->setHintParam(OsiDoReducePrint,true,OsiHintTry);
    cbcModel.branchAndBound();

    printf("Solution for x : %g\n", model.getSolutionValue(x, *cbcModel.solver()));
    printf("Solution for t : %g\n", model.getSolutionValue(t, *cbcModel.solver()));
    printf("Solution for u : %g\n", model.getSolutionValue(u, *cbcModel.solver()));
    printf("Solution for v : %g\n", model.getSolutionValue(v, *cbcModel.solver()));
    printf("Solution objvalue = : %g\n", cbcModel.solver()->getObjValue());

    assert2(fabs(23 - model.getSolutionValue(x, *cbcModel.solver())) < 0.0001);
    assert2(fabs(7 - model.getSolutionValue(t, *cbcModel.solver())) < 0.0001);
    assert2(fabs(4 - model.getSolutionValue(u, *cbcModel.solver())) < 0.0001);
    assert2(fabs(3 - model.getSolutionValue(v, *cbcModel.solver())) < 0.0001);

    printf("Test %d : OK\n", ++test_counter);

    delete solver;
}



double expressionTest1SolveExpr(){
    OsiClpSolverInterface solver;
    CelModel model(solver);

    CelNumVar x1("x1");

    CelExpression expr;
    expr.display();

    expr = x1;
    expr.display();

    model.setObjective ( x1 );
    model.addConstraint( expr <= 3.0);

    solver.setObjSense(-1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();

    double x1_value = model.getSolutionValue(x1);
    printf("Solution for x1 : %g\n", x1_value);
    printf("Solution objvalue = : %g\n", solver.getObjValue());

    return x1_value;
}

double expressionTest1SolveRef(){
    OsiClpSolverInterface solver;
    CelModel model(solver);

    CelNumVar x1("x1");

    model.setObjective ( x1 );
    model.addConstraint( x1 <= 3.0);

    solver.setObjSense(-1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();

    double x1_value = model.getSolutionValue(x1);
    printf("Solution for x1 : %g\n", x1_value);
    printf("Solution objvalue = : %g\n", solver.getObjValue());

    return x1_value;
}

double expressionTest2SolveExpr(){
    OsiClpSolverInterface solver;
    CelModel model(solver);

    CelNumVar x1("x1");

    CelExpression expr;
    expr.display();

    expr = x1;
    expr.display();

    CelExpression expr2 = expr;

    model.setObjective ( x1 );
    model.addConstraint( expr2 == 2.0);

    solver.setObjSense(-1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();

    double x1_value = model.getSolutionValue(x1);
    printf("Solution for x1 : %g\n", x1_value);
    printf("Solution objvalue = : %g\n", solver.getObjValue());

    return x1_value;
}

double expressionTest2SolveRef(){
    OsiClpSolverInterface solver;
    CelModel model(solver);

    CelNumVar x1("x1");


    model.setObjective ( x1 );
    model.addConstraint( x1 == 2.0);

    solver.setObjSense(-1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();

    double x1_value = model.getSolutionValue(x1);
    printf("Solution for x1 : %g\n", x1_value);
    printf("Solution objvalue = : %g\n", solver.getObjValue());

    return x1_value;
}


double expressionTest3SolveExpr(){
    OsiClpSolverInterface solver;
    CelModel model(solver);

    CelNumVar x1("x1");

    CelExpression expr3;
    for (int i=0; i<3; i++){
        expr3 += x1;
    }
    expr3.display();
    expr3.displayFlat();


    model.setObjective ( x1 );
    model.addConstraint( expr3 == 8.0);

    solver.setObjSense(-1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();

    double x1_value = model.getSolutionValue(x1);
    printf("Solution for x1 : %g\n", x1_value);
    printf("Solution objvalue = : %g\n", solver.getObjValue());

    return x1_value;
}

double expressionTest3SolveRef(){
    OsiClpSolverInterface solver;
    CelModel model(solver);

    CelNumVar x1("x1");


    model.setObjective ( x1 );
    model.addConstraint( 3.0 * x1 == 8.0);

    solver.setObjSense(-1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();

    double x1_value = model.getSolutionValue(x1);
    printf("Solution for x1 : %g\n", x1_value);
    printf("Solution objvalue = : %g\n", solver.getObjValue());

    return x1_value;
}

void expressionTest(){
    assert2(expressionTest1SolveExpr() == expressionTest1SolveRef());
    assert2(expressionTest2SolveExpr() == expressionTest2SolveRef());
    assert2(expressionTest3SolveExpr() == expressionTest3SolveRef());
    printf("Test %d : OK\n", ++test_counter);

}


void assignmentProblemAsTotallyUnimodularWithArray(){
    /*

        supply       demand
        +---+        +---+
        | 1 |        | 1 |
        +---+\       +---+
              \
        +---+  \     +---+
        | 2 |---\----| 2 |
        +---+    \   +---+
                  \
        +---+      \ +---+
        | 3 |       \| 3 |
        +---+        +---+

        x_i_j = 1 iff supply i is connected to demand j

        This figure shows x_1_3 and x_2_2
    */

    OsiClpSolverInterface solver;
    CelModel model(solver);

    int S = 3;  // number of supply
    int D = 3;  // number of demand

    CelNumVarArray x;
    x.multiDimensionResize(2, S, D);


    CelExpression objective;
    for (int i=0; i<S; i++){
        for (int j=0; j<D; j++){
            objective += x[i][j];
        }
    }
    model.setObjective ( objective );

    // each supply must supply something
    for (int i=0; i<S; i++){
        CelExpression supplyConstraint;
        for (int j=0; j<D; j++){
            supplyConstraint += x[i][j];
        }
        model.addConstraint( supplyConstraint == 1 );
    }

    // each demand must be fulfilled
    for (int j=0; j<D; j++){
        CelExpression demandConstraint;
        for (int i=0; i<S; i++){
            demandConstraint += x[i][j];
        }
        model.addConstraint( demandConstraint == 1 );
    }

    // variables must be positive
    for (int i=0; i<S; i++){
        for (int j=0; j<D; j++){
            model.addConstraint( 0 <= x[i][j] );
        }
    }

    solver.setObjSense(1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();

    for (int i=0; i<S; i++){
        for (int j=0; j<D; j++){
            double x_i_j_value = model.getSolutionValue(x[i][j]);
            printf("Solution for x_%d_%d : %g\n", i+1, j+1, x_i_j_value);

            // totally unimodularity check
            assert2(fabs(x_i_j_value - 0) < 0.00000001 || fabs(x_i_j_value - 1) < 0.00000001);
        }
    }


    printf("Test %d : OK\n", ++test_counter);
}

void assignmentProblemAsTotallyUnimodular(){
    /*

        supply       demand
        +---+        +---+
        | 1 |        | 1 |
        +---+\       +---+
              \
        +---+  \     +---+
        | 2 |---\----| 2 |
        +---+    \   +---+
                  \
        +---+      \ +---+
        | 3 |       \| 3 |
        +---+        +---+

        x_i_j = 1 iff supply i is connected to demand j

        This figure shows x_1_3 and x_2_2
    */

    OsiClpSolverInterface solver;
    CelModel model(solver);


    // we should use variable arrays but we do this way for clarity purpose, see assignmentProblemAsTotallyUnimodularWithArray
    CelNumVar x_1_1;
    CelNumVar x_1_2;
    CelNumVar x_1_3;
    CelNumVar x_2_1;
    CelNumVar x_2_2;
    CelNumVar x_2_3;
    CelNumVar x_3_1;
    CelNumVar x_3_2;
    CelNumVar x_3_3;

    model.setObjective ( x_1_1 + x_1_2 + x_1_3 + x_2_1 + x_2_2 + x_2_3 + x_3_1 + x_3_2 + x_3_3 );

    model.addConstraint( x_1_1 + x_1_2 + x_1_3 == 1);
    model.addConstraint( x_2_1 + x_2_2 + x_2_3 == 1);
    model.addConstraint( x_3_1 + x_3_2 + x_3_3 == 1);

    model.addConstraint( x_1_1 + x_2_1 + x_3_1 == 1);
    model.addConstraint( x_1_2 + x_2_2 + x_3_2 == 1);
    model.addConstraint( x_1_3 + x_2_3 + x_3_3 == 1);

    model.addConstraint( 0 <= x_1_1 );
    model.addConstraint( 0 <= x_1_2 );
    model.addConstraint( 0 <= x_1_3 );
    model.addConstraint( 0 <= x_2_1 );
    model.addConstraint( 0 <= x_2_2 );
    model.addConstraint( 0 <= x_2_3 );
    model.addConstraint( 0 <= x_3_1 );
    model.addConstraint( 0 <= x_3_2 );
    model.addConstraint( 0 <= x_3_3 );

    solver.setObjSense(1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();

    double x_1_1_value = model.getSolutionValue(x_1_1);
    double x_1_2_value = model.getSolutionValue(x_1_2);
    double x_1_3_value = model.getSolutionValue(x_1_3);
    double x_2_1_value = model.getSolutionValue(x_2_1);
    double x_2_2_value = model.getSolutionValue(x_2_2);
    double x_2_3_value = model.getSolutionValue(x_2_3);
    double x_3_1_value = model.getSolutionValue(x_3_1);
    double x_3_2_value = model.getSolutionValue(x_3_2);
    double x_3_3_value = model.getSolutionValue(x_3_3);

    printf("Solution for x_1_1 : %g\n", x_1_1_value);
    printf("Solution for x_1_2 : %g\n", x_1_2_value);
    printf("Solution for x_1_3 : %g\n", x_1_3_value);
    printf("Solution for x_2_1 : %g\n", x_2_1_value);
    printf("Solution for x_2_2 : %g\n", x_2_2_value);
    printf("Solution for x_2_3 : %g\n", x_2_3_value);
    printf("Solution for x_3_1 : %g\n", x_3_1_value);
    printf("Solution for x_3_2 : %g\n", x_3_2_value);
    printf("Solution for x_3_3 : %g\n", x_3_3_value);

    // totally unimodularity check
    assert2(fabs(x_1_1_value - 0) < 0.00000001 || fabs(x_1_1_value - 1) < 0.00000001);
    assert2(fabs(x_1_2_value - 0) < 0.00000001 || fabs(x_1_2_value - 1) < 0.00000001);
    assert2(fabs(x_1_3_value - 0) < 0.00000001 || fabs(x_1_3_value - 1) < 0.00000001);
    assert2(fabs(x_2_1_value - 0) < 0.00000001 || fabs(x_2_1_value - 1) < 0.00000001);
    assert2(fabs(x_2_2_value - 0) < 0.00000001 || fabs(x_2_2_value - 1) < 0.00000001);
    assert2(fabs(x_2_3_value - 0) < 0.00000001 || fabs(x_2_3_value - 1) < 0.00000001);
    assert2(fabs(x_3_1_value - 0) < 0.00000001 || fabs(x_3_1_value - 1) < 0.00000001);
    assert2(fabs(x_3_2_value - 0) < 0.00000001 || fabs(x_3_2_value - 1) < 0.00000001);
    assert2(fabs(x_3_3_value - 0) < 0.00000001 || fabs(x_3_3_value - 1) < 0.00000001);

    // constraint satisfaction
    assert2(fabs(x_1_1_value + x_1_2_value + x_1_3_value - 1) < 0.00000001);
    assert2(fabs(x_2_1_value + x_2_2_value + x_2_3_value - 1) < 0.00000001);
    assert2(fabs(x_3_1_value + x_3_2_value + x_3_3_value - 1) < 0.00000001);
    assert2(fabs(x_1_1_value + x_2_1_value + x_3_1_value - 1) < 0.00000001);
    assert2(fabs(x_1_2_value + x_2_2_value + x_3_2_value - 1) < 0.00000001);
    assert2(fabs(x_1_3_value + x_2_3_value + x_3_3_value - 1) < 0.00000001);

    printf("Test %d : OK\n", ++test_counter);

}




void assignmentProblemAsIntegerLinear(){
    /*

        supply       demand
        +---+        +---+
        | 1 |        | 1 |
        +---+\       +---+
              \
        +---+  \     +---+
        | 2 |---\----| 2 |
        +---+    \   +---+
                  \
        +---+      \ +---+
        | 3 |       \| 3 |
        +---+        +---+

        x_i_j = 1 iff supply i is connected to demand j

        This figure shows x_1_3 and x_2_2
    */

    OsiClpSolverInterface solver;
    CelModel model(solver);

    // we should use variable arrays but we do this way for clarity purpose, see assignmentProblemAsTotallyUnimodularWithArray
    CelBoolVar x_1_1;
    CelBoolVar x_1_2;
    CelBoolVar x_1_3;
    CelBoolVar x_2_1;
    CelBoolVar x_2_2;
    CelBoolVar x_2_3;
    CelBoolVar x_3_1;
    CelBoolVar x_3_2;
    CelBoolVar x_3_3;

    model.setObjective ( x_1_1 + x_1_2 + x_1_3 + x_2_1 + x_2_2 + x_2_3 + x_3_1 + x_3_2 + x_3_3 );

    model.addConstraint( x_1_1 + x_1_2 + x_1_3 == 1);
    model.addConstraint( x_2_1 + x_2_2 + x_2_3 == 1);
    model.addConstraint( x_3_1 + x_3_2 + x_3_3 == 1);

    model.addConstraint( x_1_1 + x_2_1 + x_3_1 == 1);
    model.addConstraint( x_1_2 + x_2_2 + x_3_2 == 1);
    model.addConstraint( x_1_3 + x_2_3 + x_3_3 == 1);

    model.addConstraint( 0 <= x_1_1 );
    model.addConstraint( 0 <= x_1_2 );
    model.addConstraint( 0 <= x_1_3 );
    model.addConstraint( 0 <= x_2_1 );
    model.addConstraint( 0 <= x_2_2 );
    model.addConstraint( 0 <= x_2_3 );
    model.addConstraint( 0 <= x_3_1 );
    model.addConstraint( 0 <= x_3_2 );
    model.addConstraint( 0 <= x_3_3 );

    solver.setObjSense(1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();

    CbcModel cbcModel(solver);
    cbcModel.setLogLevel(1);
    cbcModel.solver()->setHintParam(OsiDoReducePrint,true,OsiHintTry);
    cbcModel.branchAndBound();

    double x_1_1_value = model.getSolutionValue(x_1_1, *cbcModel.solver());
    double x_1_2_value = model.getSolutionValue(x_1_2, *cbcModel.solver());
    double x_1_3_value = model.getSolutionValue(x_1_3, *cbcModel.solver());
    double x_2_1_value = model.getSolutionValue(x_2_1, *cbcModel.solver());
    double x_2_2_value = model.getSolutionValue(x_2_2, *cbcModel.solver());
    double x_2_3_value = model.getSolutionValue(x_2_3, *cbcModel.solver());
    double x_3_1_value = model.getSolutionValue(x_3_1, *cbcModel.solver());
    double x_3_2_value = model.getSolutionValue(x_3_2, *cbcModel.solver());
    double x_3_3_value = model.getSolutionValue(x_3_3, *cbcModel.solver());

    printf("Solution for x_1_1 : %g\n", x_1_1_value);
    printf("Solution for x_1_2 : %g\n", x_1_2_value);
    printf("Solution for x_1_3 : %g\n", x_1_3_value);
    printf("Solution for x_2_1 : %g\n", x_2_1_value);
    printf("Solution for x_2_2 : %g\n", x_2_2_value);
    printf("Solution for x_2_3 : %g\n", x_2_3_value);
    printf("Solution for x_3_1 : %g\n", x_3_1_value);
    printf("Solution for x_3_2 : %g\n", x_3_2_value);
    printf("Solution for x_3_3 : %g\n", x_3_3_value);

    // totally unimodularity check
    assert2(fabs(x_1_1_value - 0) < 0.00000001 || fabs(x_1_1_value - 1) < 0.00000001);
    assert2(fabs(x_1_2_value - 0) < 0.00000001 || fabs(x_1_2_value - 1) < 0.00000001);
    assert2(fabs(x_1_3_value - 0) < 0.00000001 || fabs(x_1_3_value - 1) < 0.00000001);
    assert2(fabs(x_2_1_value - 0) < 0.00000001 || fabs(x_2_1_value - 1) < 0.00000001);
    assert2(fabs(x_2_2_value - 0) < 0.00000001 || fabs(x_2_2_value - 1) < 0.00000001);
    assert2(fabs(x_2_3_value - 0) < 0.00000001 || fabs(x_2_3_value - 1) < 0.00000001);
    assert2(fabs(x_3_1_value - 0) < 0.00000001 || fabs(x_3_1_value - 1) < 0.00000001);
    assert2(fabs(x_3_2_value - 0) < 0.00000001 || fabs(x_3_2_value - 1) < 0.00000001);
    assert2(fabs(x_3_3_value - 0) < 0.00000001 || fabs(x_3_3_value - 1) < 0.00000001);

    // constraint satisfaction
    assert2(fabs(x_1_1_value + x_1_2_value + x_1_3_value - 1) < 0.00000001);
    assert2(fabs(x_2_1_value + x_2_2_value + x_2_3_value - 1) < 0.00000001);
    assert2(fabs(x_3_1_value + x_3_2_value + x_3_3_value - 1) < 0.00000001);
    assert2(fabs(x_1_1_value + x_2_1_value + x_3_1_value - 1) < 0.00000001);
    assert2(fabs(x_1_2_value + x_2_2_value + x_3_2_value - 1) < 0.00000001);
    assert2(fabs(x_1_3_value + x_2_3_value + x_3_3_value - 1) < 0.00000001);

    printf("Test %d : OK\n", ++test_counter);

}

void testBooleanVar(){
    OsiClpSolverInterface solver;
    CelModel model(solver);

    CelBoolVar x1("x1");

    assert2(x1.isInteger());

    model.setObjective ( x1 );
    model.addConstraint( x1 <= 3.5);

    solver.setObjSense(-1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();
    CbcModel cbcModel(solver);
    cbcModel.setLogLevel(1);
    cbcModel.solver()->setHintParam(OsiDoReducePrint,true,OsiHintTry);
    cbcModel.branchAndBound();

    double x1_value = model.getSolutionValue(x1, *cbcModel.solver());
    printf("Solution for x1 : %g\n", x1_value);
    printf("Solution objvalue = : %g\n", cbcModel.solver()->getObjValue());

    assert2(fabs(x1_value - 1) < 0.00000001);
    printf("Test %d : OK\n", ++test_counter);
}


void testVarArray(){
    OsiClpSolverInterface solver;
    CelModel model(solver);

    int n = 5;
    int part = 3;


    CelNumVarArray t("t");
    t.multiDimensionResize(3, 30, 5, 2);
    CelNumVar &y = t[0][4][1];


    CelNumVarArray x("x");
    x.resize(5);

    CelExpression objective;
    for (int i=0; i<n; i++){
        objective += x[i];
    }
    model.setObjective ( objective );

    model.addConstraint( x[0] + x[2] == 2 );

    CelExpression constraint_expression;
    for (int i=part; i<n; i++){
        constraint_expression += x[i];
    }
    model.addConstraint( 3 <= constraint_expression );


    for (int i=0; i<n; i++){
        model.addConstraint( 0 <= x[i] );
    }

    solver.setObjSense(1.0);
    model.builderToSolver();
    solver.setLogLevel(0);
    solver.initialSolve();

    for (int i=0; i<n; i++){
        printf("Solution for x[%d] : %g\n", i, model.getSolutionValue(x[i]));
    }

    printf("Solution objvalue = : %g\n", solver.getObjValue());

}

// It's the task assignment problems: assign objects in A to objects in B so as to maximize the overall revenue
// Decision variables: x(a,b) in {0,1} for each a in A and b in B
// R(a,b)=exp(cos(2*PI*a*b/A*B)) is the revenue of the assignment pair (a,b)
// max sum_{a in A, b in B} x(a,b) * R(a,b)
// for each a in A: sum(b in B) x(a,b) <= 1
// for each b in B: sum(a in A) x(a,b) <= 1

void assignment() {
    unsigned int A = 40;
    unsigned int B = 20;
    double PI = std::acos(-1.0);

    std::cout << "Modeling..." << std::endl;
    OsiCbcSolverInterface solver;
    CelModel model(solver);

    // CelBoolVarArray x;
    // x.multiDimensionResize(2, A, B);

    // Commenting the 2 previous lines and commenting out the following one to test std::vector container
    std::vector<std::vector<CelBoolVar> > x(A, std::vector<CelBoolVar>(B));
    CelExpression objective;

    for (unsigned int a = 0 ; a < A ; a++) {
        for (unsigned int b = 0 ; b < B ; b++) {
            objective += std::exp(std::cos(2.0*PI*a*b/(A*B))) * x[a][b];
        }
    }

    model.setObjective(objective);

    // Constraints

    for (unsigned int a = 0 ; a < A ; a++) {
        CelExpression cst;
        for (unsigned int b = 0 ; b < B ; b++) {
            // objective += std::exp(std::cos(2.0*PI*a*b/(A*B))) * x[a][b];
            cst += x[a][b];
        }
        model.addConstraint(cst <= 1);
    }

    for (unsigned int b = 0 ; b < B ; b++) {
        CelExpression cst;
        for (unsigned int a = 0 ; a < A ; a++) {
            cst += x[a][b];
        }
        model.addConstraint(cst <= 1);
    }

    solver.setObjSense(-1.0);
    model.builderToSolver();
    // solver.writeLp("assignment");
    std::cout << "Optimizing..." << std::endl;
    solver.initialSolve();
    solver.branchAndBound();
    std::cout << "Assignments:" << std::endl;

    for (unsigned int a = 0 ; a < A ; a++) {
        for (unsigned int b = 0 ; b < B ; b++) {
            if (model.getSolutionValue(x[a][b]) == 1) {
                std::cout << "   " << a << " -> " << b << std::endl;
            }
        }
    }
}


int main(int argc, char *argv[]){
    testVarArray();
    testBooleanVar();
    assignmentProblemAsIntegerLinear();
    assignmentProblemAsTotallyUnimodular();
    assignmentProblemAsTotallyUnimodularWithArray();
    exemple1();
    exemple2();
    exemple3();
    exemple4();
    chineseRemainders();
    expressionTest();
    assignment();

    printf("Everything is OK\n");
}































