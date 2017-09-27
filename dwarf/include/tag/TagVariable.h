//
// Created by wilhelma on 04/19/17.
//

#ifndef DWARFLOADER_TAGVARIABLE_H
#define DWARFLOADER_TAGVARIABLE_H

#include <dwarf.h>
#include <libdwarf.h>

#include "./TagGeneric.h"

namespace pcv {
namespace dwarf {

using pcv::entity::Variable;

class Context;

template<>
struct TagHandler<DW_TAG_variable> {
  static bool handle(Context &ctxt) {
    if (hasAttr(ctxt.die, DW_AT_artificial))
      return true; // stop parsing

    Dwarf_Off off{};
    Dwarf_Die die = ctxt.die;

    if (hasAttr(ctxt.die, DW_AT_specification)) {
      die = jump(ctxt.dbg, ctxt.die, DW_AT_specification);
      if (dwarf_dieoffset(die, &off, 0) == DW_DLV_OK) {
        auto specVariable = ctxt.getVariable(off);
        if (specVariable == nullptr)
          return true;
      }
    } else {
      dwarf_dieoffset(die, &off, 0);
    }

    if (ctxt.die != die ||
        (!hasAttr(ctxt.die, DW_AT_declaration) &&
          hasAttr(die, DW_AT_decl_file) &&
          hasAttr(die, DW_AT_decl_line))) {
      Dwarf_Unsigned file{}, line{};
      getAttrUint(ctxt.dbg, die, DW_AT_decl_file, &file);
      getAttrUint(ctxt.dbg, die, DW_AT_decl_line, &line);

      VariableArgs var{getVariableName(ctxt.dbg, die),
                       getVariableSize(ctxt.dbg, die),
                       file,
                       line,
                       (ctxt.currentClass.empty()) ? Class::NO_CLASS : ctxt.currentClass.top()->id};

      auto addVariable = [&](Dwarf_Debug dbg, Dwarf_Die die, Variable::offset_t offset,
                             Variable::Type type) {
        ctxt.addVariable(off, std::unique_ptr<Variable> {
           new Variable(off,
                        var.name,
                        ctxt.currentImage,
                        ctxt.currentNamespace,
                        getClassOfVariable(ctxt.dbg, ctxt.die, ctxt),
                        ctxt.srcFiles[var.file - 1],
                        var.line,
                        var.size,
                        offset,
                        type)
        });
      };

      auto stackHandler = [&](Dwarf_Debug dbg, Dwarf_Die die, Variable::offset_t offset) {
        addVariable(dbg, die, offset, Variable::Type::STACK);
        ctxt.currentRoutine.top()->locals.emplace_back(ctxt.variables.back().get());
      };

      auto staticGlobalHandler = [&](Dwarf_Debug dbg, Dwarf_Die die, Variable::offset_t offset) {
        if (ctxt.currentRoutine.empty()) {
          addVariable(dbg, die, offset, Variable::Type::GLOBAL);
        } else {
          addVariable(dbg, die, offset, Variable::Type::STATICVAR);
          ctxt.currentRoutine.top()->locals.emplace_back(ctxt.variables.back().get());
        }
      };

      handleLocListFromExpr(ctxt.dbg, ctxt.die, stackHandler, staticGlobalHandler);
    }

    return true;  // do not investigate further
  }

  static
  bool handleDuplicate(Context &ctxt) {
    auto var = ctxt.getVariable(ctxt.duplicate);
    return (var != nullptr);
  }
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_TAGVARIABLE_H
