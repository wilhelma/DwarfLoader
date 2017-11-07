//
// Created by Faris Cakaric on 15.06.17.
//

#include <iostream>

#include "TemplateHelper.h"
#include "Context.h"

#include "FunctionRule.h"

namespace pcv {
FunctionRule::FunctionRule(const std::string &artifactName,
                           const std::string &regexString)
   : artifactName_(artifactName), rx_(regexString) {}

std::unique_ptr<ArchRule::artifacts_t>
FunctionRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt)
{
  artifact_ = new Artifact_t(artifactName_, &archSet);
  artifact_->entity = nullptr;

  std::unordered_set<const Routine*> routines;
  for (auto &f : ctxt.routines) {
    if (std::regex_match(f->name, rx_)) {
      routines.insert(f.get());
    }
  }

  apply(artifact_, routines);

  return nullptr;
}

std::unique_ptr<ArchRule::artifacts_t> FunctionRule::append(Artifact_t &archSet,
                                                            const dwarf::Context &ctxt)
{
  return nullptr;
}

ArchRule::added_t FunctionRule::apply(Artifact_t* artifact,
                                      const std::unordered_set<const Routine *> &routines)
{
  TemplateHelper helper(artifact);
  for (auto routine : routines) {
    auto tmpArtifact = helper.processRoutine(routine);
    tmpArtifact->children.emplace_back(new Artifact_t(routine->name, tmpArtifact));
    auto newRoutine = tmpArtifact->children.back().get();
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
