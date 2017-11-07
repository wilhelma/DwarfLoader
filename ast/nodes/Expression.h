//
// Created by Faris Cakaric on 16/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_EXPRESSION_H
#define META_ADL_PARSER_VISITOR_EXPRESSION_H

#include <json11.hpp>
#include <iostream>
#include <string>
#include "ASTNode.h"

using json11::Json;

class Expression : public ASTNode {
public:
    virtual void accept(Visitor& v) override;
};


#endif //META_ADL_PARSER_VISITOR_EXPRESSION_H
