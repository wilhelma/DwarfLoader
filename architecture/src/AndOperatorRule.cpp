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

  void findIntersectionOfArtifacts(Artifact_t &first, Artifact_t &second, Artifact_t &archSet) {
    Artifact_t* parent = &archSet;
    if(first.name == second.name) {
     // std::cout << first.name << std::endl;
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

  std::unique_ptr<ArchRule::artifacts_t> AndOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};

    archSet.children.push_back(std::unique_ptr<Artifact_t>{new Artifact_t(artifactName_, &archSet)});

    Artifact_t *parent = archSet.children.back().get();

    Artifact_t* firstArtifactSet = firstArtifact_->getArchSet();
    Artifact_t* secondArtifactSet = secondArtifact_->getArchSet();

    for(auto &firstArtifact : firstArtifactSet->children)
      for(auto &secondArtifact : secondArtifactSet->children)
        findIntersectionOfArtifacts(*firstArtifact, *secondArtifact, *parent);

    this->setArchSet(archSet.children.back().get());
    return artifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> AndOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}

}