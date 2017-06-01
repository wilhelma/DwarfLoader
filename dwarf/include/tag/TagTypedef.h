//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGTYPEDEF_H
#define DWARFLOADER_TAGTYPEDEF_H

#include "tag/TagGeneric.h"
#include "DwarfHelper.h"

namespace pcv {
namespace dwarf {

class Context;

bool handleTypedef(Context &ctxt)
{
  // consider non-void typedefs
  if (hasAttr(ctxt.die, DW_AT_type)) {
    Dwarf_Off off{};
    Dwarf_Attribute at{};
    char *name;

    dwarf_dieoffset(ctxt.die, &off, nullptr);

    if (dwarf_attr(ctxt.die, DW_AT_type, &at, nullptr) != DW_DLV_OK) throw DwarfError("attr");
    if (dwarf_global_formref(at, &off, nullptr) != DW_DLV_OK) throw DwarfError("formref");
    if (!getDieName(ctxt.dbg, ctxt.die, &name)) throw DwarfError("die_name");
    ctxt.addTypedef(off, std::string(name));
    dwarf_dealloc(ctxt.dbg, at, DW_DLA_ATTR);
  }

  return true;  // do not investigate further
}

template<>
struct TagHandler<DW_TAG_typedef> {
  static bool handle(Context &ctxt)
  {
    return handleTypedef(ctxt);
  }
  static bool handleDuplicate(Context &ctxt)
  {
    return handleTypedef(ctxt);
  }
};

}  // dwarf
}  // pcv


#endif //DWARFLOADER_TAGTYPEDEF_H
