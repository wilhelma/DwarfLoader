//
// Created by Faris Cakaric on 28/05/2017.
//

#include "SetExpression.h"
#include "../visitor/Visitor.h"

void SetExpression::accept(Visitor &v) {
    v.visit(*this);
}

SetExpression::SetExpression(
        std::vector<std::unique_ptr<Artifact>> &terms) {
    this->terms = std::move(terms);

}

const std::vector<std::unique_ptr<Artifact>, std::allocator<std::unique_ptr<Artifact>>> &
SetExpression::getTerms() const {
    return terms;
}
