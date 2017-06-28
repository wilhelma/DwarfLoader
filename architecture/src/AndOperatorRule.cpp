//
// Created by Faris Cakaric on 19.06.17.
//

#include <iostream>
#include <AndOperatorRule.h>
#include "ArchBuilder.h"
#include "Context.h"
#include "entities/SoftwareEntity.h"
using pcv::entity::SoftwareEntity;
using pcv::entity::EntityType;
namespace pcv {
  AndOperatorRule::AndOperatorRule(const std::string &artifactName, ArchRule *firstArtifact,
                                 ArchRule *secondArtifact) : artifactName_(artifactName),
                                                             firstArtifact_(firstArtifact),
                                                             secondArtifact_(secondArtifact) {}

  void findIntersectionOfArtifacts(Artifact_t &first, Artifact_t &second, Artifact_t &archSet) {
    Artifact_t* parent = &archSet;
    if((first.name == second.name) && first.name != "") {
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
    } else {
      for(auto &entity : first.entities) {
        for(auto &childOfSecond : second.children) {
          if (entity->getEntityType() == EntityType::Class && (entity->name == childOfSecond->name)) {
            std::unique_ptr<Artifact_t> intersectArtifact(new Artifact_t(childOfSecond->name, parent));
            for (auto &child : childOfSecond->children) {
              intersectArtifact->children.emplace_back(new Artifact_t(child->name, intersectArtifact.get()));
              for (auto childEntity : child->entities) {
                intersectArtifact->entities.insert(childEntity);
              }
            }

            for (auto entityFromSecond : childOfSecond->entities)
              intersectArtifact->entities.insert(entityFromSecond);

            archSet.children.push_back(std::move(intersectArtifact));
          }
        }
      }
      for(auto &entity : second.entities) {
        for(auto &childOfFirst : first.children) {
          if(entity->getEntityType() == EntityType::Class && (entity->name == childOfFirst->name)) {
            std::cout << "hre";
            std::unique_ptr<Artifact_t> intersectArtifact(new Artifact_t(childOfFirst->name, parent));
            for(auto &child : childOfFirst->children) {
              intersectArtifact->children.emplace_back(new Artifact_t(child->name, intersectArtifact.get()));
              for(auto childEntity : child->entities) {
                intersectArtifact->entities.insert(childEntity);
              }
            }

            for(auto entityFromSecond : childOfFirst->entities)
              intersectArtifact->entities.insert(entityFromSecond);

            archSet.children.push_back(std::move(intersectArtifact));
          }
        }

      }
    }
    for(auto &firstChild : first.children)
      for(auto &secondChild : second.children)
        findIntersectionOfArtifacts(*firstChild, *secondChild, *parent);
  }

  std::unique_ptr<ArchRule::artifacts_t> AndOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};
    artifact_ = new Artifact_t(artifactName_, &archSet);

    Artifact_t* firstArtifactSet = firstArtifact_->getArchSet();
    Artifact_t* secondArtifactSet = secondArtifact_->getArchSet();


        findIntersectionOfArtifacts(*firstArtifactSet, *secondArtifactSet, *artifact_);

    return artifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> AndOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}



}