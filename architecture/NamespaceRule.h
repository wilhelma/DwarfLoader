//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_NAMESPACERULE_H
#define DWARFLOADER_NAMESPACERULE_H

#include "ArchRule.h"

namespace pcv {
namespace dwarf {

struct ArchSet_t;

class NamespaceRule : public ArchRule {
 public:
  void execute(ArchSet_t &archSet, const Context &ctxt) override;
  void append(ArchSet_t &archSet, const Context &ctxt) override;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_NAMESPACERULE_H
