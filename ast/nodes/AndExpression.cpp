//
// Created by Faris Cakaric on 28/05/2017.
//

#include "AndExpression.h"
#include "../visitor/Visitor.h"



void AndExpression::accept(Visitor &v)  {
    v.visit(*this);
}

AndExpression::AndExpression(std::unique_ptr<Expression> &left, std::unique_ptr<Expression> &right) {
    this->left = std::move(left);
    this->right = std::move(right);
}

const std::unique_ptr<Expression> &AndExpression::getLeft() const {
    return left;
}

const std::unique_ptr<Expression> &AndExpression::getRight() const {
    return right;
}
