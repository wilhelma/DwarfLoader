//
// Created by Faris Cakaric on 28/05/2017.
//

#include "AtomExpression.h"
#include "../visitor/Visitor.h"


void AtomExpression::accept(Visitor &v) {
    v.visit(*this);
}

AtomExpression::AtomExpression(const std::string &rule, const std::string &regex) : rule(rule), regex(regex) {}

const std::string &AtomExpression::getRule() const {
    return rule;
}

const std::string &AtomExpression::getRegex() const {
    return regex;
}
