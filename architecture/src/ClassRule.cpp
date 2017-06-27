//
// Created by wilhelma on 1/17/17.
//

#include "../include/ClassRule.h"


#include <iostream>
#include <regex>

#include "ArchBuilder.h"
#include "Context.h"

namespace {

void addMethod(const pcv::dwarf::Routine* routine, pcv::Artifact_t* artifact)
{
  artifact->children.emplace_back(std::unique_ptr<pcv::Artifact_t> {
     new pcv::Artifact_t(routine->name, artifact)
  });
  auto newArtifact = artifact->children.back().get();

  for (auto variable : routine->locals) {
    newArtifact->entities.insert(variable);
  }
}

}

namespace pcv {

using pcv::entity::EntityType;
using pcv::entity::Class;

ClassRule::ClassRule(const std::string &artifactName,
                     const std::string &regex,
                     const std::string &fileRegex)
   : artifactName_(artifactName), rx_(regex), fileRx_(fileRegex) {}

const Class* ClassRule::getBaseClass(const Class *currentClass)
{
  const Class *cp = currentClass;

  for (auto bc : currentClass->baseClasses) {
    if (std::regex_match(bc->name, rx_) && std::regex_match(bc->file, fileRx_)) {
      cp = getBaseClass(bc);
      break;  // todo: consider multiple inheritance
    }
  }

  return cp;
};

void ClassRule::traverseHierarchy(const Class* cls, Artifact_t* artifact)
{
  artifact->children.emplace_back(std::unique_ptr<Artifact_t> {
     new pcv::Artifact_t(cls->name, artifact)
  });
  auto newArtifact = artifact->children.back().get();
  newArtifact->entity = cls;
  added.insert(cls);

  for (auto member : cls->members) {
    newArtifact->entities.insert(member);
  }

  for (auto method: cls->methods) {
    addMethod(method, newArtifact);
  }

  for (auto childClass : cls->inheritClasses) {
    if (std::regex_match(childClass->name, rx_) && std::regex_match(childClass->file, fileRx_)) {
      traverseHierarchy(childClass, newArtifact);
    }
  }
}

std::unique_ptr<ArchRule::artifacts_t>
ClassRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt)
{
  archSet.children.push_back(std::unique_ptr<Artifact_t> {new Artifact_t(artifactName_, &archSet)});
  auto newArtifact = archSet.children.back().get();

  // handle all classes that match the regexes
  for (auto &c : ctxt.classes) {
    if (std::regex_match(c->name, rx_) && std::regex_match(c->file, fileRx_)) {
      if (added.find(c.get()) == std::end(added)) {
        traverseHierarchy(getBaseClass(c.get()), newArtifact);
      }
    }
  }

  this->setArchSet(newArtifact);
  return nullptr;
}

std::unique_ptr<ArchRule::artifacts_t>
ClassRule::append(Artifact_t &as, const dwarf::Context &ctxt)
{
  return nullptr;
}

}  // namespace pcv
