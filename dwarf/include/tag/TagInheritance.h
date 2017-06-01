//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGINHERITANCE_H
#define DWARFLOADER_TAGINHERITANCE_H

#include <cassert>

#include "./TagGeneric.h"

namespace pcv {
namespace dwarf {

void handleInheritance(Context &ctxt, Dwarf_Off &baseOff)
{
  assert(!ctxt.currentClass.empty() && "The context contains no current class!");
  ctxt.addInheritance(baseOff, ctxt.currentClass.top()->id);
}

template <> struct TagHandler<DW_TAG_inheritance> {
  static bool handle(Context &ctxt)
  {
    Dwarf_Die base = jump(ctxt.dbg, ctxt.die, DW_AT_type);

    if (!hasAttr(ctxt.die, DW_AT_declaration)) {
      Dwarf_Off off{};
      if (dwarf_dieoffset(base, &off, nullptr) != DW_DLV_OK) throw DwarfError("die_offset");
      handleInheritance(ctxt, off);
    }
    return true;  // do not investigate further
  }
  static bool handleDuplicate(Context &ctxt)
  {
    handleInheritance(ctxt, ctxt.duplicate);
    return true;  // do not investigate further
  }
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_TAGINHERITANCE_H
