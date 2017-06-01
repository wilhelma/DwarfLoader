//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGMEMBER_H
#define DWARFLOADER_TAGMEMBER_H

#include "tag/TagGeneric.h"
#include "DwarfHelper.h"
#include "entities/Class.h"

namespace {

using pcv::dwarf::Context;
using pcv::dwarf::Namespace;
using pcv::dwarf::Class;

std::string fillNmspString(const Context& ctxt, Namespace* nmsp)
{
  if (!nmsp->parent || nmsp->parent == ctxt.emptyNamespace)
    return nmsp->name;
  else
    return fillNmspString(ctxt, nmsp->parent) + "::" + nmsp->name;
}

std::string fillClassString(const Context& ctxt, std::stack<Class*>& s)
{
    std::string className = s.top()->name;
    s.pop();

    if (s.empty())
      return className;
    else
      return fillClassString(ctxt, s) + "::" + className;
}

std::string prepareMemberName(const Context& ctxt, const std::string& varName)
{
  auto tmpStack(ctxt.currentClass);
  auto nmspString = fillNmspString(ctxt, ctxt.currentNamespace);
  auto classString = fillClassString(ctxt, tmpStack);

  return ((nmspString.empty()) ? "" : (nmspString + "::")) +
         ((classString.empty()) ? "" : (classString + "::")) + varName;
};

}

namespace pcv {
namespace dwarf {

std::string demangle(const char *);

class Context;

template<>
struct TagHandler<DW_TAG_member> {
  static bool handle(Context &ctxt)
  {
    if (!hasAttr(ctxt.die, DW_AT_artificial) &&
        !hasAttr(ctxt.die, DW_AT_declaration)) {
      if (ctxt.currentClass.empty()) throw DwarfError("No class for member");

      Dwarf_Off off{};
      if (dwarf_dieoffset(ctxt.die, &off, nullptr) != DW_DLV_OK) throw DwarfError("dwarf_dieoff");

      if (getPtrOff(ctxt.dbg, jump(ctxt.dbg, ctxt.die, DW_AT_type), &off)) {
        ctxt.addComposition(ctxt.currentClass.top()->id, off);
      } else { // member variable
        char *varName{nullptr};
        if (getDieName(ctxt.dbg, ctxt.die, &varName)) {
          Dwarf_Unsigned file{}, line{}, offset{};
          Dwarf_Half tag{};
          entity::Class* classType {nullptr};

          getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_file, &file);
          getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_line, &line);
          getAttrUint(ctxt.dbg, ctxt.die, DW_AT_data_member_location, &offset);

          auto typeDie = pcv::dwarf::jump(ctxt.dbg, ctxt.die, DW_AT_type);
          if (dwarf_tag(typeDie, &tag, nullptr) != DW_DLV_OK) throw DwarfError("tag");
          if (tag == DW_TAG_structure_type || tag == DW_TAG_class_type) {
            Dwarf_Off off{};
            if (dwarf_dieoffset(typeDie, &off, nullptr) != DW_DLV_OK) throw DwarfError("dieoffset");
            classType = ctxt.getClass(off);
          }

          ctxt.variables.emplace_back(
              std::unique_ptr<Variable> {
                  new Variable(off,
                               prepareMemberName(ctxt, varName),
                               ctxt.currentImage,
                               ctxt.currentNamespace,
                               ctxt.currentClass.top(),
                               ctxt.srcFiles[file - 1],
                               line,
                               getVariableSize(ctxt.dbg, ctxt.die),
                               offset,
                               Variable::Type::STACK,
                               classType)
              }
          );
          ctxt.currentClass.top()->members.emplace_back(ctxt.variables.back().get());
        }
      }
    }

    return true;  // do not investigate further
  }
  static bool handleDuplicate(Context &ctxt)
  {
    return false;
  }
};

}  // dwarf
}  // pcv


#endif //DWARFLOADER_TAGMEMBER_H
