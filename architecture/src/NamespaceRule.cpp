//
// Created by wilhelma on 1/10/17.
//

#include "NamespaceRule.h"
#include "ArchBuilder.h"
#include "Context.h"

namespace pcv {

  using entity::Namespace;

  NamespaceRule::NamespaceRule(const std::string &artifactName, const std::string &regexString) : artifactName_(
          artifactName), rx_(regexString) {}

  std::unique_ptr<ArchRule::artifacts_t>
  NamespaceRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) { return nullptr; }

  std::unique_ptr<ArchRule::artifacts_t>
  NamespaceRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    static std::unordered_map<Namespace *, Artifact_t *> addedArtifacts;

    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};

    archSet.children.emplace_back(std::unique_ptr<Artifact_t>{ new Artifact_t(artifactName_, &archSet)});

    Artifact_t *parent = archSet.children.back().get();

    for (auto &nmsp : ctxt.namespaces) {
      if(std::regex_match(nmsp->name, rx_)) {
        if (nmsp->parent && addedArtifacts.find(nmsp->parent) != end(addedArtifacts))
          parent = addedArtifacts[nmsp->parent];

        std::string name = nmsp->name.empty() ? "empty" : nmsp->name;
        parent->children.push_back(
                std::unique_ptr<Artifact_t>{new Artifact_t(name, parent)}
        );

        addedArtifacts[nmsp.get()] = parent->children.back().get();

        auto &children = parent->children.back();
        for (auto &entity : nmsp->entities) {
          children->entities.insert(entity);
        }

        artifacts->emplace_back(parent->children.back().get());
      }
    }

    this->setArchSet(archSet.children.back().get());
    return artifacts;
  }

}  // namespace pcv
