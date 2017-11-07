//
// Created by Faris Cakaric on 18.06.17.
//

#include <iostream>
#include <ArchBuilder.h>
#include "SetOperatorRule.h"

namespace pcv {
  void SetOperatorRule::copyChildren(Artifact_t &parent, Artifact_t &artifact) {
    for(auto &child : artifact.children) {
      parent.children.emplace_back(new Artifact_t(child->name, &parent));
      auto copied = parent.children.back().get();
      if(child->entity) {
        copied->entity = child->entity;
      } else {
        copied->entity = nullptr;
      }
      copyChildren(*copied, *child);
    }
  }

  std::unique_ptr<ArchRule::artifacts_t> SetOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto newArtifacts = std::unique_ptr<artifacts_t> {new artifacts_t};
    artifact_ = new Artifact_t(artifactName_, &archSet);
    artifact_->entity = nullptr;

    for(auto &memberArtifact : artifacts) {
      artifact_->children.emplace_back(new Artifact_t(memberArtifact->name, artifact_));
      Artifact_t* parent = artifact_->children.back().get();
      parent->entity = memberArtifact->entity;
      copyChildren(*parent, *memberArtifact);
    }

    return newArtifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> SetOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt)
  {
    return nullptr;
  }

}