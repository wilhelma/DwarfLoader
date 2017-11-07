//
// Created by Faris Cakaric on 28/05/2017.
//

#include "DefinitionExpression.h"
#include "../visitor/Visitor.h"


void DefinitionExpression::accept(Visitor &v) {
    v.visit(*this);
}

DefinitionExpression::DefinitionExpression(std::unique_ptr<Artifact> &artifact,
                                           std::unique_ptr<Expression> &expression) {

    this->artifact = std::move(artifact);
    this->expression = std::move(expression);
}

const std::unique_ptr<Artifact> &DefinitionExpression::getArtifact() const {
    return artifact;
}

const std::unique_ptr<Expression> &DefinitionExpression::getExpression() const {
    return expression;
}
