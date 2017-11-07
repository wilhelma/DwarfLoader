//
// Created by Faris Cakaric on 25.05.17.
//

#ifndef META_ADL_PARSER_VISITOR_VISITOR_H
#define META_ADL_PARSER_VISITOR_VISITOR_H


#include "../nodes/AndExpression.h"
#include "../nodes/OrExpression.h"
#include "../nodes/AssignmentExpression.h"
#include "../nodes/AtomExpression.h"
#include "../nodes/Artifact.h"
#include "../nodes/DefinitionExpression.h"
#include "../nodes/Expression.h"
#include "../nodes/NotExpression.h"
#include "../nodes/Program.h"
#include "../nodes/SetExpression.h"

class Visitor {
public:
    virtual void visit(AndExpression& el) = 0;
    virtual void visit(AssignmentExpression& el) = 0;
    virtual void visit(AtomExpression& el) = 0;
    virtual void visit(Artifact& el) = 0;
    virtual void visit(DefinitionExpression& el) = 0;
    virtual void visit(Expression& el) = 0;
    virtual void visit(NotExpression& el) = 0;
    virtual void visit(OrExpression& el) = 0;
    virtual void visit(Program& el) = 0;
    virtual void visit(SetExpression& el) = 0;
};


#endif //META_ADL_PARSER_VISITOR_VISITOR_H
