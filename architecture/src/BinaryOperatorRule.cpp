//
// Created by Faris Cakaric on 27.08.17.
//

#include "BinaryOperatorRule.h"

namespace pcv {
  bool BinaryOperatorRule::isParent (const Namespace* nmspL, const Namespace* nmspR) {
    return nmspR->parent && (nmspR->parent == nmspL || isParent(nmspL, nmspR->parent));
  };

  void BinaryOperatorRule::getNamespacesInArtifact(Artifact_t &artifact,
                                                   std::vector<const Namespace *> &namespaces) {
    if (artifact.entity != nullptr && artifact.entity->getEntityType() == pcv::entity::EntityType::Namespace) {
      namespaces.push_back(static_cast<const Namespace*>(artifact.entity));
    }
    for(auto &child : artifact.children) {
      getNamespacesInArtifact(*child, namespaces);
    }
  }

  void BinaryOperatorRule::getClassesInArtifact(Artifact_t &artifact, std::unordered_set<const Class *> &classes) {
    if (artifact.entity != nullptr && artifact.entity->getEntityType() == pcv::entity::EntityType::Class) {
      classes.insert(static_cast<const Class*>(artifact.entity));
    }
    for(auto &child : artifact.children) {
      getClassesInArtifact(*child, classes);
    }
  }

  void BinaryOperatorRule::getRoutinesInArtifact(Artifact_t &artifact, std::unordered_set<const Routine *> &routines, ArchRule::added_t &added) {
    if (artifact.entity && artifact.entity->getEntityType() == pcv::entity::EntityType::Routine) {
      if(added.find(artifact.entity) == std::end(added)) {
        routines.insert(static_cast<const Routine*>(artifact.entity));
      }
    }
    for(auto &child : artifact.children) {
      getRoutinesInArtifact(*child, routines, added);
    }
  }

  void BinaryOperatorRule::getGlobalVariablesInArtifact(Artifact_t &artifact, std::unordered_set<const Variable *> &variables, ArchRule::added_t &added) {
    if (artifact.entity && artifact.entity->getEntityType() == pcv::entity::EntityType::Variable) {
      if(added.find(artifact.entity) == std::end(added)) {
        variables.insert(static_cast<const Variable*>(artifact.entity));
      }
    }
    for(auto &child : artifact.children) {
      getGlobalVariablesInArtifact(*child, variables, added);
    }
  }
}