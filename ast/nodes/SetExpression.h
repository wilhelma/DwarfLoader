//
// Created by Faris Cakaric on 28/05/2017.
//

#ifndef META_ADL_PARSER_VISITOR_SETTERM_H
#define META_ADL_PARSER_VISITOR_SETTERM_H

#include <vector>
#include <json11.hpp>

#include "Expression.h"
#include "Artifact.h"

using json11::Json;

class SetExpression : public virtual Expression {
    std::vector<std::unique_ptr<Artifact>> terms;
public:
    SetExpression(std::vector<std::unique_ptr<Artifact>> &terms);

    const std::vector<std::unique_ptr<Artifact>, std::allocator<std::unique_ptr<Artifact>>> &getTerms() const;

    virtual void accept(Visitor& v) override;
};


#endif //META_ADL_PARSER_VISITOR_SETTERM_H
