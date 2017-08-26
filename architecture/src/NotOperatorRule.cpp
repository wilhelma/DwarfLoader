//
// Created by Faris Cakaric on 19.07.17.
//

#include <Context.h>
#include <ClassRule.h>
#include "NotOperatorRule.h"

namespace pcv {
  NotOperatorRule::NotOperatorRule(const std::string &artifactName_, Artifact_t *operand_) : artifactName_(
          artifactName_), operand_(operand_) {}

  bool NotOperatorRule::checkIfOperandContainsClass(const Class *cls, Artifact_t *artifact) {
    if (dynamic_cast<const Class *>(artifact->entity) == cls) {
      return true;
    } else {
      for (auto &childArtifact : artifact->children) {
        if (checkIfOperandContainsClass(cls, childArtifact.get())) return true;
      }
    }
    return false;
  }

  const Class* NotOperatorRule::getBaseClass(const Class *currentClass) {
    const Class *cp = currentClass;

    for (auto bc : currentClass->baseClasses) {
      cp = getBaseClass(bc);
      break;  // todo: consider multiple inheritance
    }

    return cp;
  }

  void NotOperatorRule::addMethod(const Routine *routine, Artifact_t *artifact) {
    artifact->children.emplace_back(std::unique_ptr<pcv::Artifact_t> {
            new pcv::Artifact_t(routine->name, artifact)
    });
    auto newArtifact = artifact->children.back().get();
    newArtifact->entity = routine;
    added.insert(routine);

    for (auto variable : routine->locals) {
      newArtifact->entities.insert(variable);
      added.insert(variable);
    }
  }

  bool NotOperatorRule::checkIfClassIsInherited(const Class *cls, const std::unordered_set<const Class *> &classes) {
    if (classes.find(cls) != std::end(classes)) {
      return true;
    } else {
      for (auto childClass : cls->inheritClasses) {
        if (checkIfClassIsInherited(childClass, classes))
          return true;
      }
    }
    return false;
  }

  void NotOperatorRule::traverseHierarchy(const Class *cls,
                                    Artifact_t *artifact,
                                    const std::unordered_set<const Class *> &classes) {
    artifact->children.emplace_back(std::unique_ptr<Artifact_t> {
            new pcv::Artifact_t(cls->name, artifact)
    });
    auto newArtifact = artifact->children.back().get();
    newArtifact->entity = cls;
    added.insert(cls);

    for (auto member : cls->members) {
      newArtifact->entities.insert(member);
      added.insert(member);
    }

    for (auto method: cls->methods) {
      addMethod(method, newArtifact);
    }

    for (auto childClass : cls->inheritClasses) {
      if (checkIfClassIsInherited(childClass, classes)) {
        traverseHierarchy(childClass, newArtifact, classes);
      }
    }
  }

  std::unique_ptr<ArchRule::artifacts_t> NotOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};
    artifact_ = new Artifact_t(artifactName_, &archSet);

    std::unordered_set<const Class *> classes;

    for (auto &cls : ctxt.classes) {
      if (!checkIfOperandContainsClass(cls.get(), operand_)) {
        classes.insert(cls.get());
      }
    }

    for (auto cls : classes) {
      if (added.find(cls) == std::end(added)) {
        const Class *baseClass = getBaseClass(cls);
        traverseHierarchy(baseClass, artifact_, classes);
      }
    }

    return artifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> NotOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}
}