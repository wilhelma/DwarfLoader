//
// Created by wilhelma on 1/17/17.
//

#include "ClassRule.h"

#include <algorithm>
#include <iostream>

#include "./ArchBuilder.h"
#include "../include/Context.h"

namespace pcv {
namespace dwarf {

const Class *getBaseClass(const Class *c)
{
  const Class *cp = c;

  // todo: consider multiple inheritance
  for (auto bc : c->baseClasses)
    cp = getBaseClass(bc);

  return cp;
};

Artifact_t* appendHierarchy(const Class *c, Artifact_t &as,
                     std::unordered_set<const SoftwareEntity*> *toRemove)
{
  static std::unordered_map<const Class*, Artifact_t*> added;

  const Class *bc = getBaseClass(c);
  if (added.find(bc) == end(added)) {
    as.children.emplace_back(std::unique_ptr<Artifact_t>{ new Artifact_t(bc->name, &as)});
    added[bc] = as.children.back().get();
  }

  added[bc]->children.emplace_back(
      std::unique_ptr<Artifact_t>{ new Artifact_t(c->name, added[bc]) }
  );

  added[bc]->children.back()->entities.insert( c );
  toRemove->insert( c );

  for (auto &member : c->members) {
    added[bc]->children.back()->entities.insert( member );
    toRemove->insert( member );
  }

  for (auto &method : c->methods) {
    added[bc]->children.back()->entities.insert( method );
    toRemove->insert( method );
  }

  return added[bc];
}

std::unique_ptr<ArchRule::artifacts_t>
ClassRule::execute(Artifact_t &archSet, const Context &ctxt)
{
  auto artifacts = std::unique_ptr<artifacts_t> { new artifacts_t };
  std::unordered_set<const SoftwareEntity*> toRemove;

  for (auto &c : ctxt.classes)
    artifacts->emplace_back( appendHierarchy(c.get(), archSet, &toRemove) );

  for (auto e : toRemove)
    archSet.entities.erase(e);

  return artifacts;
}

std::unique_ptr<ArchRule::artifacts_t>
ClassRule::append(Artifact_t &as, const Context &ctxt)
{
  auto artifacts = std::unique_ptr<artifacts_t> { new artifacts_t };

  for (auto &c : as.children)
    append(*c.get(), ctxt);

  std::unordered_set<const SoftwareEntity*> toRemove;
  for (auto e : as.entities) {
    if (e->getEntityType() == EntityType::Class) {
      const Class *c = reinterpret_cast<const Class *>(e);
      artifacts->emplace_back( appendHierarchy(c, as, &toRemove) );
    }
  }

  for (auto e : toRemove)
    as.entities.erase(e);

  return artifacts;
}

}  // namespace dwarf
}  // namespace pcv
