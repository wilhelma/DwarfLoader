//
// Created by Faris Cakaric on 27.08.17.
//

#include "AssignmentExpression.h"
#include "../visitor/Visitor.h"


void AssignmentExpression::accept(Visitor &v) {
  v.visit(*this);
}

AssignmentExpression::AssignmentExpression(std::unique_ptr<Artifact> &artifact,
                                           std::unique_ptr<Expression> &expression) {

  this->artifact = std::move(artifact);
  this->expression = std::move(expression);
}

const std::unique_ptr<Artifact> &AssignmentExpression::getArtifact() const {
  return artifact;
}

const std::unique_ptr<Expression> &AssignmentExpression::getExpression() const {
  return expression;
}
