//
// Created by wilhelma on 1/9/17.
//

#include <dwarf.h>
#include "DwarfHelper.h"
#include "DwarfException.h"

bool getAttrText(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Half attr, char **text)
{
  bool res{ false };
  Dwarf_Attribute attribute {};

  if (dwarf_attr(die, attr, &attribute, nullptr) == DW_DLV_OK) {
    if (dwarf_formstring(attribute, text, nullptr) == DW_DLV_OK) {
      res = true;
    }
    dwarf_dealloc(dbg, attribute, DW_DLA_ATTR);
  }

  return res;
}

void getAttrUint(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Half attr, Dwarf_Unsigned *uint)
{
  Dwarf_Attribute attribute;

  if (dwarf_attr(die, attr, &attribute, nullptr) != DW_DLV_OK) throw DwarfError("attr");
  if (dwarf_formudata(attribute, uint, nullptr) != DW_DLV_OK) throw DwarfError("formudata");
  dwarf_dealloc(dbg, attribute, DW_DLA_ATTR);
}

bool hasAttr(Dwarf_Die die, Dwarf_Half attr)
{
  Dwarf_Bool has_attr { false };
  if (dwarf_hasattr(die, attr, &has_attr, nullptr) != DW_DLV_OK)
    throw DwarfError(std::string("dwarf_hasattr() failed " + attr));
  return has_attr;
}

Dwarf_Die jump(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Half attrDesc)
{
  Dwarf_Attribute attr {};
  Dwarf_Off off {};
  Dwarf_Die jumpDie {};

  if (dwarf_attr(die, attrDesc, &attr, nullptr) != DW_DLV_OK) throw DwarfError("attr");
  if (dwarf_global_formref(attr, &off, nullptr) != DW_DLV_OK) throw DwarfError("global_formref");
  if (dwarf_offdie_b(dbg, off, 1, &jumpDie, nullptr) != DW_DLV_OK) throw DwarfError("offdie_b");

  dwarf_dealloc(dbg, attr, DW_DLA_ATTR);
  return jumpDie;
}

bool getPtrOff(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Off *off)
{
  Dwarf_Off off2{};
  dwarf_die_CU_offset(die, &off2, nullptr);

  Dwarf_Half tag{};
  if (dwarf_tag(die, &tag, nullptr) != DW_DLV_OK) throw DwarfError("tag");
  switch (tag) {
    case DW_TAG_const_type:
      if (hasAttr(die, DW_AT_type))
        return getPtrOff(dbg, jump(dbg, die, DW_AT_type), off);
      break;
    case DW_TAG_reference_type:
    case DW_TAG_pointer_type:
      if (!hasAttr(die, DW_AT_type)) return false;
      return getPtrOff(dbg, jump(dbg, die, DW_AT_type), off);
    case DW_TAG_structure_type:
    case DW_TAG_class_type: { // support smart pointers by relying on first template parameter
      Dwarf_Die tmp_die{};
      if (dwarf_child(die, &tmp_die, nullptr) == DW_DLV_OK) {
        while (dwarf_siblingof(dbg, tmp_die, &tmp_die, nullptr) == DW_DLV_OK) {
          Dwarf_Half sib_tag{};
          if (dwarf_tag(tmp_die, &sib_tag, nullptr) != DW_DLV_OK) throw DwarfError("tag");
          if (sib_tag == DW_TAG_template_type_param) { // first param is pointer type
            Dwarf_Attribute at;
            if (dwarf_attr(tmp_die, DW_AT_type, &at, nullptr) == DW_DLV_OK) {
              if (dwarf_global_formref(at, off, nullptr) != DW_DLV_OK) throw DwarfError("formref");
              dwarf_dealloc(dbg, at, DW_DLA_ATTR);
              return true;
            }
          }
        }
      }
      break;
    }
    default:
      break;
  }
  return false;
}

bool getDieName(Dwarf_Debug dbg, Dwarf_Die die, char **name)
{
  bool res{ false };
  if (getAttrText(dbg, die, DW_AT_MIPS_linkage_name, name) ||
      getAttrText(dbg, die, DW_AT_linkage_name, name) ||
      getAttrText(dbg, die, DW_AT_name, name)) {
    res = true;
  }

  return res;
}
