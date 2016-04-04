#ifndef INC_CELEXPRESSION_H
#define INC_CELEXPRESSION_H

#include "CelCoefficients.fwd.h"

using namespace std;

class CelModel;
class CelVariable;

class CelExpression {
    friend class CelModel;


protected:
    int column_index;
    LinearCoefficientMap coefficient_map;
    double constant_coefficient;
    void attributeColumnIndex(vector<CelVariable *> &model_variables);
    void fillConstraintLinearCoefficients(LinearCoefficients &linear_coefficients, double infinity, double &lower, double &upper);
    void fillExpressionLinearCoefficients(LinearCoefficients &linear_coefficients);
    void fillLinearCoefficientMap();
    bool hasSeveralConstraints();

    // Expression is a binary tree with operators and operands
    // we keep the tree information directly in this class because overrided operators must return CelExpression
    enum NodeType { NODE_UNKNOWN, NODE_PROXY, NODE_VARIABLE, NODE_CONSTANT,
                    NODE_OP_ADD, NODE_OP_SUB, NODE_OP_MULT, NODE_OP_DIV,
                    NODE_OP_LTE, NODE_OP_EQ };
    NodeType node_type;
    CelExpression *left;
    CelExpression *right;
    double constant_value;

    bool deletable;
    virtual void displayMeOnly();
    bool isComparisonOrAdditiveOperator(NodeType node_type);
    bool isComparisonOperator(NodeType node_type);
    bool isBinaryOperator(NodeType node_type);

    CelExpression(bool deletable);

public:

    friend CelExpression & operator <= (CelExpression &left_expression, double constant_value);
    friend CelExpression & operator <= (double constant_value, CelExpression &right_expression);
    friend CelExpression & operator <= (CelExpression &left_expression, CelExpression &right_expression);
    friend CelExpression & operator == (CelExpression &left_expression, double constant_value);
    friend CelExpression & operator == (double constant_value, CelExpression &right_expression);
    friend CelExpression & operator == (CelExpression &left_expression, CelExpression &right_expression);
    friend CelExpression & operator * (CelExpression &left_expression, double constant_value);
    friend CelExpression & operator * (double constant_value, CelExpression &right_expression);
    friend CelExpression & operator * (CelExpression &left_expression, CelExpression &right_expression);
    friend CelExpression & operator + (CelExpression &left_expression, double constant_value);
    friend CelExpression & operator + (double constant_value, CelExpression &right_expression);
    friend CelExpression & operator + (CelExpression &left_expression, CelExpression &right_expression);
    friend CelExpression & operator - (CelExpression &left_expression, double constant_value);
    friend CelExpression & operator - (double constant_value, CelExpression &right_expression);
    friend CelExpression & operator - (CelExpression &left_expression, CelExpression &right_expression);
    friend CelExpression & operator - (CelExpression &expression);

    CelExpression & operator += (CelExpression &expression);
    CelExpression & operator = (CelExpression &expression);

    void display();
    void display(int indent);
    void displayFlat(bool crlf=true);
    bool isDeletable();
    bool isLinear();
    bool isVariable();
    bool isConstant();
    bool isConsistentConstraint();

    CelExpression();
    virtual ~CelExpression();
};


#endif
