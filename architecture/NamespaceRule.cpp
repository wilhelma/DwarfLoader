//
// Created by wilhelma on 1/10/17.
//

#include "NamespaceRule.h"
#include "./ArchBuilder.h"
#include "../include/Context.h"

namespace pcv {
namespace dwarf {

std::unique_ptr<ArchRule::artifacts_t>
NamespaceRule::append(Artifact_t &archSet, const Context &ctxt) {}

std::unique_ptr<ArchRule::artifacts_t>
NamespaceRule::execute(Artifact_t &archSet, const Context &ctxt)
{
  static std::unordered_map<Namespace*, Artifact_t*> addedArtifacts;

  auto artifacts = std::unique_ptr<artifacts_t> { new artifacts_t };
  Artifact_t *parent = &archSet;

  for (auto &nmsp : ctxt.namespaces) {
    if (nmsp->parent && addedArtifacts.find(nmsp->parent) != end(addedArtifacts))
      parent = addedArtifacts[nmsp->parent];

    std::string name = nmsp->name.empty() ? "empty" : nmsp->name;
    parent->children.push_back(
        std::unique_ptr<Artifact_t>{ new Artifact_t(name, parent) }
    );

    addedArtifacts[nmsp.get()] = parent->children.back().get();

    auto &children = parent->children.back();
    for (auto &entity : nmsp->entities) {
        children->entities.insert( entity );
    }

    artifacts->emplace_back( parent->children.back().get() );
  }

  return artifacts;
}

}  // namespace dwarf
}  // namespace pcv
