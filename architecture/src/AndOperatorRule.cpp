//
// Created by Faris Cakaric on 19.06.17.
//

#include <iostream>
#include <AndOperatorRule.h>
#include "ArchBuilder.h"
#include "Context.h"

namespace pcv {
  AndOperatorRule::AndOperatorRule(const std::string &artifactName, ArchRule *firstArtifact,
                                 ArchRule *secondArtifact) : artifactName_(artifactName),
                                                             firstArtifact_(firstArtifact),
                                                             secondArtifact_(secondArtifact) {}

  Artifact_t* AndOperatorRule::copyChildren(Artifact_t &parent, Artifact_t &artifact) {
    for(auto &child : artifact.children) {
      parent.children.push_back(std::unique_ptr<Artifact_t>{new Artifact_t(child->name, &parent)});

      for(auto &entity : child->entities)
        parent.children.back().get()->entities.insert(entity);
      copyChildren(*(parent.children.back().get()), *child);
    }
    return &parent;
  }

 /* std::unique_ptr<Artifact_t> findIntersectionOfArtifacts(Artifact_t &first, Artifact_t &second, Artifact_t &archSet) {
    if(first.name == second.name) {
      std::unique_ptr<Artifact_t> intersectArtifact = new Artifact_t(first.name, &archSet);
      for(auto entity : first.entities) {
        if(second.entities.find(entity) != second.entities.end()) {
          intersectArtifact->entities.insert(entity);
        }
      }
      return std::move(intersectArtifact);
    }
    return nullptr;
  }*/

  std::unique_ptr<ArchRule::artifacts_t> AndOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    std::unique_ptr<artifacts_t> firstArtifactsList = firstArtifact_->getArtifacts();
    std::unique_ptr<artifacts_t> secondArtifactsList = secondArtifact_->getArtifacts();
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};

    archSet.children.push_back(std::unique_ptr<Artifact_t>{new Artifact_t(artifactName_, &archSet)});

    Artifact_t *parent = archSet.children.back().get();

    for(auto &artifact : *firstArtifactsList) {
      if(std::find(secondArtifactsList->begin(), secondArtifactsList->begin(), artifact) != secondArtifactsList->end()) {
        std::cout << "Found duplicate: " << artifact->name << std::endl;
      }

    }

    return artifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> AndOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}

}