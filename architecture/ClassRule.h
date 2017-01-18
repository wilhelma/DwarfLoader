//
// Created by wilhelma on 1/17/17.
//

#ifndef DWARFLOADER_CLASSRULE_H
#define DWARFLOADER_CLASSRULE_H

#include "ArchRule.h"

namespace pcv {
namespace dwarf {

struct ArchSet_t;

class ClassRule : public ArchRule {
 public:
  void execute(ArchSet_t &archSet, const Context &ctxt) override;
  void append(ArchSet_t &archSet, const Context &ctxt) override;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_CLASSRULE_H
