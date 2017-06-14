//
// Created by Faris Cakaric on 28/05/2017.
//

#include "Program.h"
#include "../visitor/Visitor.h"


void Program::accept(Visitor &v) {
    v.visit(*this);
}

Program::Program(
        std::vector<std::unique_ptr<DefinitionExpression>> &expressions) {
    this->expressions = std::move(expressions);
}

const std::vector<std::unique_ptr<DefinitionExpression>, std::allocator<std::unique_ptr<DefinitionExpression>>> &
Program::getExpressions() const {
    return expressions;
}
