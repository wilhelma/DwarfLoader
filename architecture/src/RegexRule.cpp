//
// Created by wilhelma on 1/10/17.
//

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
  for (auto &entity : entities) {
    if ((entity->getEntityType() & type_) != EntityType::None) {
      if (std::regex_match(getMatchString(*entity), rx_))
        toFill->entities.insert(entity);
    }
  }
}

std::unique_ptr<ArchRule::artifacts_t>
RegexRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt)
{
  auto artifacts = std::unique_ptr<artifacts_t> { new artifacts_t };
  artifact_ = new Artifact_t(artifactName_, &archSet);

  artifacts->emplace_back( artifact_);
std::cout << ctxt.images.size();
  for (auto &image : ctxt.images) {
    fillArtifact(image->entities, artifact_);
  }

  return artifacts;
}


}  // namespace pcv
