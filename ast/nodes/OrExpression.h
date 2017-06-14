//
// Created by Faris Cakaric on 28/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_OREXPRESSION_H
#define META_ADL_PARSER_VISITOR_OREXPRESSION_H

#include "Expression.h"

class OrExpression : public virtual Expression {
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

public:
    OrExpression(OrExpression const &) = delete;

    OrExpression &operator=(OrExpression const &) = delete;

    OrExpression(std::unique_ptr<Expression> &left, std::unique_ptr<Expression> &right);

    const std::unique_ptr<Expression> &getLeft() const;

    const std::unique_ptr<Expression> &getRight() const;

    virtual void accept(Visitor& v) override;
};


#endif //META_ADL_PARSER_VISITOR_OREXPRESSION_H
