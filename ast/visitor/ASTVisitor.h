//
// Created by Faris Cakaric on 25.05.17.
//

#ifndef META_ADL_PARSER_VISITOR_ASTVISITOR_H
#define META_ADL_PARSER_VISITOR_ASTVISITOR_H

#include "Visitor.h"
#include "VisitorContext.h"

#include <iostream>

class ASTVisitor : public Visitor {
  virtual void visit(AndExpression &el) override;

  virtual void visit(AtomExpression &el) override;

  virtual void visit(Component &el) override;

  virtual void visit(DefinitionExpression &el) override;

  virtual void visit(Expression &el) override;

  virtual void visit(NotExpression &el) override;

  virtual void visit(OrExpression &el) override;

  virtual void visit(Program &el) override;

  virtual void visit(SetExpression &el) override;

  VisitorContext* visitorContext_;
public:
  ASTVisitor(VisitorContext* visitorContext);
};


#endif //META_ADL_PARSER_VISITOR_ASTVISITOR_H
