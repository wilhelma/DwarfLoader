//
// Created by Faris Cakaric on 27.08.17.
//

#ifndef DWARFLOADER_ASSIGNMENTEXPRESSION_H
#define DWARFLOADER_ASSIGNMENTEXPRESSION_H

#include "Expression.h"
#include "Artifact.h"

#include <json11.hpp>

using json11::Json;

class AssignmentExpression : public Expression {
  std::unique_ptr<Artifact> artifact;
  std::unique_ptr<Expression> expression;
public:
  AssignmentExpression(std::unique_ptr<Artifact> &artifact, std::unique_ptr<Expression> &expression);

  const std::unique_ptr<Artifact> &getArtifact() const;

  const std::unique_ptr<Expression> &getExpression() const;

  virtual void accept(Visitor& v) override;
};


#endif //DWARFLOADER_ASSIGNMENTEXPRESSION_H
