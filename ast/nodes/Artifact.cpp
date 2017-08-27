//
// Created by Faris Cakaric on 28/05/2017.
//

#include "Artifact.h"
#include "../visitor/Visitor.h"

const std::string& Artifact::getName() const {
    return name;
}

void Artifact::accept(Visitor &v) {
    v.visit(*this);
}

Artifact::Artifact(const std::string &name) : name(name) {}
