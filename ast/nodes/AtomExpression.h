//
// Created by Faris Cakaric on 28/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_ATOMEXPRESSION_H
#define META_ADL_PARSER_VISITOR_ATOMEXPRESSION_H


#include <string>

#include "Expression.h"

#include <json11.hpp>

using json11::Json;

class AtomExpression : public virtual Expression {
    std::string rule;
    std::string regex;

public:
    AtomExpression(const std::string &rule, const std::string &regex);

    const std::string &getRule() const;

    const std::string &getRegex() const;

    virtual void accept(Visitor& v) override;
};


#endif //META_ADL_PARSER_VISITOR_ATOMEXPRESSION_H
