
Rehearse
========



Introduction
------------

Rehearse is a C++ library designed to describe linear optimization problems programmatically.

You can describe linear objectives and constraints using native C++ algebraic expressions, in a similar way you write a mathematical model on a paper.


###Usage :

First declare your variables :

    CelNumVar x1;
    CelNumVar x2;

That's it! Now you are ready to use them in algebraic expressions.

For example, to set the objective :

    model.setObjective (       7 * x1 + 9 * x2 );

and to add constraints :

    model.addConstraint(       1 * x1 +     x2 == 18  );
    model.addConstraint(                    x2 <= 14  );
    model.addConstraint(       2 * x1 + 3 * x2 <= 50  );


Then the solver will try to find values for `x1` and `x2` which give the biggest value for `7*x1 + 9*x2`, subject to constraints we described.

The complete example:

```cpp
    void exemple1(){
        OsiClpSolverInterface solver;
        CelModel model(solver);

        CelNumVar x1;
        CelNumVar x2;

        model.setObjective (       7 * x1 + 9 * x2 );

        model.addConstraint(       1 * x1 +     x2 == 18  );
        model.addConstraint(                    x2 <= 14  );
        model.addConstraint(       2 * x1 + 3 * x2 <= 50  );

        solver.setObjSense(-1.0);
        model.builderToSolver();
        solver.setLogLevel(0);
        solver.initialSolve();

        printf("Solution for x1 : %g\n", model.getSolutionValue(x1));
        printf("Solution for x2 : %g\n", model.getSolutionValue(x2));
        printf("Solution objective value = : %g\n", solver.getObjValue());

    }
```


What's the trick?
-----------------
Rehearse builds a linear expression tree from algebraic C++ expressions using operator overloading feature of C++. It extracts linear coefficients to fill the constraint matrix which is then given as input to the open-source OsiSolver.

Then the default linear optimization solver Coin-Clp takes care of solving the linear program.

Expression trees are not template based, thus they are built at run-time. That gives us the flexibility we need to deal with variable arrays (see below).





Variable arrays
---------------

Rehearse comes with a convenient way to deal with indexed variables. Let say you have a model with `x_i_j` variables where `1 ≤ i ≤ N` and `1 ≤ j ≤ L`.

Rehearse enables you to declare a multi-dimensional array that you use with a seamless C++ syntax :

    CelNumVarArray x;
    x.multiDimensionResize(2, N, L);

    ...

    model.addConstraint( x[i][j] <= 3             );
    model.addConstraint( x[i][j] <= 1 + x[i][j+1] );

of course, you can use as many dimensions as you want to

    CelNumVarArray x;
    x.multiDimensionResize(6, N, M, O, P, Q, R);
    ...
    model.addConstraint( x[i][j][k][l][m][n] <= 3 );







Dynamic expressions
-------------------
The most important feature of Rehearse is it's ability to handle dynamic expressions. You can programmatically describe an expression and use it in your model.

    CelExpression expr;

    for (int i=0; i<N; i++){
        for (int j=0; j<L; j++){

            expr += x[i][j];

        }
    }

    model.addConstraint( expr <= 5 );






Mixed integer linear programming
--------------------------------
If you want some of your variables to be integers, use `CelIntVar` instead of `CelNumVar`. You must bind the solver to an Integer Linear Programming solver as well, for example Coin-cbc.






Examples
--------
Let's consider an assignment problem like this :

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

[See the formal mathematical definition here](http://en.wikipedia.org/wiki/Assignment_problem#Formal_mathematical_definition)

We define a multi-dimensional variable `x` :

    int S = 3;  // number of supply
    int D = 3;  // number of demand

    CelNumVarArray x;
    x.multiDimensionResize(2, S, D);

The objective is to minimize the cost of the sum of assignments :

    CelExpression objective;
    for (int i=0; i<S; i++){
        for (int j=0; j<D; j++){
            objective += x[i][j];
        }
    }
    model.setObjective ( objective );

Note that we could have weighted any assignment by writing something like `objective += cost[i][j] * x[i][j];`

Then we define constraints in a similar way to the underlying mathematical model :

Each supply must supply something:


![Each supply must supply something](http://upload.wikimedia.org/math/b/8/3/b83c2f157ff8d8ebf768686e93e2538c.png)

    for (int i=0; i<S; i++){
        CelExpression supplyConstraint;
        for (int j=0; j<D; j++){
            supplyConstraint += x[i][j];
        }
        model.addConstraint( supplyConstraint == 1 );
    }


Each demand must be fulfilled:


![Each demand must be fulfilled](http://upload.wikimedia.org/math/7/4/7/747785aed264783377bd36da70f7aa7c.png)

    for (int j=0; j<D; j++){
        CelExpression demandConstraint;
        for (int i=0; i<S; i++){
            demandConstraint += x[i][j];
        }
        model.addConstraint( demandConstraint == 1 );
    }


Variables must be positive:
![Variables must be positive](http://upload.wikimedia.org/math/0/1/b/01b1c9ea20f6071abe6de1b8e9693500.png)

    for (int i=0; i<S; i++){
        for (int j=0; j<D; j++){
            model.addConstraint( 0 <= x[i][j] );
        }
    }






Rehearse/tests/testRehearse.cpp contains other examples like :

 - Chinese remainders resolution with ILP
 - Various versions of assignment problem
 - Various linear and integer linear programs
 - Extensive use of variable arrays





How to install
--------------
See INSTALL




How to contribute
-----------------
Before contributing make sure all the tests are OK.
Beware with dynamic aspect of `CelExpression` class, make sure the library doesn't leak memory by running valgrind.

In the directory `Rehearse/tests/`, run :

    valgrind --tool=memcheck --leak-check=yes ./unitTest > analyze.log 2>&1

or

    valgrind --tool=memcheck --leak-check=full ./unitTest > analyze.log 2>&1

depending on your valgrind version. Verify that the log has

    ==????==    definitely lost: 0 bytes in 0 blocks
    ==????==    indirectly lost: 0 bytes in 0 blocks
    ==????==      possibly lost: 0 bytes in 0 blocks

or a suite of

    All heap blocks were freed -- no leaks are possible



Frequently asked questions
--------------------------

- Does the library leak memory? No it doesn't. Potential contributors will be asked to validate valgrind memory leak checks with the tests.

- What is the maximum dimension of data, variables and constraints? There is no limit, except your memory of course. The multi-dimensional array is in fact implemented as a one-dimensional array that just exposes a clever api in order to mimic multi-dimensional array. Beside that, it can raise `out_of_range` exceptions when an out of range index is requested for one of it's dimensions.

- Operator overloading is bad. Why do you use it? It's usually bad because it's misused. Overloading is okay when you don't lose the meaning of an operation by using it's operator syntax, (ie `+` means "addition", `*` means "multiplication", ... ) which is the case here.







Behind the scenes
-----------------
Behind the scenes, Rehearse builds expression trees at runtime.

An expression like

    2 * x1 + 5 * x2

becomes:


                    +---+
             -------| + |------
            /       +---+       \
        +----+                +----+
        | *  |                | *  |
        +----+                +----+
         /  \                  /  \
        /    \                /    \
    +----+    +----+      +----+    +----+
    | 2  |    | x1 |      | 5  |    | x2 |
    +----+    +----+      +----+    +----+

Nodes of this expression tree are instances of `CelExpression` class. `node_type` of each instance defines the nature of the node. In this example we see `CelExpression` instances which have `NODE_OP_ADD`, `NODE_OP_MULT`, `NODE_CONSTANT` and `NODE_VARIABLE` as `node_type`s. Their children are determined by `left` and `right` pointers.

Variables `x1` and `x2` are instances of `CelVariable` which derives from `CelExpression`.

Dealing with memory:

When a dynamically built expression like this won't be used anymore, it must be destroyed. Its destructor is called when leaving the scope of the expression or by some of `CelModel` class function members such as `setObjective` or `addConstraint` after traversing it to extract linear coefficients. The memory occupied by `x1` and `x2` are managed by the compiler, or the programmer if they were created dynamically with `new` keyword. But other nodes for `+`, `*` and constants are created dynamically to build this tree. Therefore, they should be deleted when this expression tree has to be deleted. `CelExpression` instances have a `deletable` member which determines if they should be deleted when deleting the expression tree. In this example, `x1` and `x2` have `deletable` member to `false` while other nodes have `deletable` member to `true`.




Contact
-------
Nereo SARL.
onur.celebi@nereo.fr







