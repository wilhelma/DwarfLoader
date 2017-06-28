//
// Created by wilhelma on 1/10/17.
//

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

void RegexRule::fillArtifact(const std::vector<SoftwareEntity*> &entities,
                             Artifact_t *toFill) const
{
  added_t added;

  // consider classes
  {
    std::unordered_set<const Class *> classes;
    for (auto &entity : entities) {
      if (entity->getEntityType() == EntityType::Class) {
        classes.insert(static_cast<const Class *>(entity));
      }
    }
    ClassRule cRule;
    added = cRule.apply(*toFill, classes);
  }

  // consider functions
  {
    std::unordered_set<const Routine *> routines;
    for (auto &entity : entities) {
      if (entity->getEntityType() == EntityType::Routine) {
        if (added.find(entity) == std::end(added)) {
          routines.insert(static_cast<const Routine *>(entity));
        }
      }
    }
    FunctionRule fRule;
    auto added2 = fRule.apply(*toFill, routines);
    added.insert(std::begin(added2), std::end(added2));
  }

  // fill rest
  {
    for (auto &entity : entities) {
      if ((entity->getEntityType() & type_) != EntityType::None) {
        if (std::regex_match(getMatchString(*entity), rx_)) {
          if (added.find(entity) == std::end(added))
            toFill->entities.insert(entity);
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

  artifacts->emplace_back( artifact_ );
  for (auto &image : ctxt.images) {
    fillArtifact(image->entities, artifact_);
  }

  return artifacts;
}

}  // namespace pcv
