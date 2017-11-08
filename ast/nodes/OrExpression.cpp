//
// Created by Faris Cakaric on 28/05/2017.
//

#include "OrExpression.h"
#include "../visitor/Visitor.h"


void OrExpression::accept(Visitor &v) {
    v.visit(*this);
}

OrExpression::OrExpression(std::unique_ptr<Expression> &left, std::unique_ptr<Expression> &right) {
    this->left = std::move(left);
    this->right = std::move(right);
}

const std::unique_ptr<Expression> &OrExpression::getLeft() const {
    return left;
}

const std::unique_ptr<Expression> &OrExpression::getRight() const {
    return right;
}
