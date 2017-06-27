//
// Created by Faris Cakaric on 15.06.17.
//

#include <iostream>
#include "FunctionRule.h"
#include "ArchBuilder.h"
#include "Context.h"

namespace pcv {
  FunctionRule::FunctionRule(const std::string &artifactName, const std::string &regexString) : artifactName_(
          artifactName), rx_(regexString) {}

  std::unique_ptr<ArchRule::artifacts_t>
  FunctionRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};
    artifact_ = new Artifact_t(artifactName_, &archSet);

    for (auto &f : ctxt.routines) {
      if (std::regex_match(f->name, rx_)) {
        artifact_->children.emplace_back(new Artifact_t(f->name, artifact_));

        auto &children = artifact_->children.back();
        for (auto &locals : f->locals) {
          children->entities.insert(locals);
        }

        artifacts->emplace_back(artifact_->children.back().get());
      }
    }

    return artifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> FunctionRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}

}
