//
// Created by wilhelma on 1/17/17.
//

#include "../include/ClassRule.h"


#include <iostream>
#include <regex>

#include "TemplateHelper.h"
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
    added[routine] = newArtifact;

    for (auto variable : routine->locals) {
      newArtifact->children.emplace_back(new Artifact_t(variable->name, newArtifact));
      auto varArtifact = newArtifact->children.back().get();
      varArtifact->entity = variable;
      added[variable] = varArtifact;
    }
  }

  bool checkIfClassIsInherited(const Class *cls, const std::vector<const Class *> &classes) {
    if (std::find(classes.begin(), classes.end(), cls) != std::end(classes)) {
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
                                    const std::vector<const Class *> &classes,
                                    bool useAllClassesFromCtxt) {
    artifact->children.emplace_back(std::unique_ptr<Artifact_t> {
            new pcv::Artifact_t(cls->name, artifact)
    });
    auto newArtifact = artifact->children.back().get();
    newArtifact->entity = cls;
    added[cls] = newArtifact;

    for (auto member : cls->members) {
      newArtifact->children.emplace_back(new Artifact_t(member->name, newArtifact));
      auto clsArtifact = newArtifact->children.back().get();
      clsArtifact->entity = member;
      added[member] = clsArtifact;
    }

    { // add methods
      TemplateHelper helper(newArtifact);
      for (auto method: cls->methods) {
        auto tmpArtifact = helper.processRoutine(method);
        addMethod(method, tmpArtifact);
      }
    }

    for (auto nested : cls->nestedClasses) {
      traverseHierarchy(nested, newArtifact, classes, useAllClassesFromCtxt);
    }

    for (auto childClass : cls->inheritClasses) {
      if (checkIfClassIsInherited(childClass, classes) && added.find(childClass) == std::end(added) &&
          (!useAllClassesFromCtxt ? (std::find(classes.begin(), classes.end(), childClass) != std::end(classes)) : true)) {
        traverseHierarchy(childClass, artifact, classes);
      }
    }
  }


  std::unique_ptr<ArchRule::artifacts_t>
  ClassRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    artifact_ = new Artifact_t(artifactName_, &archSet);
    artifact_->entity = nullptr;
    auto newArtifact = artifact_;
    std::unordered_set<const Class *> classes;

    // consider only matching classes
    for (auto &c : ctxt.classes) {
      if (std::regex_match(c->name, rx_) && std::regex_match(c->file, fileRx_)) {
        classes.insert(c.get());
      }
    }

    apply(newArtifact, classes);
    return nullptr;
  }

  std::unique_ptr<ArchRule::artifacts_t>
  ClassRule::append(Artifact_t &as, const dwarf::Context &ctxt) {
    return nullptr;
  }

  std::unordered_map<const SoftwareEntity *, Artifact_t *> ClassRule::apply(Artifact_t *artifact,
                                                                            const std::unordered_set<const Class *> &classes,
                                                                            bool useAllClassesFromCtxt) {
    std::vector<const Class *> classesVector;
    std::copy(classes.begin(), classes.end(), std::inserter(classesVector, classesVector.end()));
    auto nmsp1 = classesVector.begin();
    while(nmsp1 != std::end(classesVector)) {
        auto nmspBefore = classesVector.begin();
        while(nmspBefore != nmsp1) {
          if(std::find((*nmsp1)->nestedClasses.begin(), (*nmsp1)->nestedClasses.end(), *nmspBefore) != (*nmsp1)->nestedClasses.end()) {
            std::swap(*nmsp1, *nmspBefore);
            break;
          }
          ++nmspBefore;
        }
      ++nmsp1;
    }

    for (auto cls : classesVector) {
      if (added.find(cls) == std::end(added)) {
        const Class *baseClass = getBaseClass(cls);
        if (useAllClassesFromCtxt)
          traverseHierarchy(baseClass, artifact, classesVector, useAllClassesFromCtxt);
        else {
          if (classes.find(baseClass) == std::end(classes))
            traverseHierarchy(cls, artifact, classesVector, useAllClassesFromCtxt);
          else
            traverseHierarchy(baseClass, artifact, classesVector, useAllClassesFromCtxt);
        }

      }
    }

    return added;

  }

}  // namespace pcv
