//
// Created by wilhelma on 1/9/17.
//

#ifndef DWARFLOADER_DWARFHELPER_H
#define DWARFLOADER_DWARFHELPER_H

#include <libdwarf.h>

bool getPtrOff(Dwarf_Debug dbg, Dwarf_Die, Dwarf_Off *off);
bool hasAttr(Dwarf_Die die, Dwarf_Half attr);
bool getAttrText(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Half attr, char **text);
void getAttrUint(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Half attr, Dwarf_Unsigned *uint);
Dwarf_Die jump(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Half attrDesc);
bool getDieName(Dwarf_Debug dbg, Dwarf_Die, char **name);

#endif //DWARFLOADER_DWARFHELPER_H
