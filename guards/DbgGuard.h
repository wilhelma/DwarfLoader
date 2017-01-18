//
// Created by wilhelma on 1/4/17.
//

#ifndef DWARFLOADER_DWARFGUARD_H
#define DWARFLOADER_DWARFGUARD_H

#include <libdwarf.h>
#include <stdexcept>
#include "../include/DwarfException.h"
namespace pcv {
namespace dwarf {

struct DbgGuard {
  Dwarf_Debug dbg {};

  explicit DbgGuard(int fileDescriptor, Dwarf_Handler errHandler)
  {
    Dwarf_Error err{};
    if (dwarf_init(fileDescriptor, DW_DLC_READ, errHandler, dbg, &dbg, &err) != DW_DLV_OK)
      throw DwarfError("Can not init Dwarf");
  }

  ~DbgGuard()
  {
    Dwarf_Error err{};
    dwarf_finish(dbg, &err);
  }
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_DWARFGUARD_H
