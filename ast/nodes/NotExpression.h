//
// Created by Faris Cakaric on 28/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_NOTEXPRESSION_H
#define META_ADL_PARSER_VISITOR_NOTEXPRESSION_H
#include "Expression.h"

#include <json11.hpp>

using json11::Json;

class NotExpression : public virtual Expression {
    std::unique_ptr<Expression> operand;
public:
    NotExpression(std::unique_ptr<Expression> &operand);

    const std::unique_ptr<Expression> &getOperand() const;

    virtual void accept(Visitor& v) override;
};


#endif //META_ADL_PARSER_VISITOR_NOTEXPRESSION_H
