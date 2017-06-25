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
    archSet.children.push_back(std::unique_ptr<Artifact_t>{new Artifact_t(artifactName_, &archSet)});

    for(auto &memberArtifact : artifacts) {
      Artifact_t *parent = archSet.children.back().get();
      std::unique_ptr<artifacts_t> artifactsList = memberArtifact->getArtifacts();
      if(artifactsList.get()[0][0]->parent) {
        parent->children.emplace_back(new Artifact_t(artifactsList.get()[0][0]->parent->name, parent));
        for(auto &entity : artifactsList.get()[0][0]->parent->entities)
          parent->children.back().get()->entities.insert(entity);
        parent = parent->children.back().get();
      }
      for(auto &artifact : *artifactsList) {
        parent->children.emplace_back(new Artifact_t(artifact->name, parent));
        for(auto &entity : artifact->entities)
          parent->children.back().get()->entities.insert(entity);
        newArtifacts->emplace_back(copyChildren(*(parent->children.back().get()), *artifact));
      }
    }

    this->setArchSet(archSet.children.back().get());
    return newArtifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> SetOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}
}