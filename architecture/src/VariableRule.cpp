//
// Created by Faris Cakaric on 17.06.17.
//

#include "../include/VariableRule.h"

#include <algorithm>
#include <regex>

#include "ArchBuilder.h"
#include "Context.h"

namespace pcv {

  using entity::Variable;

  VariableRule::VariableRule(const std::string &artifactName, const std::string &regexString) : artifactName_(
          artifactName), rx_(regexString) {}

  std::unique_ptr<ArchRule::artifacts_t> VariableRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    artifact_ = new Artifact_t(artifactName_, &archSet);
    artifact_->entity = nullptr;

    std::unordered_set<const Variable*> variables;
    for (auto &variable : ctxt.variables) {
      if (std::regex_match(variable->name, rx_)) {
        variables.insert(variable.get());
      }
    }

    apply(*artifact_, variables);

    return nullptr;
  }

  ArchRule::added_t VariableRule::apply(Artifact_t &artifact, const std::unordered_set<const Variable *> &variables) {
    for (auto variable : variables) {
      artifact.children.emplace_back(new Artifact_t(variable->name, &artifact));
      auto newVariable = artifact.children.back().get();
      newVariable->entity = variable;
      added_.insert(variable);
    }

    return added_;
  }

  std::unique_ptr<ArchRule::artifacts_t> VariableRule::append(Artifact_t &archSet, const dwarf::Context &ctxt)
  {
    return nullptr;
  }

}