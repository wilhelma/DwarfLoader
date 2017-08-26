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
  artifact_ = new Artifact_t(artifactName_, &archSet);

  std::unordered_set<const Routine*> routines;
  for (auto &f : ctxt.routines) {
    //std::cout << f->name << std::endl;
    if (std::regex_match(f->name, rx_)) {
      routines.insert(f.get());
    }
  }

  apply(*artifact_, routines);

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
    auto newRoutine = artifact.children.back().get();
    newRoutine->entity = routine;
    added_.insert(routine);

    for (auto variable : routine->locals) {
      newRoutine->children.emplace_back(new Artifact_t(variable->name, newRoutine));
      newRoutine->children.back().get()->entity = variable;
      added_.insert(variable);
    }
  }

  return added_;
}

}  // namespace pcv
