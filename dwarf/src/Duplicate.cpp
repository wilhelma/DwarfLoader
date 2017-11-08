//
// Created by wilhelma on 1/25/17.
//

#include <dwarf.h>
#include "Duplicate.h"
#include "DwarfHelper.h"
#include "Context.h"
#include "DwarfException.h"

namespace {

bool isRealDuplicate(const pcv::dwarf::Context &ctxt, Dwarf_Die dupDie)
{
  const auto &origDie = ctxt.die;
  const auto &dbg = ctxt.dbg;

  Dwarf_Half tagDup{}, tagOrig{};
  if (dwarf_tag(dupDie, &tagDup, nullptr) != DW_DLV_OK) throw DwarfError("dwarf_tag() failed");
  if (dwarf_tag(origDie, &tagOrig, nullptr) != DW_DLV_OK) throw DwarfError("dwarf_tag() failed");

  // compare tag
  if (tagDup != tagOrig)
    return false;

  {  // compare line
    Dwarf_Unsigned lineDup{}, lineOrig{};
    pcv::dwarf::getAttrUint(dbg, dupDie, DW_AT_decl_line, &lineDup);
    pcv::dwarf::getAttrUint(dbg, origDie, DW_AT_decl_line, &lineOrig);

    if (lineDup != lineOrig)
      return false;
  }

  {  // compare die name
    char *dupChr{}, *origChr{};
    if (pcv::dwarf::getDieName(dbg, origDie, &origChr) &&
      pcv::dwarf::getDieName(dbg, dupDie, &dupChr) &&
      std::string(dupChr) != std::string(origChr))
      return false;
  }

  if (!ctxt.currentClass.empty()) {  // compare template class
    Dwarf_Off offDup{};
    if (dwarf_dieoffset(dupDie, &offDup, nullptr) != DW_DLV_OK) throw DwarfError("dwarf_offset");

    auto entity = ctxt.get<pcv::entity::SoftwareEntity>(offDup);
    if (entity != nullptr) {
      auto clsDup = ctxt.get<pcv::entity::SoftwareEntity>(offDup)->cls;
      if (clsDup != ctxt.currentClass.back())
        return false;
    }
  }

  return true;  // identical
}

}

namespace pcv {
namespace dwarf {

std::size_t DieDuplicate::getHash(const Context &ctxt) const
{
  static std::hash<DieIdentifier_t> hasher;

  Dwarf_Unsigned fileNo{}, line{};
  Dwarf_Half tag{};
  std::string nameStr{};
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

void DieDuplicate::addDuplicate(Context &ctxt)
{
  if (hasAttr(ctxt.die, DW_AT_decl_file) &&
      hasAttr(ctxt.die, DW_AT_decl_line)) {
    auto dup = duplicates_.find(getHash(ctxt));
    if (dup != end(duplicates_)) {
      Dwarf_Off off;
      if (dwarf_dieoffset(ctxt.die, &off, nullptr) != DW_DLV_OK) throw DwarfError("offset");
      ctxt.addDuplicate(dup->second, off);
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

      if (isRealDuplicate(ctxt, d))
        return dup->second;
      }
    }
  }

  return 0;
}

}  // namespace dwarf
}  // namespace pcv
