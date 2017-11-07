//
// Created by Faris Cakaric on 28/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_ANDEXPRESSION_H
#define META_ADL_PARSER_VISITOR_ANDEXPRESSION_H

#include "Expression.h"

class AndExpression : public virtual Expression {
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

public:
    AndExpression(std::unique_ptr<Expression> &left, std::unique_ptr<Expression> &right);

    const std::unique_ptr<Expression> &getLeft() const;

    const std::unique_ptr<Expression> &getRight() const;

    virtual void accept(Visitor& v) override;

};


#endif //META_ADL_PARSER_VISITOR_ANDEXPRESSION_H
