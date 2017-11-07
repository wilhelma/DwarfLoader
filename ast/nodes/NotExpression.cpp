//
// Created by Faris Cakaric on 28/05/2017.
//

#include "NotExpression.h"
#include "../visitor/Visitor.h"

void NotExpression::accept(Visitor &v) {
    v.visit(*this);
}

NotExpression::NotExpression(std::unique_ptr<Expression> &operand) {
    this->operand = std::move(operand);
}
const std::unique_ptr<Expression> &NotExpression::getOperand() const {
    return operand;
}
