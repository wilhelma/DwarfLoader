//
// Created by Faris Cakaric on 28/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_COMPONENT_H
#define META_ADL_PARSER_VISITOR_COMPONENT_H

#include <json11.hpp>
#include <string>
#include <iostream>

#include "Expression.h"

using json11::Json;

class Component : public virtual Expression {
    std::string name;
public:
    Component(const std::string &name);

    const std::string &getName() const;

    virtual void accept(Visitor& v) override;
};


#endif //META_ADL_PARSER_VISITOR_COMPONENT_H
