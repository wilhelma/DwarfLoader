//
// Created by Faris Cakaric on 17.06.17.
//

#include <iostream>
#include "OrOperatorRule.h"
#include "ArchBuilder.h"
#include "Context.h"


namespace pcv {
  OrOperatorRule::OrOperatorRule(const std::string &artifactName, ArchRule* firstArtifact,
                    ArchRule* secondArtifact) : artifactName_(artifactName),
                                                                      firstArtifact_(firstArtifact),
                                                                      secondArtifact_(secondArtifact) {}
  Artifact_t* OrOperatorRule::copyChildren(Artifact_t &parent, Artifact_t &artifact) {
    for(auto &child : artifact.children) {
      parent.children.push_back(std::unique_ptr<Artifact_t>{new Artifact_t(child->name, &parent)});

      for(auto &entity : child->entities)
        parent.children.back().get()->entities.insert(entity);
      copyChildren(*(parent.children.back().get()), *child);
    }
    return &parent;
  }
  std::unique_ptr<ArchRule::artifacts_t> OrOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    std::unique_ptr<artifacts_t> firstArtifactsList = firstArtifact_->getArtifacts();
    std::unique_ptr<artifacts_t> secondArtifactsList = secondArtifact_->getArtifacts();
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};

    archSet.children.push_back(std::unique_ptr<Artifact_t>{new Artifact_t(artifactName_, &archSet)});

    Artifact_t *parent = archSet.children.back().get();

    for(auto &artifact : *firstArtifactsList) {
      parent->children.emplace_back(new Artifact_t(artifact->name, parent));
      for(auto &entity : artifact->entities)
        parent->children.back().get()->entities.insert(entity);
      artifacts->emplace_back(copyChildren(*(parent->children.back().get()), *artifact));
    }

    for(auto &artifact : *secondArtifactsList) {
      parent->children.emplace_back(new Artifact_t(artifact->name, parent));
      for(auto &entity : artifact->entities)
        parent->children.back().get()->entities.insert(entity);
      artifacts->emplace_back(copyChildren(*(parent->children.back().get()), *artifact));
    }

    return artifacts;
  }


  std::unique_ptr<ArchRule::artifacts_t> OrOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}
}