//
// Created by Faris Cakaric on 28/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_PROGRAM_H
#define META_ADL_PARSER_VISITOR_PROGRAM_H


#include "DefinitionExpression.h"
#include "ASTNode.h"

#include <iostream>
#include <memory>

using json11::Json;

class Program : public ASTNode {
    std::vector<std::unique_ptr<DefinitionExpression>> expressions;

public:
    Program(std::vector<std::unique_ptr<DefinitionExpression>> &expressions);

    const std::vector<std::unique_ptr<DefinitionExpression>, std::allocator<std::unique_ptr<DefinitionExpression>>> &
    getExpressions() const;

    virtual void accept(Visitor& v) override;
};


#endif //META_ADL_PARSER_VISITOR_PROGRAM_H
