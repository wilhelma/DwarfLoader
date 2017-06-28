//
// Created by Faris Cakaric on 18.06.17.
//

#include <iostream>
#include <ArchBuilder.h>
#include "SetOperatorRule.h"

namespace pcv {
  Artifact_t* SetOperatorRule::copyChildren(Artifact_t &parent, Artifact_t &artifact) {
    for(auto &child : artifact.children) {
      parent.children.push_back(std::unique_ptr<Artifact_t>{new Artifact_t(child->name, &parent)});

      for(auto &entity : child->entities)
        parent.children.back().get()->entities.insert(entity);
      copyChildren(*(parent.children.back().get()), *child);
    }
    return &parent;
  }

  std::unique_ptr<ArchRule::artifacts_t> SetOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto newArtifacts = std::unique_ptr<artifacts_t> {new artifacts_t};
    artifact_ = new Artifact_t(artifactName_, &archSet);

    for(auto &memberArtifact : artifacts) {
      artifact_->children.emplace_back(new Artifact_t(memberArtifact->name, artifact_));
      Artifact_t* parent = artifact_->children.back().get();
      for(auto &child : memberArtifact->children) {
        parent->children.emplace_back(new Artifact_t(child->name, parent));
        for(auto &entity : child->entities) {
          parent->entities.insert(entity);
        }
      }
      for(auto &entity : memberArtifact->entities) {
        parent->entities.insert(entity);
      }
    }

    return newArtifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> SetOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt)
  {
    return nullptr;
  }

}