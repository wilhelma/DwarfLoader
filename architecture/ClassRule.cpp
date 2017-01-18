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

const Class *getBaseClass(const Class *c, const ArchSet_t &as)
{
  const Class *cp = c;

  // todo: consider multiple inheritance
  for (auto bc : c->baseClasses)
    cp = getBaseClass(bc, as);

  return cp;
};

void ClassRule::execute(ArchSet_t &archSet, const Context &ctxt)
{
/*  for (auto &c : ctxt.classes) {
    archSet.children.push_back(
      std::unique_ptr<ArchSet_t>{ new ArchSet_t(c->name) }
    );
  }*/
}

void ClassRule::append(ArchSet_t &as, const Context &ctxt)
{
  for (auto &c : as.children)
    append(*c.get(), ctxt);

  std::unordered_map<const Class*, ArchSet_t *> added;
  std::unordered_set<const SoftwareEntity*> toRemove;
  for (auto e : as.entities) {
    if (e->getEntityType() == EntityType::Class) {
      const Class *c = reinterpret_cast<const Class *>(e);
      const Class *bc = getBaseClass(c, as);

      if (added.find(bc) == end(added)) {
        as.children.emplace_back(std::unique_ptr<ArchSet_t>{
            new ArchSet_t(bc->name, &as)});
        added[bc] = as.children.back().get();
      }

      added[bc]->entities.insert(c);
      toRemove.insert(e);
    }
  }

  for (auto e : toRemove)
    as.entities.erase(e);
}

}  // namespace dwarf
}  // namespace pcv
