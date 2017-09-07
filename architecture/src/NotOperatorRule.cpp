//
// Created by Faris Cakaric on 19.07.17.
//

#include <Context.h>
#include <ClassRule.h>
#include <FunctionRule.h>
#include <VariableRule.h>
#include "NotOperatorRule.h"

namespace pcv {
  NotOperatorRule::NotOperatorRule(const std::string &artifactName_, Artifact_t *operand_) : artifactName_(
          artifactName_), operand_(operand_) {}

  std::unique_ptr<ArchRule::artifacts_t> NotOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};
    artifact_ = new Artifact_t(artifactName_, &archSet);
    artifact_->entity = nullptr;

    ArchRule::added_t added;

    //find namespaces, classes, functions and variables mapped in the operand

    std::unordered_set<const Class *> classesInOperand;
    getClassesInArtifact(*operand_, classesInOperand);

    std::unordered_set<const Routine *> routinesInOperand;
    getRoutinesInArtifact(*operand_, routinesInOperand, added);

    std::unordered_set<const Variable *> variablesInOperand;
    getGlobalVariablesInArtifact(*operand_, variablesInOperand, added);

    //find namespaces, classes, functions and variables which are not in the operand
    std::unordered_set<const Class *> classes;
    std::unordered_set<const Routine *> routines;
    std::unordered_set<const Variable *> variables;

    for(auto &cls : ctxt.classes) {
      if(classesInOperand.find(cls.get()) == std::end(classesInOperand)) {
        classes.insert(cls.get());
      }
    }

    ClassRule cRule;
    added = cRule.apply(*artifact_, classes, false);

    for(auto &routine : ctxt.routines) {
      if(routinesInOperand.find(routine.get()) == std::end(routinesInOperand) && added.find(routine.get()) == std::end(added)) {
        routines.insert(routine.get());
      }
    }

    FunctionRule fRule;
    auto fAdded = fRule.apply(*artifact_, routines);
    added.insert(std::begin(fAdded), std::end(fAdded));

    for(auto &var : ctxt.variables) {
      if(variablesInOperand.find(var.get()) == std::end(variablesInOperand) && added.find(var.get()) == std::end(added)) {
        variables.insert(var.get());
      }
    }

    VariableRule vRule;
    vRule.apply(*artifact_, variables);

    return artifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> NotOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}
}