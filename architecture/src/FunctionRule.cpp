//
// Created by Faris Cakaric on 15.06.17.
//

#include <iostream>
#include "FunctionRule.h"
#include "Context.h"

namespace pcv {
FunctionRule::FunctionRule(const std::string &artifactName,
                           const std::string &regexString)
   : artifactName_(artifactName), rx_(regexString) {}

std::unique_ptr<ArchRule::artifacts_t>
FunctionRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt)
{
  archSet.children.push_back(std::unique_ptr<Artifact_t>{new Artifact_t(artifactName_, &archSet)});
  auto newArtifact = archSet.children.back().get();

  std::unordered_set<const Routine*> routines;
  for (auto &f : ctxt.routines) {
    if (std::regex_match(f->name, rx_)) {
      routines.insert(f.get());
    }
  }

  apply(*newArtifact, routines);

  this->setArchSet(newArtifact);
  return nullptr;
}

std::unique_ptr<ArchRule::artifacts_t> FunctionRule::append(Artifact_t &archSet,
                                                            const dwarf::Context &ctxt)
{
  return nullptr;
}

ArchRule::added_t FunctionRule::apply(Artifact_t &artifact,
                                      const std::unordered_set<const Routine *> &routines)
{
  for (auto routine : routines) {
    artifact.children.emplace_back(new Artifact_t(routine->name, &artifact));
    auto& newArtifact = artifact.children.back();
    added_.insert(routine);

    for (auto variable : routine->locals) {
      newArtifact->entities.insert(variable);
      added_.insert(variable);
    }
  }

  return added_;
}

}  // namespace pcv
