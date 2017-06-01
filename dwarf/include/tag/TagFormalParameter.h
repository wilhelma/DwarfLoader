//
// Created by wilhelma on 04/19/17.
//

#ifndef DWARFLOADER_TAGFORMALPARAMETER_H
#define DWARFLOADER_TAGFORMALPARAMETER_H

#include <dwarf.h>
#include <libdwarf.h>
#include <cassert>

#include "TagGeneric.h"

namespace pcv {
namespace dwarf {

using pcv::entity::Variable;

class Context;

template<>
struct TagHandler<DW_TAG_formal_parameter> {
  static bool handle(Context &ctxt)
  {
      if (hasAttr(ctxt.die, DW_AT_decl_file)) { // we do not consider parameters without dbg info

        Dwarf_Unsigned file{}, line{};
        getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_file, &file);
        getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_line, &line);

        VariableArgs var{getVariableName(ctxt.dbg, ctxt.die),
                         getVariableSize(ctxt.dbg, ctxt.die),
                         file,
                         line,
                         (ctxt.currentClass.empty()) ? Class::NO_CLASS
                                                     : ctxt.currentClass.top()->id};

        assert(!ctxt.currentRoutine.empty());

        auto stackHandler = [&ctxt, &var](Dwarf_Debug dbg, Dwarf_Die die, Variable::offset_t offset) {
          Dwarf_Off off{};
          dwarf_dieoffset(die, &off, 0);

          ctxt.variables.emplace_back(
            std::unique_ptr<Variable> {
              new Variable(off,
                           demangle(var.name.c_str()),
                           ctxt.currentImage,
                           ctxt.currentNamespace,
                           getClassOfVariable(ctxt.dbg, ctxt.die, ctxt),
                           ctxt.srcFiles[var.file - 1],
                           var.line,
                           var.size,
                           offset,
                           Variable::Type::STACK)
            }
          );
          ctxt.currentRoutine.top()->locals.emplace_back(ctxt.variables.back().get());
        };

        handleLocListFromExpr(ctxt.dbg, ctxt.die, stackHandler,
                              [](Dwarf_Debug, Dwarf_Die, Variable::offset_t){});
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

#endif // DWARFLOADER_TAGFORMALPARAMETER_H
