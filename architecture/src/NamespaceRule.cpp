//
// Created by wilhelma on 1/10/17.
//

#include <ClassRule.h>
#include <entities/SoftwareEntity.h>
#include <FunctionRule.h>
#include <VariableRule.h>
#include "NamespaceRule.h"
#include "ArchBuilder.h"
#include "Context.h"
#include "entities/SoftwareEntity.h"

namespace pcv {

  using entity::Namespace;
  using entity::Variable;

  NamespaceRule::NamespaceRule(const std::string &artifactName, const std::string &regexString) : artifactName_(
          artifactName), rx_(regexString) {}

  std::unique_ptr<ArchRule::artifacts_t>
  NamespaceRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) { return nullptr; }

  std::unique_ptr<ArchRule::artifacts_t>
  NamespaceRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    std::unordered_set<const Namespace *> namespaces;

    artifact_ = new Artifact_t(artifactName_, &archSet);
    artifact_->entity = nullptr;

    for (auto &nmsp : ctxt.namespaces) {
      namespaces.insert(nmsp.get());
    }

    apply(*artifact_, namespaces, true);

    return nullptr;
  }

  std::unordered_map<const Namespace *, Artifact_t *> NamespaceRule::apply(Artifact_t &artifact,
                                      const std::unordered_set<const Namespace *> &namespaces, bool abstraction) {
    std::unordered_map<const Namespace *, Artifact_t *> addedArtifacts;
    for (auto nmsp : namespaces) {
      if(std::regex_match(nmsp->name, rx_)) {
        if (nmsp->parent && addedArtifacts.find(nmsp->parent) != end(addedArtifacts))
          artifact_ = addedArtifacts[nmsp->parent];

        std::string name = nmsp->name.empty() ? "empty" : nmsp->name;
        artifact_->children.push_back(
                std::unique_ptr<Artifact_t>{new Artifact_t(name, artifact_)}
        );

        auto parent = artifact.children.back().get();
        parent->entity = nmsp;

        addedArtifacts[nmsp] = artifact_->children.back().get();
        if(abstraction) {
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
            added = cRule.apply(*parent, classes);
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
            auto fAdded = fRule.apply(*parent, routines);
            added.insert(begin(fAdded), end(fAdded));
          }

          // consider global variables
          {
            std::unordered_set<const Variable *> variables;
            for (auto entity : nmsp->entities) {
              if (entity->getEntityType() == pcv::entity::EntityType::Variable) {
                if (added.find(entity) == std::end(added)) {
                  variables.insert(static_cast<const Variable *>(entity));
                }
              }
            }
            VariableRule vRule;
            vRule.apply(*parent, variables);
          }
        }
      }
    }

    return addedArtifacts;
  }

}  // namespace pcv
