//
// Created by wilhelma on 1/10/17.
//

#include <NamespaceRule.h>
#include <ClassRule.h>
#include <FunctionRule.h>
#include "RegexRule.h"
#include "ArchBuilder.h"
#include "Context.h"
#include "entities/SoftwareEntity.h"

namespace pcv {

RegexRule::RegexRule(const std::string &artifactName, EntityType type, const std::string &rx)
    : artifactName_{artifactName}, type_{type}, rx_{rx} {}

std::unique_ptr<ArchRule::artifacts_t>
RegexRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) { return nullptr; }

void RegexRule::fillArtifact(const dwarf::Context &ctxt,
                             Artifact_t *toFill,
                             ArchRule::added_t& added) const
{


  // consider classes
  {
    std::unordered_set<const Class *> classes;
    for(auto &entities : ctxt.images) {
      for (auto& entity : entities.get()->entities) {
        if (entity->getEntityType() == EntityType::Class) {
          if (added.find(entity) == std::end(added)) {
            if (std::regex_match(getMatchString(*entity), rx_) && classes.find(static_cast<const Class*>(entity)) == std::end(classes)) {
              classes.insert(static_cast<const Class *>(entity));
            }
          }
        }
      }
    }

    ClassRule cRule;
    auto tmpAdded = cRule.apply(toFill, classes);
    for(auto cls : tmpAdded)
      added.insert(cls.first);
  }

  // consider functions
  {
    std::unordered_set<const Routine *> routines;
    for(auto &entities : ctxt.images) {
      for (auto &entity : entities.get()->entities) {
        if (entity->getEntityType() == EntityType::Routine) {
          if (added.find(entity) == std::end(added)) {
            if (std::regex_match(getMatchString(*entity), rx_)) {
              routines.insert(static_cast<const Routine *>(entity));
            }
          }
        }
      }
    }
    FunctionRule fRule;
    auto tmpAdded = fRule.apply(toFill, routines);
    added.insert(std::begin(tmpAdded), std::end(tmpAdded));
  }

  // consider variables
  {
    for(auto &entities : ctxt.images) {
      for (auto &entity : entities.get()->entities) {
        if (entity->getEntityType() == EntityType::Variable) {
          if (std::regex_match(getMatchString(*entity), rx_)) {
            if (added.find(entity) == std::end(added))
              toFill->entities.insert(entity);
          }
        }
      }
    }
  }
}

std::unique_ptr<ArchRule::artifacts_t>
RegexRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt)
{
  auto artifacts = std::unique_ptr<artifacts_t> { new artifacts_t };

  artifact_ = new Artifact_t(artifactName_, &archSet);
  artifact_->entity = nullptr;

  artifacts->emplace_back( artifact_ );

  added_t added;

  NamespaceRule nRule;
  for (auto& nmsp : ctxt.namespaces) {
    if (nmsp->parent && nmsp->parent == ctxt.emptyNamespace) {
      if (std::regex_match(getMatchString(*nmsp.get()), rx_)) {
        auto tmpAdded = nRule.apply(artifact_, *nmsp.get());
        added.insert(std::begin(tmpAdded), std::end(tmpAdded));
      }
    }
  }

  fillArtifact(ctxt, artifact_, added);

  return artifacts;
}

}  // namespace pcv
