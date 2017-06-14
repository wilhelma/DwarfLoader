//
// Created by Faris Cakaric on 28/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_DEFINITIONEXPRESSION_H
#define META_ADL_PARSER_VISITOR_DEFINITIONEXPRESSION_H


#include "Expression.h"
#include "Component.h"

#include <json11.hpp>

using json11::Json;

class DefinitionExpression : public Expression {
    std::unique_ptr<Component> component;
    std::unique_ptr<Expression> expression;
public:
    DefinitionExpression(std::unique_ptr<Component> &component, std::unique_ptr<Expression> &expression);

    const std::unique_ptr<Component> &getComponent() const;

    const std::unique_ptr<Expression> &getExpression() const;

    virtual void accept(Visitor& v) override;
};


#endif //META_ADL_PARSER_VISITOR_DEFINITIONEXPRESSION_H
