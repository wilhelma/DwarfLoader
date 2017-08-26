//
// Created by wilhelma on 1/10/17.
//

#include <ClassRule.h>
#include <entities/SoftwareEntity.h>
#include <FunctionRule.h>
#include "NamespaceRule.h"
#include "ArchBuilder.h"
#include "Context.h"
#include "entities/SoftwareEntity.h"

namespace pcv {

  using entity::Namespace;

  NamespaceRule::NamespaceRule(const std::string &artifactName, const std::string &regexString) : artifactName_(
          artifactName), rx_(regexString) {}

  std::unique_ptr<ArchRule::artifacts_t>
  NamespaceRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) { return nullptr; }

  std::unique_ptr<ArchRule::artifacts_t>
  NamespaceRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    static std::unordered_map<Namespace *, Artifact_t *> addedArtifacts;

    artifact_ = new Artifact_t(artifactName_, &archSet);

    for (auto &nmsp : ctxt.namespaces) {
      if(std::regex_match(nmsp->name, rx_)) {
        if (nmsp->parent && addedArtifacts.find(nmsp->parent) != end(addedArtifacts))
          artifact_ = addedArtifacts[nmsp->parent];

        std::string name = nmsp->name.empty() ? "empty" : nmsp->name;
        artifact_->children.push_back(
                std::unique_ptr<Artifact_t>{new Artifact_t(name, artifact_)}
        );

       // artifact_->children.back().get()->entity = nmsp.get();

        addedArtifacts[nmsp.get()] = artifact_->children.back().get();

        ArchRule::added_t added;
        // consider classes
        {
          std::unordered_set<const Class *> classes;
          for (auto entity : nmsp->entities) {
            if (entity->getEntityType() == pcv::entity::EntityType::Class) {
              classes.insert(static_cast<const Class *>(entity));
            }
          }
          ClassRule cRule;
          added = cRule.apply(*artifact_, classes);
        }

        // consider routines
        {
          std::unordered_set<const Routine *> routines;
          for (auto entity : nmsp->entities) {
            if (entity->getEntityType() == pcv::entity::EntityType::Routine) {
              if (added.find(entity) == std::end(added)) {
                routines.insert(static_cast<const Routine *>(entity));
              }
            }
          }
          FunctionRule fRule;
          auto fAdded = fRule.apply(*artifact_, routines);
          added.insert(begin(fAdded), end(fAdded));
        }

        auto &children = artifact_->children.back();
        for (auto entity : nmsp->entities) {
          if (added.find(entity) == end(added)) {
            children->entities.insert(entity);
          }
        }
      }
    }

    return nullptr;
  }

}  // namespace pcv
