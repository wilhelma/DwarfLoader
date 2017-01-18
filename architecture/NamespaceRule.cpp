//
// Created by wilhelma on 1/10/17.
//

#include "NamespaceRule.h"
#include "./ArchBuilder.h"
#include "../include/Context.h"

namespace pcv {
namespace dwarf {

void NamespaceRule::append(ArchSet_t &archSet, const Context &ctxt) {}

void NamespaceRule::execute(ArchSet_t &archSet, const Context &ctxt)
{
  for (auto &nmsp : ctxt.namespaces) {
    archSet.children.push_back(
        std::unique_ptr<ArchSet_t>{ new ArchSet_t(nmsp->name, &archSet) }
    );

    auto &children = archSet.children.back();
    for (auto &entity : nmsp->entities) {
      children->entities.insert( entity );
    }
  }
}

}  // namespace dwarf
}  // namespace pcv
