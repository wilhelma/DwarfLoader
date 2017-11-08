//
// Created by Faris Cakaric on 16/05/2017.
//

#include "Expression.h"
#include "../visitor/Visitor.h"

void Expression::accept(Visitor &v) {
    v.visit(*this);
}