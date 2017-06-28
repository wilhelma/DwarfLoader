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

const Class* ClassRule::getBaseClass(const Class *currentClass,
                                     const std::unordered_set<const Class*>& classes)
{
  const Class *cp = currentClass;

  for (auto bc : currentClass->baseClasses) {
    if (classes.find(bc) != std::end(classes)) {
      cp = getBaseClass(bc, classes);
      break;  // todo: consider multiple inheritance
    }
  }

  return cp;
};

void ClassRule::addMethod(const pcv::dwarf::Routine* routine, pcv::Artifact_t* artifact)
{
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

void ClassRule::traverseHierarchy(const Class* cls,
                                  Artifact_t* artifact,
                                  const std::unordered_set<const Class*>& classes)
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
    if (classes.find(childClass) != std::end(classes)) {
      traverseHierarchy(childClass, newArtifact, classes);
    }
  }
}

std::unique_ptr<ArchRule::artifacts_t>
ClassRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt)
{
  archSet.children.push_back(std::unique_ptr<Artifact_t> {new Artifact_t(artifactName_, &archSet)});
  auto newArtifact = archSet.children.back().get();
  std::unordered_set<const Class *> classes;

  // consider only matching classes
  for (auto &c : ctxt.classes) {
    if (std::regex_match(c->name, rx_) && std::regex_match(c->file, fileRx_)) {
      classes.insert(c.get());
    }
  }

  apply(*newArtifact, classes);
  this->setArchSet(newArtifact);
  return nullptr;
}

std::unique_ptr<ArchRule::artifacts_t>
ClassRule::append(Artifact_t &as, const dwarf::Context &ctxt)
{
  return nullptr;
}

ClassRule::added_t ClassRule::apply(Artifact_t &artifact,
                                    const std::unordered_set<const Class *> &classes)
{
  for (auto cls : classes) {
    if (added.find(cls) == std::end(added)) {
      traverseHierarchy(getBaseClass(cls, classes), &artifact, classes);
    }
  }

  return added;
}

}  // namespace pcv
