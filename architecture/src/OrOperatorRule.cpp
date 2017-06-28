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

  void findUnionOfArtifacts(Artifact_t &first, Artifact_t &second, Artifact_t &archSet) {
    Artifact_t* parent = &archSet;
    if(first.name == second.name) {
      std::unique_ptr<Artifact_t> intersectArtifact(new Artifact_t(first.name, parent));
      for(auto &firstChild : first.children)
        for(auto &secondChild : second.children)
          if(firstChild->name == secondChild->name) {
            intersectArtifact->children.push_back(std::unique_ptr<Artifact_t>{new Artifact_t(firstChild->name, intersectArtifact.get())});
            for(auto entity : firstChild->entities)
              if(secondChild->entities.find(entity) != secondChild->entities.end())
                intersectArtifact->children.back()->entities.insert(entity);
          }

      for(auto entity : first.entities)
        if(second.entities.find(entity) != second.entities.end())
          intersectArtifact->entities.insert(entity);
      archSet.children.push_back(std::move(intersectArtifact));
    }
  }

  std::unique_ptr<ArchRule::artifacts_t> OrOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};
    artifact_ = new Artifact_t(artifactName_, &archSet);

    Artifact_t* firstArtifactSet = firstArtifact_->getArchSet();
    Artifact_t* secondArtifactSet = secondArtifact_->getArchSet();

    for(auto &firstArtifact : firstArtifactSet->children)
      for(auto &secondArtifact : secondArtifactSet->children)
        findUnionOfArtifacts(*firstArtifact, *secondArtifact, *artifact_);

    return artifacts;
  }


  std::unique_ptr<ArchRule::artifacts_t> OrOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt)
  {
    return nullptr;
  }

}