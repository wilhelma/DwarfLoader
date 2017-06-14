//
// Created by Faris Cakaric on 28/05/2017.
//

#include "Component.h"
#include "../visitor/Visitor.h"

const std::string& Component::getName() const {
    return name;
}

void Component::accept(Visitor &v) {
    v.visit(*this);
}

Component::Component(const std::string &name) : name(name) {}
