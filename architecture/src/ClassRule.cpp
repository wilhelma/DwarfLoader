//
// Created by wilhelma on 1/17/17.
//

#include "../include/ClassRule.h"


#include <iostream>
#include <regex>

#include "ArchBuilder.h"
#include "Context.h"

namespace pcv {

  using pcv::entity::EntityType;
  using pcv::entity::Class;

  ClassRule::ClassRule(const std::string &artifactName,
                       const std::string &regex,
                       const std::string &fileRegex)
          : artifactName_(artifactName), rx_(regex), fileRx_(fileRegex) {}

  const Class *ClassRule::getBaseClass(const Class *currentClass) {
    const Class *cp = currentClass;

    for (auto bc : currentClass->baseClasses) {
      cp = getBaseClass(bc);
      break;  // todo: consider multiple inheritance
    }

    return cp;
  };

  void ClassRule::addMethod(const pcv::dwarf::Routine *routine, pcv::Artifact_t *artifact) {
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

  bool checkIfClassIsInherited(const Class *cls, const std::unordered_set<const Class *> &classes) {
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

  void ClassRule::traverseHierarchy(const Class *cls,
                                    Artifact_t *artifact,
                                    const std::unordered_set<const Class *> &classes)
  {
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


  std::unique_ptr<ArchRule::artifacts_t>
  ClassRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    artifact_ = new Artifact_t(artifactName_, &archSet);
    auto newArtifact = artifact_;
    std::unordered_set<const Class *> classes;

    // consider only matching classes
    for (auto &c : ctxt.classes) {
      if (std::regex_match(c->name, rx_) && std::regex_match(c->file, fileRx_)) {
        classes.insert(c.get());
      }
    }

    apply(*newArtifact, classes);
    return nullptr;
  }

  std::unique_ptr<ArchRule::artifacts_t>
  ClassRule::append(Artifact_t &as, const dwarf::Context &ctxt) {
    return nullptr;
  }

  ClassRule::added_t ClassRule::apply(Artifact_t &artifact,
                                      const std::unordered_set<const Class *> &classes) {
    for (auto cls : classes) {
      if (added.find(cls) == std::end(added)) {
        const auto *baseClass = getBaseClass(cls);
        traverseHierarchy(baseClass, &artifact, classes);
      }
    }

    return added;
  }

}  // namespace pcv
