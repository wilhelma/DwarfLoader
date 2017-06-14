//
// Created by Faris Cakaric on 28/05/2017.
//

#include "DefinitionExpression.h"
#include "../visitor/Visitor.h"


void DefinitionExpression::accept(Visitor &v) {
    v.visit(*this);
}

DefinitionExpression::DefinitionExpression(std::unique_ptr<Component> &component,
                                           std::unique_ptr<Expression> &expression) {

    this->component = std::move(component);
    this->expression = std::move(expression);
}

const std::unique_ptr<Component> &DefinitionExpression::getComponent() const {
    return component;
}

const std::unique_ptr<Expression> &DefinitionExpression::getExpression() const {
    return expression;
}
