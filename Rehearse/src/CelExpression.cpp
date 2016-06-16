#include "CelExpression.h"

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

namespace rehearse {

CelExpression::CelExpression() : column_index(-1),
                                 node_type(NODE_UNKNOWN),
                                 left(NULL), right(NULL),
                                 constant_value(0.0),
                                 constant_coefficient(0.0),
                                 deletable(false) {
}

CelExpression::CelExpression(bool deletable) : column_index(-1),
                                               node_type(NODE_UNKNOWN),
                                               left(NULL), right(NULL),
                                               constant_value(0.0),
                                               constant_coefficient(0.0),
                                               deletable(deletable) {
}


CelExpression::~CelExpression(){
    if (left && left->deletable)
        delete left;
    if (right && right->deletable)
        delete right;
}

void CelExpression::attributeColumnIndex(
    vector<CelVariable *> &model_variables){

    CelExpression *cur_node = this;

    while (cur_node){
        if (cur_node->node_type == NODE_PROXY){
            left->attributeColumnIndex(model_variables);
        } else if (cur_node->node_type == NODE_VARIABLE){
            if (cur_node->column_index == -1){
                cur_node->column_index = model_variables.size();
                model_variables.push_back((CelVariable *)cur_node);
            }
        } else {
            if (cur_node->right){
                cur_node->right->attributeColumnIndex(model_variables);
            }
        }
        cur_node = cur_node->left;
    }
}

void CelExpression::fillExpressionLinearCoefficients(
    LinearCoefficientMap &linear_coefficients){

    fillLinearCoefficientMap();

    for (const auto &it : coefficient_map){
        linear_coefficients[it.first] = it.second;
    }
}


void CelExpression::fillConstraintLinearCoefficients(
    LinearCoefficientMap &linear_coefficients,
    double infinity,
    double &lower,
    double &upper){

    if (node_type == NODE_PROXY){
        return left->fillConstraintLinearCoefficients(linear_coefficients,
                                                      infinity,
                                                      lower, upper);
    }

    if (node_type == NODE_OP_LTE){
        if (left->node_type == NODE_OP_LTE){
            left->fillLinearCoefficientMap();
            right->fillLinearCoefficientMap();

            for (const auto &it : left->coefficient_map){
                coefficient_map[it.first] = -1.0 * it.second;
            }

            for (const auto &it : right->coefficient_map){
                if (!coefficient_map.count(it.first)){
                    coefficient_map[it.first] = 0.0;
                }

                coefficient_map[it.first] -= it.second;
            }

            for (const auto &it : coefficient_map){
                linear_coefficients[it.first] = it.second;
            }

            lower = left->constant_coefficient;
            upper = right->constant_coefficient;

        } else {
            fillLinearCoefficientMap();

            for (const auto &it : coefficient_map){
                linear_coefficients[it.first] = it.second;
            }

            lower = -infinity;
            upper = -1.0 * this->constant_coefficient;

        }

    } else if (node_type == NODE_OP_EQ) {
        fillLinearCoefficientMap();

        for (const auto &it : coefficient_map){
            linear_coefficients[it.first] = it.second;
        }

        lower = -1.0 * this->constant_coefficient;
        upper = -1.0 * this->constant_coefficient;
    }
}


void CelExpression::fillLinearCoefficientMap(){
    if (node_type == NODE_PROXY){
        left->fillLinearCoefficientMap();
        for (const auto &it : left->coefficient_map){
            coefficient_map[it.first] = it.second;
        }

        return;
    }

    coefficient_map.clear();

    if (node_type == NODE_VARIABLE){

        coefficient_map[column_index] = 1.0;

    } else if (node_type == NODE_CONSTANT){

        constant_coefficient = constant_value;

    } else if (isComparisonOrAdditiveOperator(node_type)){
        CelExpression *cur_node = this;

        while (isComparisonOrAdditiveOperator(cur_node->node_type)){

            cur_node->right->fillLinearCoefficientMap();
            for (const auto &it : cur_node->right->coefficient_map){
                if (!coefficient_map.count(it.first)){
                    coefficient_map[it.first] = 0.0;
                }

                if (cur_node->node_type == NODE_OP_ADD){
                    coefficient_map[it.first] += it.second;
                } else if (cur_node->node_type == NODE_OP_SUB ||
                           isComparisonOperator(cur_node->node_type)) {
                    coefficient_map[it.first] -= it.second;
                }
            }

            if (cur_node->node_type == NODE_OP_ADD){
                constant_coefficient += cur_node->right->constant_coefficient;
            } else if (cur_node->node_type == NODE_OP_SUB ||
                       isComparisonOperator(cur_node->node_type)){
                constant_coefficient -= cur_node->right->constant_coefficient;
            }

            if (!isComparisonOrAdditiveOperator(cur_node->left->node_type)){
                cur_node->left->fillLinearCoefficientMap();
                for (const auto &it : cur_node->left->coefficient_map){
                    coefficient_map[it.first] = it.second;
                }
                constant_coefficient += cur_node->left->constant_coefficient;
            }

            cur_node = cur_node->left;
        }

    } else if (node_type == NODE_OP_MULT){

        if (left->node_type == NODE_CONSTANT){

            if (right->node_type == NODE_VARIABLE){
                coefficient_map[right->column_index] = left->constant_value;
            } else {
                right->fillLinearCoefficientMap();
                for (const auto &it : right->coefficient_map){
                    coefficient_map[it.first] = left->constant_value *
                                                it.second;
                }
            }

        } else if (right->node_type == NODE_CONSTANT){

            if (left->node_type == NODE_VARIABLE){
                coefficient_map[left->column_index] = right->constant_value;
            } else {
                left->fillLinearCoefficientMap();
                for (const auto &it : left->coefficient_map){
                    coefficient_map[it.first] = right->constant_value *
                                                it.second;
                }
            }
        }

    } else if (node_type == NODE_OP_DIV){

    }

}

bool CelExpression::hasSeveralConstraints(){
    if (node_type == NODE_PROXY){
        return left->hasSeveralConstraints();
    }

    if (node_type == NODE_OP_LTE && left->node_type == NODE_OP_LTE)
        return true;

    return false;
}


bool CelExpression::isDeletable() const{
    return this->deletable;
}

void CelExpression::display() const{
    this->display(0);
}

void CelExpression::displayFlat(bool crlf) const{
    if (this->left)
        this->left->displayFlat(false);
    this->displayMeOnly();
    if (this->right)
        this->right->displayFlat(false);
    if (crlf)
        printf("\n");
}

void CelExpression::display(int indent) const{
    if (this->right)
        this->right->display(indent+1);

    for(int i=0; i<indent*4; i++){
        printf(" ");
    }
    printf("[");
    this->displayMeOnly();
    printf("]\n");

    if (this->left)
        this->left->display(indent+1);

}

void CelExpression::displayMeOnly() const{
    switch (this->node_type){
        case NODE_PROXY:
            printf(" proxy ");
            break;
        case NODE_CONSTANT:
            printf(" %f ", this->constant_value);
            break;
        case NODE_OP_ADD:
            printf(" + ");
            break;
        case NODE_OP_SUB:
            printf(" - ");
            break;
        case NODE_OP_MULT:
            printf(" * ");
            break;
        case NODE_OP_DIV:
            printf(" / ");
            break;
        case NODE_OP_LTE:
            printf(" <= ");
            break;
        case NODE_OP_EQ:
            printf(" == ");
            break;
        default:
            printf("N/A");
            break;
    };
#ifdef DEBUG_ADDRESS
    printf("(%p)", this);
#endif
}

bool CelExpression::isVariable() const{
    if (this->node_type == NODE_PROXY){
        return left->isVariable();
    }

    switch (this->node_type){
        case NODE_CONSTANT:
            return false;
        case NODE_VARIABLE:
            return true;

        case NODE_OP_ADD:
        case NODE_OP_SUB:
        case NODE_OP_MULT:
        case NODE_OP_DIV:
            return left->isVariable() || right->isVariable();

        case NODE_OP_LTE:
            return true;
        default:
            break;
    };
    return false;
}

bool CelExpression::isConstant() const{
    if (this->node_type == NODE_PROXY){
        return left->isConstant();
    }

    switch (this->node_type){
        case NODE_CONSTANT:
            return true;
        case NODE_VARIABLE:
            return false;

        case NODE_OP_ADD:
        case NODE_OP_SUB:
        case NODE_OP_MULT:
        case NODE_OP_DIV:
            return left->isConstant() && right->isConstant();

        case NODE_OP_LTE:
            return true;
        default:
            break;
    };
    return false;
}

bool CelExpression::isLinear() const{
    if (this->node_type == NODE_PROXY){
        return left->isLinear();
    }

    switch (this->node_type){
        case NODE_CONSTANT:
        case NODE_VARIABLE:
            return true;


        case NODE_OP_ADD:
        case NODE_OP_SUB:
            {
                const CelExpression *cur_node = this;

                while (cur_node->node_type == NODE_OP_ADD ||
                       cur_node->node_type == NODE_OP_SUB){
                    if (!right->isLinear()){
                        return false;
                    }
                    cur_node = cur_node->left;
                }
                return cur_node->isLinear();
            }

        case NODE_OP_MULT:
        case NODE_OP_DIV:
            {
                bool left_is_variable = left && left->isVariable();
                bool right_is_variable = right && right->isVariable();

                return !(left_is_variable && right_is_variable);
            }

        case NODE_OP_LTE:
        case NODE_OP_EQ:
            return left->isLinear() && right->isLinear();

        case NODE_UNKNOWN:
            return true;

        default:
            return false;
            break;
    };
    return false;
}

bool CelExpression::isConsistentConstraint() const{
    if (node_type == NODE_PROXY){
        return left->isConsistentConstraint();
    }

    if (node_type == NODE_OP_LTE){
        //if (right->isConstant()){
            if (!isComparisonOperator(left->node_type)){
                return true;
            }
            if (left->node_type == NODE_OP_LTE){
                if (left->left->isConstant() &&
                    !isComparisonOperator(left->right->node_type)){
                    return true;
                }
            }
            return false;
        //}
    } else if (node_type == NODE_OP_EQ){
        if (!isComparisonOperator(left->node_type)){
            return true;
        }
    }

    return false;
}

bool CelExpression::isComparisonOrAdditiveOperator(NodeType node_type) const{
    return (node_type == NODE_OP_ADD ||
            node_type == NODE_OP_SUB ||
            isComparisonOperator(node_type));
}
bool CelExpression::isComparisonOperator(NodeType node_type) const{
    return (node_type == NODE_OP_LTE ||
            node_type == NODE_OP_EQ);
}
bool CelExpression::isBinaryOperator(NodeType node_type) const{
    return (node_type == NODE_OP_ADD ||
            node_type == NODE_OP_SUB ||
            node_type == NODE_OP_MULT ||
            node_type == NODE_OP_DIV);
}


CelExpression & operator <= (CelExpression &left_expression,
                             double constant_value){
    CelExpression *right_expression = new CelExpression(true);
    right_expression->node_type = CelExpression::NODE_CONSTANT;
    right_expression->constant_value = constant_value;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_LTE;
    parent_expression->left = &left_expression;
    parent_expression->right = right_expression;

    return *parent_expression;
}

CelExpression & operator <= (double constant_value,
                             CelExpression &right_expression){
    CelExpression *left_expression = new CelExpression(true);
    left_expression->node_type = CelExpression::NODE_CONSTANT;
    left_expression->constant_value = constant_value;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_LTE;
    parent_expression->left = left_expression;
    parent_expression->right = &right_expression;

    return *parent_expression;
}

CelExpression & operator <= (CelExpression &left_expression,
                             CelExpression &right_expression){
    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_LTE;
    parent_expression->left = &left_expression;
    parent_expression->right = &right_expression;

    return *parent_expression;
}

CelExpression & operator == (CelExpression &left_expression,
                             double constant_value){
    CelExpression *right_expression = new CelExpression(true);
    right_expression->node_type = CelExpression::NODE_CONSTANT;
    right_expression->constant_value = constant_value;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_EQ;
    parent_expression->left = &left_expression;
    parent_expression->right = right_expression;

    return *parent_expression;
}

CelExpression & operator == (double constant_value,
                             CelExpression &right_expression){
    CelExpression *left_expression = new CelExpression(true);
    left_expression->node_type = CelExpression::NODE_CONSTANT;
    left_expression->constant_value = constant_value;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_EQ;
    parent_expression->left = left_expression;
    parent_expression->right = &right_expression;

    return *parent_expression;
}

CelExpression & operator == (CelExpression &left_expression,
                             CelExpression &right_expression){
    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_EQ;
    parent_expression->left = &left_expression;
    parent_expression->right = &right_expression;

    return *parent_expression;
}

CelExpression & operator * (CelExpression &left_expression,
                            double constant_value){
    CelExpression *right_expression = new CelExpression(true);
    right_expression->node_type = CelExpression::NODE_CONSTANT;
    right_expression->constant_value = constant_value;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_MULT;
    parent_expression->left = &left_expression;
    parent_expression->right = right_expression;

    return *parent_expression;
}

CelExpression & operator * (double constant_value,
                            CelExpression &right_expression){
    CelExpression *left_expression = new CelExpression(true);
    left_expression->node_type = CelExpression::NODE_CONSTANT;
    left_expression->constant_value = constant_value;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_MULT;
    parent_expression->left = left_expression;
    parent_expression->right = &right_expression;

    return *parent_expression;
}
CelExpression & operator * (CelExpression &left_expression,
                            CelExpression &right_expression){
    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_MULT;
    parent_expression->left = &left_expression;
    parent_expression->right = &right_expression;

    return *parent_expression;
}

CelExpression & operator + (CelExpression &left_expression,
                            double constant_value){
    CelExpression *right_expression = new CelExpression(true);
    right_expression->node_type = CelExpression::NODE_CONSTANT;
    right_expression->constant_value = constant_value;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_ADD;
    parent_expression->left = &left_expression;
    parent_expression->right = right_expression;

    return *parent_expression;
}

CelExpression & operator + (double constant_value,
                            CelExpression &right_expression){
    CelExpression *left_expression = new CelExpression(true);
    left_expression->node_type = CelExpression::NODE_CONSTANT;
    left_expression->constant_value = constant_value;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_ADD;
    parent_expression->left = left_expression;
    parent_expression->right = &right_expression;

    return *parent_expression;
}
CelExpression & operator + (CelExpression &left_expression,
                            CelExpression &right_expression){
    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_ADD;
    parent_expression->left = &left_expression;
    parent_expression->right = &right_expression;

    return *parent_expression;
}

CelExpression & operator - (CelExpression &left_expression,
                            double constant_value){
    CelExpression *right_expression = new CelExpression(true);
    right_expression->node_type = CelExpression::NODE_CONSTANT;
    right_expression->constant_value = constant_value;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_SUB;
    parent_expression->left = &left_expression;
    parent_expression->right = right_expression;

    return *parent_expression;
}

CelExpression & operator - (double constant_value,
                            CelExpression &right_expression){
    CelExpression *left_expression = new CelExpression(true);
    left_expression->node_type = CelExpression::NODE_CONSTANT;
    left_expression->constant_value = constant_value;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_SUB;
    parent_expression->left = left_expression;
    parent_expression->right = &right_expression;

    return *parent_expression;
}

CelExpression & operator - (CelExpression &left_expression,
                            CelExpression &right_expression){
    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_SUB;
    parent_expression->left = &left_expression;
    parent_expression->right = &right_expression;

    return *parent_expression;
}

CelExpression & operator - (CelExpression &expression){
    CelExpression *left_expression = new CelExpression(true);
    left_expression->node_type = CelExpression::NODE_CONSTANT;
    left_expression->constant_value = -1.0;

    CelExpression *parent_expression = new CelExpression(true);
    parent_expression->node_type = CelExpression::NODE_OP_MULT;
    parent_expression->left = left_expression;
    parent_expression->right = &expression;

    return *parent_expression;
}

CelExpression & CelExpression::operator += (CelExpression &expression){
    CelExpression *my_copy = new CelExpression(*this);
    my_copy->deletable = true;

    this->node_type = CelExpression::NODE_OP_ADD;
    this->left = my_copy;
    this->right = &expression;

    return *this;
}

CelExpression & CelExpression::operator = (CelExpression &expression){
    this->node_type = CelExpression::NODE_PROXY;
    this->left = &expression;
    this->right = NULL;

    return *this;
}


}   // namespace rehearse

