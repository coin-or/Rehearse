#ifndef INC_CELEXPRESSION_H
#define INC_CELEXPRESSION_H

#include <vector>

#include "CelCoefficients.fwd.h"

namespace rehearse {

class CelModel;
class CelVariable;

class CelExpression {
    friend class CelModel;


protected:
    // Expression is a binary tree with operators and operands
    // we keep the tree information directly in this class because overrided
    // operators must return CelExpression
    enum NodeType { NODE_UNKNOWN, NODE_PROXY, NODE_VARIABLE, NODE_CONSTANT,
                    NODE_OP_ADD, NODE_OP_SUB, NODE_OP_MULT, NODE_OP_DIV,
                    NODE_OP_LTE, NODE_OP_EQ };

    int column_index;
    LinearCoefficientMap coefficient_map;
    double constant_coefficient;
    NodeType node_type;
    CelExpression *left;
    CelExpression *right;
    double constant_value;
    bool deletable;

    virtual void displayMeOnly() const;
    bool isComparisonOrAdditiveOperator(NodeType node_type) const;
    bool isComparisonOperator(NodeType node_type) const;
    bool isBinaryOperator(NodeType node_type) const;
    void attributeColumnIndex(std::vector<CelVariable *> &model_variables);
    void fillConstraintLinearCoefficients(
        LinearCoefficientMap &linear_coefficients,
        double infinity,
        double &lower, double &upper);
    void fillExpressionLinearCoefficients(
        LinearCoefficientMap &linear_coefficients);
    void fillLinearCoefficientMap();
    bool hasSeveralConstraints();

    CelExpression(bool deletable);

public:

    friend CelExpression & operator <= (CelExpression &left_expression,
                                        double constant_value);
    friend CelExpression & operator <= (double constant_value,
                                        CelExpression &right_expression);
    friend CelExpression & operator <= (CelExpression &left_expression,
                                        CelExpression &right_expression);
    friend CelExpression & operator == (CelExpression &left_expression,
                                        double constant_value);
    friend CelExpression & operator == (double constant_value,
                                        CelExpression &right_expression);
    friend CelExpression & operator == (CelExpression &left_expression,
                                        CelExpression &right_expression);
    friend CelExpression & operator * (CelExpression &left_expression,
                                       double constant_value);
    friend CelExpression & operator * (double constant_value,
                                       CelExpression &right_expression);
    friend CelExpression & operator * (CelExpression &left_expression,
                                       CelExpression &right_expression);
    friend CelExpression & operator + (CelExpression &left_expression,
                                       double constant_value);
    friend CelExpression & operator + (double constant_value,
                                       CelExpression &right_expression);
    friend CelExpression & operator + (CelExpression &left_expression,
                                       CelExpression &right_expression);
    friend CelExpression & operator - (CelExpression &left_expression,
                                       double constant_value);
    friend CelExpression & operator - (double constant_value,
                                       CelExpression &right_expression);
    friend CelExpression & operator - (CelExpression &left_expression,
                                       CelExpression &right_expression);
    friend CelExpression & operator - (CelExpression &expression);

    CelExpression & operator += (CelExpression &expression);
    CelExpression & operator = (CelExpression &expression);

    void display() const;
    void display(int indent) const;
    void displayFlat(bool crlf=true) const;
    bool isDeletable() const;
    bool isLinear() const;
    bool isVariable() const;
    bool isConstant() const;
    bool isConsistentConstraint() const;

    CelExpression();
    virtual ~CelExpression();
};

}   // namespace rehearse

#endif
