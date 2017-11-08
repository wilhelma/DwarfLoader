//
// Created by wilhelma on 1/9/17.
//

#include <dwarf.h>
#include <Context.h>
#include <libdwarf.h>
#include "DwarfHelper.h"
#include "DwarfException.h"
#include "Type.h"
#include "entities/SoftwareEntity.h"

namespace {

#if defined __x86_64__
#define DISPLACEMENT 16
#else
#define DISPLACEMENT 8
#endif

bool checkTag(Dwarf_Die die, Dwarf_Half tag)
{
  Dwarf_Half tmpTag;
  return dwarf_tag(die, &tmpTag, nullptr) == DW_DLV_OK && tmpTag == tag;
}

int getFormValues(Dwarf_Attribute attr, Dwarf_Half *form, Dwarf_Half *directForm)
{
  int res = dwarf_whatform(attr, form, nullptr);
  dwarf_whatform_direct(attr, directForm, nullptr);
  return res;
}

}

namespace pcv {
namespace dwarf {

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
    throw DwarfError(std::string("dwarf_hasattr() failed " + std::to_string(attr)));
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
  } else if (hasAttr(die, DW_AT_abstract_origin)) {
    auto aoDie = jump(dbg, die, DW_AT_abstract_origin);
    res = getDieName(dbg, aoDie, name);
    dwarf_dealloc(dbg, aoDie, DW_DLA_DIE);
  } else if (hasAttr(die, DW_AT_specification)) {
      auto specDie = jump(dbg, die, DW_AT_specification);
      res = getDieName(dbg, specDie, name);
      dwarf_dealloc(dbg, specDie, DW_DLA_DIE);
  }

  return res;
}

void handleLocListFromExpr(Dwarf_Debug dbg,
                           Dwarf_Die die,
                           const locationHandler_t& stackHandler,
                           const locationHandler_t& globalAndStaticHandler)
{
  Dwarf_Attribute attr;

  if (dwarf_attr(die, DW_AT_location, &attr, nullptr) == DW_DLV_OK) {
    Dwarf_Half form{};
    Dwarf_Half directForm{};
    getFormValues(attr, &form, &directForm);
    if (form == DW_FORM_exprloc) {
      Dwarf_Unsigned tmp{};
      Dwarf_Ptr x{};
      if (dwarf_formexprloc(attr, &tmp, &x, nullptr) == DW_DLV_OK) {
        Dwarf_Half size{};
        if (dwarf_get_die_address_size(die, &size, nullptr) == DW_DLV_OK) {
          Dwarf_Locdesc *locDesc{};
          Dwarf_Signed len{};
          if (dwarf_loclist_from_expr_a(dbg, x, tmp, size, &locDesc, &len, nullptr) == DW_DLV_OK) {
            for (int i = 0; i < locDesc->ld_cents; i++) {
              Dwarf_Loc *expr{&locDesc->ld_s[i]};
              if (expr->lr_atom == DW_OP_fbreg) {
                stackHandler(dbg, die, static_cast<int>(expr->lr_number) + DISPLACEMENT);
                break;
              }
              else
                globalAndStaticHandler(dbg, die, static_cast<int>(expr->lr_number));
            }
            dwarf_dealloc(dbg, locDesc->ld_s, DW_DLA_LOC_BLOCK);
          }
          dwarf_dealloc(dbg, locDesc, DW_DLA_LOCDESC);
        }
      }
    }
    dwarf_dealloc(dbg, attr, DW_DLA_ATTR);
  }
}

Dwarf_Unsigned getVariableSize(Dwarf_Debug dbg, Dwarf_Die die)
{
  Dwarf_Unsigned bsize {0};

  auto typeDie = jump(dbg, die, DW_AT_type);
  if (checkTag(typeDie, DW_TAG_array_type)) {
    return getArraySize(dbg, typeDie);
  } else if (dwarf_bytesize(typeDie, &bsize, nullptr) != DW_DLV_OK) {
    if (!hasAttr(typeDie, DW_AT_type)) {
      bsize = 0;  // die has no size information!
    } else {
      Dwarf_Die constDie = jump(dbg, typeDie, DW_AT_type);
      if (dwarf_bytesize(constDie, &bsize, nullptr) != DW_DLV_OK) {
        bsize = 0;
      }
      dwarf_dealloc(dbg, constDie, DW_DLA_DIE);
    }
  }
  dwarf_dealloc(dbg, typeDie, DW_DLA_DIE);
  return bsize;
}

void identifySubrangeDie(Dwarf_Die& die)
{

}

Dwarf_Unsigned getArraySize(Dwarf_Debug dbg, Dwarf_Die arrayDie)
{
  Dwarf_Unsigned bsize {0};

  auto typeDie = jump(dbg, arrayDie, DW_AT_type);

  // get element size
  if (hasAttr(typeDie, DW_AT_byte_size)) {
    if (dwarf_bytesize(typeDie, &bsize, nullptr) != DW_DLV_OK) throw DwarfError("size");
  } else {
    auto arrayTypeDie = jump(dbg, typeDie, DW_AT_type);
    Dwarf_Half tag;
    if (dwarf_tag(arrayTypeDie, &tag, nullptr) != DW_DLV_OK) throw DwarfError("dwarf_tag");
    if (tag == DW_TAG_typedef) { // consider typedefs
      arrayTypeDie = jump(dbg, arrayTypeDie, DW_AT_type);
    }

    if (dwarf_bytesize(arrayTypeDie, &bsize, nullptr) != DW_DLV_OK) throw DwarfError("size");
    dwarf_dealloc(dbg, arrayTypeDie, DW_DLA_DIE);
  }

  // get number of elements
  Dwarf_Die subRangeDie{};
  if (dwarf_child(arrayDie, &subRangeDie, nullptr) == DW_DLV_OK) {
    while (1) {
      Dwarf_Attribute attr{};
      Dwarf_Unsigned nElements{};
      Dwarf_Half tag{}, form{};

      // todo: support dynamic sized arrays on the stack (issue #133)
      if (dwarf_attr(subRangeDie, DW_AT_upper_bound, &attr, nullptr) != DW_DLV_OK)
        break;

      if (dwarf_whatform(attr, &form, nullptr) != DW_DLV_OK) throw DwarfError("whatform");
      if (form != DW_FORM_exprloc) {
        if (dwarf_formudata(attr, &nElements, nullptr) != DW_DLV_OK) throw DwarfError("formudata");
        bsize *= (nElements + 1);
      } else {
        // todo: support dynamic sized arrays on the stack (issue #133)
        bsize = 0;
        dwarf_dealloc(dbg, attr, DW_DLA_ATTR);
        break;
      }
      dwarf_dealloc(dbg, attr, DW_DLA_ATTR);

      if (dwarf_tag(subRangeDie, &tag, nullptr) != DW_DLV_OK) throw DwarfError("tag");
      if (dwarf_siblingof_b(dbg, subRangeDie, true, &subRangeDie, nullptr) != DW_DLV_OK) break;
      if (tag != DW_TAG_subrange_type) break;
    }
    dwarf_dealloc(dbg, subRangeDie, DW_DLA_DIE);
  }
  dwarf_dealloc(dbg, typeDie, DW_DLA_DIE);

  return bsize;
}

const char* getVariableName(Dwarf_Debug dbg, Dwarf_Die die)
{
  char const *name {"<undefined>"};
  char* tmp;

  if (getDieName(dbg, die, &tmp)) {
    name = tmp;
  } else {
    auto absDie = jump(dbg, die, DW_AT_abstract_origin);
    if (getDieName(dbg, absDie, &tmp)) {
      name = tmp;
    }
    dwarf_dealloc(dbg, absDie, DW_DLA_DIE);
  }

  return name;
}

Class *getClassOfVariable(Dwarf_Debug dbg, Dwarf_Die die, const Context& ctxt)
{
  if (hasAttr(die, DW_AT_type)) {
    auto typeDie = jump(dbg, die, DW_AT_type);
    Dwarf_Off off{};
    if (dwarf_dieoffset(typeDie, &off, nullptr) == DW_DLV_OK) {
      return ctxt.get<Class>(off);
    }
    dwarf_dealloc(dbg, typeDie, DW_DLA_DIE);
  }
  return nullptr;
}

}  // namespace dwarf
}  // namespace pcv

