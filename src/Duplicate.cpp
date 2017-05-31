//
// Created by wilhelma on 1/25/17.
//

#include <dwarf.h>
#include "../include/Duplicate.h"
#include "../include/DwarfHelper.h"
#include "../include/Context.h"
#include "../include/DwarfException.h"

namespace {

bool isRealDuplicate(Dwarf_Debug dbg, Dwarf_Die rhs, Dwarf_Die lhs)
{
  Dwarf_Half tagL{}, tagR{};
  Dwarf_Unsigned lineL{}, lineR{};

  if (dwarf_tag(lhs, &tagL, nullptr) != DW_DLV_OK) throw DwarfError("dwarf_tag() failed");
  getAttrUint(dbg, lhs, DW_AT_decl_line, &lineL);
  if (dwarf_tag(rhs, &tagR, nullptr) != DW_DLV_OK) throw DwarfError("dwarf_tag() failed");
  getAttrUint(dbg, rhs, DW_AT_decl_line, &lineR);

  return (tagL == tagR && lineL == lineR);
}

}

namespace pcv {
namespace dwarf {

std::size_t DieDuplicate::getHash(const Context &ctxt) const
{
  static std::hash<DieIdentifier_t> hasher;

  Dwarf_Unsigned fileNo{}, line{};
  Dwarf_Half tag{};
  getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_file, &fileNo);
  getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_line, &line);
  if (dwarf_tag(ctxt.die, &tag, nullptr) != DW_DLV_OK) throw DwarfError("dwarf_tag() failed");
  DieIdentifier_t identifier(tag, line, ctxt.srcFiles[fileNo - 1]);
  return hasher( identifier );
}

void DieDuplicate::addDie(const Context &ctxt)
{
  if (hasAttr(ctxt.die, DW_AT_decl_file) &&
      hasAttr(ctxt.die, DW_AT_decl_line)) {
    Dwarf_Off off;
    if (dwarf_dieoffset(ctxt.die, &off, nullptr) != DW_DLV_OK) throw DwarfError("offset");
    duplicates_[getHash(ctxt)] = off;
  }
}

void DieDuplicate::addDuplicate(const Context &ctxt)
{
  if (hasAttr(ctxt.die, DW_AT_decl_file) &&
      hasAttr(ctxt.die, DW_AT_decl_line)) {
    auto dup = duplicates_.find(getHash(ctxt));
    if (dup != end(duplicates_)) {
      Dwarf_Off off;
      if (dwarf_dieoffset(ctxt.die, &off, nullptr) != DW_DLV_OK) throw DwarfError("offset");
      mappings_[off] = dup->second;
    }
  }
}

Dwarf_Off DieDuplicate::isDuplicate(const Context &ctxt) const
{
  if (hasAttr(ctxt.die, DW_AT_decl_file) &&
      hasAttr(ctxt.die, DW_AT_decl_line)) {
    auto dup = duplicates_.find(getHash(ctxt));
    if (dup != end(duplicates_)) {
      // check for hash collisions
      Dwarf_Die d{};
      if (dwarf_offdie(ctxt.dbg, dup->second, &d, nullptr) != DW_DLV_OK) throw DwarfError("offDie");

      if (isRealDuplicate(ctxt.dbg, ctxt.die, d))
        return dup->second;
    }
  }

  return 0;
}

}  // namespace dwarf
}  // namespace pcv
