//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGHANDLER_H
#define DWARFLOADER_TAGHANDLER_H

#include <libdwarf.h>
#include <dwarf.h>
#include <cstdlib>

#include "../include/Context.h"
#include "../include/Type.h"

#include "../entities/Variable.h"

namespace pcv {
namespace dwarf {

template<Dwarf_Half tag>
struct TagHandler {
  static bool handle(Context& /* ctxt */)
  {
    std::cerr << "Tag " << tag << " declared but not defined!\n";
    abort();
  }
  static bool handleDuplicate(Context& /* ctxt */)
  {
    std::cerr << "Tag " << tag << " declared but not defined!\n";
    abort();
  }
};

template<Dwarf_Half tag>
struct TagLeaver {
  static void leave(Context& /* ctxt */)
  {
    std::cerr << "Tag " << tag << " declared but not defined!\n";
    abort();
  }
  static void leaveDuplicate(Context& /* ctxt */)
  {
    std::cerr << "Tag " << tag << " declared but not defined!\n";
    abort();
  }
};

template<Dwarf_Half ...>
struct TagList {};

bool handleDie(Context&, Dwarf_Half, TagList<>) { return false; }

template<Dwarf_Half I, Dwarf_Half ...N>
bool handleDie(Context &ctxt, Dwarf_Half i, TagList<I, N...>)
{
  if (i != I) { return handleDie(ctxt, i, TagList<N...>()); }

  if (ctxt.duplicate)
    return TagHandler<I>::handleDuplicate(ctxt);
  else
    return TagHandler<I>::handle(ctxt);
}

template<Dwarf_Half ...N>
bool handleDie(Context &ctxt, Dwarf_Half tag)
{
  return handleDie(ctxt, tag, TagList<N...>());
}

void leaveDie(Context&, Dwarf_Half, TagList<>) {}

template<Dwarf_Half I,Dwarf_Half...N>
void leaveDie(Context &ctxt, Dwarf_Half i, TagList<I, N...>)
{
  if (i != I) { return leaveDie(ctxt, i, TagList<N...>()); }

  if (ctxt.duplicate)
    TagLeaver<I>::leaveDuplicate(ctxt);
  else
    TagLeaver<I>::leave(ctxt);
}

template<Dwarf_Half...N>
void leaveDie(Context &ctxt, Dwarf_Half tag)
{
  return leaveDie(ctxt, tag, TagList<N...>());
}

using handleDwarfDie_t = bool(Context&, Dwarf_Half);
handleDwarfDie_t *handleDwarfDie = handleDie<DW_TAG_compile_unit,
                                             DW_TAG_member,
                                             DW_TAG_namespace,
                                             DW_TAG_subprogram,
                                             DW_TAG_class_type,
                                             DW_TAG_structure_type,
                                             DW_TAG_inheritance,
                                             DW_TAG_typedef,
                                             DW_TAG_union_type>;

using leaveDwarfDie_t = void(Context&, Dwarf_Half);
leaveDwarfDie_t *leaveDwarfDie = leaveDie<DW_TAG_namespace,
                                          DW_TAG_class_type,
                                          DW_TAG_structure_type,
                                          DW_TAG_compile_unit,
                                          DW_TAG_union_type>;

}  // namespace dwarf
}  // namespace pcv

#include "./TagCompileUnit.impl"
#include "./TagMember.impl"
#include "./TagNamespace.impl"
#include "./TagSubprogram.impl"
#include "./TagClass.impl"
#include "./TagInheritance.impl"
#include "./TagTypedef.impl"
#include "./TagUnionType.impl"

#endif //DWARFLOADER_TAGHANDLER_H
