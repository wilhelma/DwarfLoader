//
// Created by Faris Cakaric on 17.06.17.
//

#include "../include/VariableRule.h"

#include <algorithm>
#include <regex>

#include "ArchBuilder.h"
#include "Context.h"

namespace pcv {

  VariableRule::VariableRule(const std::string &artifactName, const std::string &regexString) : artifactName_(
          artifactName), rx_(regexString) {}

  std::unique_ptr<ArchRule::artifacts_t> VariableRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};

    archSet.children.push_back(std::unique_ptr<Artifact_t>{ new Artifact_t(artifactName_, &archSet)});

    Artifact_t *parent = archSet.children.back().get();

    for (auto &variable : ctxt.variables) {
      if (std::regex_match(variable->name, rx_)) {
        parent->children.emplace_back(new Artifact_t(variable->name, parent));
        artifacts->emplace_back(parent->children.back().get());
      }
    }

    this->setArchSet(archSet.children.back().get());
    return artifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> VariableRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}
}