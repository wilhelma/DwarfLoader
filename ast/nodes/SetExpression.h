//
// Created by Faris Cakaric on 28/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_SETTERM_H
#define META_ADL_PARSER_VISITOR_SETTERM_H

#include <vector>
#include <json11.hpp>

#include "Expression.h"
#include "Component.h"

using json11::Json;

class SetExpression : public virtual Expression {
    std::vector<std::unique_ptr<Component>> terms;
public:
    SetExpression(std::vector<std::unique_ptr<Component>> &terms);

    const std::vector<std::unique_ptr<Component>, std::allocator<std::unique_ptr<Component>>> &getTerms() const;

    virtual void accept(Visitor& v) override;
};


#endif //META_ADL_PARSER_VISITOR_SETTERM_H
