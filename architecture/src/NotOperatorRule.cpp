//
// Created by Faris Cakaric on 19.07.17.
//

#include <Context.h>
#include <ClassRule.h>
#include <FunctionRule.h>
#include <NamespaceRule.h>
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

    Artifact_t newArtifact("newArtifact", nullptr);
    Artifact_t nmspArtifact("newArtifact1", nullptr);

    //find namespaces, classes, functions and variables mapped in the operand
    std::unordered_set<const Namespace *> namespacesInOperand;
    getNamespacesInArtifact(*operand_, namespacesInOperand);

    std::unordered_set<const Class *> classesInOperand;
    getClassesInArtifact(*operand_, classesInOperand);

    std::unordered_set<const Routine *> routinesInOperand;
    getRoutinesInArtifact(*operand_, routinesInOperand, added);

    std::unordered_set<const Variable *> variablesInOperand;
    getGlobalVariablesInArtifact(*operand_, variablesInOperand, added);

    //find namespaces, classes, functions and variables which are not in the operand
    std::unordered_set<const Namespace *> namespaces;
    std::unordered_set<const Class *> classes;
    std::unordered_set<const Routine *> routines;
    std::unordered_set<const Variable *> variables;

    for (auto &nmsp : ctxt.namespaces) {
      if (namespacesInOperand.find(nmsp.get()) == std::end(namespacesInOperand)) {
        namespaces.insert(nmsp.get());
      }
    }

    std::vector<const Namespace *> namespacesVector;
    std::copy(namespaces.begin(), namespaces.end(), std::inserter(namespacesVector, namespacesVector.end()));
    auto nmsp1 = namespacesVector.begin();
    while(nmsp1 != std::end(namespacesVector)) {
      if((*nmsp1)->name != "") {
        auto nmspBefore = namespacesVector.begin();
        while(nmspBefore != nmsp1) {
          if(std::find((*nmsp1)->children.begin(), (*nmsp1)->children.end(), *nmspBefore) != (*nmsp1)->children.end()) {
            std::swap(*nmsp1, *nmspBefore);
            break;
          }
          ++nmspBefore;
        }
      }

      ++nmsp1;
    }



    NamespaceRule namespaceRule;
    std::unordered_map<const Namespace *, Artifact_t *> namespacesAdded = namespaceRule.apply(&nmspArtifact,
                                                                                              namespacesVector);

    for (auto &cls : ctxt.classes) {
      if (classesInOperand.find(cls.get()) == std::end(classesInOperand)) {
        classes.insert(cls.get());
      }
    }

    ClassRule cRule;
    std::unordered_map<const SoftwareEntity *, Artifact_t *> addedCls = cRule.apply(&newArtifact, classes, false);
    for(auto cls : addedCls)
      added.insert(cls.first);

    for (auto &routine : ctxt.routines) {
      if (routinesInOperand.find(routine.get()) == std::end(routinesInOperand) &&
          added.find(routine.get()) == std::end(added)) {
        routines.insert(routine.get());
      }
    }

    FunctionRule fRule;
    auto fAdded = fRule.apply(&newArtifact, routines);
    added.insert(std::begin(fAdded), std::end(fAdded));

    for (auto &var : ctxt.variables) {
      if (variablesInOperand.find(var.get()) == std::end(variablesInOperand) &&
          added.find(var.get()) == std::end(added)) {
        variables.insert(var.get());
      }
    }

    VariableRule vRule;
    vRule.apply(newArtifact, variables);

    auto child = std::begin(newArtifact.children);
    while (child != std::end(newArtifact.children)) {
      bool hasMapping = false;
      if ((*child)->entity->nmsp->name != "") {
        for (auto &nmsp : nmspArtifact.children) {
          if (nmsp.get()->entity == (*child)->entity->nmsp) {
            (*child)->parent = nmsp.get();
            nmsp->children.push_back(std::move(*child));
            hasMapping = true;
            break;
          }
        }
      }
      if (!hasMapping) {
        (*child)->parent = artifact_;
        artifact_->children.push_back(std::move(*child));
      }
      ++child;
    }


    auto nmsp = std::begin(nmspArtifact.children);
    while (nmsp != std::end(nmspArtifact.children)) {
      if ((*nmsp).get()->name != "empty") {
        (*nmsp)->parent = artifact_;
        artifact_->children.push_back(std::move(*nmsp));
      }
      ++nmsp;
    }


    return artifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> NotOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {
    return nullptr;
  }
}