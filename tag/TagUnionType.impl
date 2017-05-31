//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGUNIONTYPE_H
#define DWARFLOADER_TAGUNIONTYPE_H

#include "./TagGeneric.h"

namespace pcv {
namespace dwarf {

template<>
struct TagHandler<DW_TAG_union_type> {
  static bool handle(Context &ctxt)
  {
    // todo: handle DW_TAG_union_tpye
    return true;
  }
  static bool handleDuplicate(Context &ctxt)
  {
    return true;
  }
};

template<>
struct TagLeaver<DW_TAG_union_type> {
  static void leave(Context &ctxt) {}
  static void leaveDuplicate(Context &ctxt) {}
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_TAGUNIONTYPE_H
