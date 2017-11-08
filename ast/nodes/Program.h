//
// Created by Faris Cakaric on 28/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_PROGRAM_H
#define META_ADL_PARSER_VISITOR_PROGRAM_H


#include "Expression.h"
#include "ASTNode.h"

#include <memory>

using json11::Json;

class Program : public ASTNode {
    std::vector<std::unique_ptr<Expression>> expressions;

public:
    Program(std::vector<std::unique_ptr<Expression>> &expressions);

    const std::vector<std::unique_ptr<Expression>, std::allocator<std::unique_ptr<Expression>>> &
    getExpressions() const;

    virtual void accept(Visitor& v) override;
};


#endif //META_ADL_PARSER_VISITOR_PROGRAM_H
