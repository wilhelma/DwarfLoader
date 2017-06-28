//
// Created by Faris Cakaric on 28/05/2017.
//

#include "SetExpression.h"
#include "../visitor/Visitor.h"

void SetExpression::accept(Visitor &v) {
    v.visit(*this);
}

SetExpression::SetExpression(
        std::vector<std::unique_ptr<Component>> &terms) {
    this->terms = std::move(terms);

}

const std::vector<std::unique_ptr<Component>, std::allocator<std::unique_ptr<Component>>> &
SetExpression::getTerms() const {
    return terms;
}
