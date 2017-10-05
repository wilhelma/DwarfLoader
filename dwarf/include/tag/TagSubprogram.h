//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGSUBPROGRAM_H
#define DWARFLOADER_TAGSUBPROGRAM_H

#include "tag/TagGeneric.h"
#include "entities/Routine.h"
#include "Type.h"
#include "DwarfReader.h"
#include "Context.h"

namespace {

bool isValid(const Context &ctxt, Dwarf_Die die)
{
  return !(ctxt.currentClass.empty() && pcv::dwarf::hasAttr(die, DW_AT_artificial));
}

}

namespace pcv {
namespace dwarf {

bool inspectConstructors(Context &ctxt, Dwarf_Die die)
{
  char *rtnName = nullptr;
  if (getDieName(ctxt.dbg, die, &rtnName)) {
    std::string uName = demangle(rtnName);
    size_t pos2{uName.rfind("::")};
    if (pos2 != std::string::npos) {
      size_t pos1{uName.rfind("::", pos2 - 1)};
      if (pos1 != std::string::npos) pos1 += 2;
      else pos1 = 0;

      const std::string className = uName.substr(pos1, pos2 - pos1);
      const std::string methodName = uName.substr(pos2 + 2);

      if (className == methodName) {
        if (hasAttr(die, DW_AT_abstract_origin)) {
          auto declDie = jump(ctxt.dbg, die, DW_AT_abstract_origin);
          if (hasAttr(declDie, DW_AT_specification)) {
            auto specDie = jump(ctxt.dbg, declDie, DW_AT_specification);
            Dwarf_Off specOff{};

            // add line of declaration to class constructors
            dwarf_dieoffset(specDie, &specOff, nullptr);
            auto rtn = ctxt.getRoutine(specOff);
            if (rtn != nullptr) {
              rtn->isConstructor = true;
              ctxt.linkNameToRoutine(rtnName, rtn);
            }
            dwarf_dealloc(ctxt.dbg, specDie, DW_DLA_DIE);
          }
          dwarf_dealloc(ctxt.dbg, declDie, DW_DLA_DIE);

          return true;
        }
      }
    }
  }
  return false;
}

bool handleSubProgram(Context &ctxt, Dwarf_Die die, Dwarf_Off off = 0)
{
  static std::unordered_map<Dwarf_Off, Routine*> handled;

  if (isValid(ctxt, die)) {
    if (off) {
      Dwarf_Off specOff;
      if (dwarf_dieoffset(die, &specOff, nullptr) != DW_DLV_OK) throw DwarfError("offset");
      if (handled.find(specOff) != std::end(handled)) {
        ctxt.currentRoutine.emplace(handled[specOff]);
        return false;  // continue
      }
    }

    char *rtnName{nullptr};
    if (!getDieName(ctxt.dbg, die, &rtnName)) throw DwarfError("diename");

    // handle constructors
    bool isConstructor {!ctxt.currentClass.empty() && (ctxt.currentClass.top()->name == rtnName)};
    if (std::strstr(rtnName, "C1") != nullptr ||
      std::strstr(rtnName, "C2") != nullptr ||
      std::strstr(rtnName, "C3") != nullptr) {
      inspectConstructors(ctxt, die);
    }

    if (!off && (dwarf_dieoffset(die, &off, nullptr) != DW_DLV_OK)) throw DwarfError("offset");

    Dwarf_Unsigned fileNo{}, lineNo{};
    std::string file{};
    if (hasAttr(die, DW_AT_decl_file)) {
      getAttrUint(ctxt.dbg, die, DW_AT_decl_file, &fileNo);
      file = ctxt.srcFiles[fileNo - 1];
      getAttrUint(ctxt.dbg, die, DW_AT_decl_line, &lineNo);
    }

    if (lineNo > 0 || isConstructor) {
      auto rtn = std::unique_ptr<Routine> {
          new Routine(off,
                      demangle(rtnName),
                      ctxt.currentImage,
                      ctxt.currentNamespace,
                      ctxt.currentClass.empty() ? nullptr : ctxt.currentClass.top(),
                      file,
                      lineNo)};

      ctxt.addRoutine(off, std::move(rtn));
      handled[off] = ctxt.routines.back().get();

      if (!ctxt.currentClass.empty())
        ctxt.currentClass.top()->methods.push_back(ctxt.routines.back().get());

      ctxt.currentRoutine.emplace(ctxt.routines.back().get());

      return false; // continue
    }
  }
  return true; // stop
}

template<>
struct TagHandler<DW_TAG_subprogram> {
  static bool handle(Context &ctxt)
  {
    bool handled = true;

    if (hasAttr(ctxt.die, DW_AT_specification)) {
      Dwarf_Off off{}, specOff{};
      auto specDie = jump(ctxt.dbg, ctxt.die, DW_AT_specification);
      if (dwarf_dieoffset(ctxt.die, &off, 0) == DW_DLV_OK) {
        if (dwarf_dieoffset(specDie, &specOff, 0) == DW_DLV_OK) {
          auto specRoutine = ctxt.getRoutine(specOff);
//          if (specRoutine == nullptr)
//            return true;

          auto tmpNmsp = ctxt.currentNamespace;
          if (specRoutine != nullptr) {
            ctxt.currentNamespace = specRoutine->nmsp;
          }

          handled = handleSubProgram(ctxt, specDie, off);

          if (specRoutine != nullptr)
            ctxt.currentNamespace = tmpNmsp;
        }
      }
      dwarf_dealloc(ctxt.dbg, specDie, DW_DLA_DIE);
    } else if (hasAttr(ctxt.die, DW_AT_decl_file)) {
      handled = handleSubProgram(ctxt, ctxt.die);
    }

    return handled;
  }
  static bool handleDuplicate(Context &ctxt)
  {
    auto rtn = ctxt.getRoutine(ctxt.duplicate);
    if (rtn) {
      ctxt.currentRoutine.push(rtn);
      return false;  // continue
    } else {
      return true;  // stop traversing
    }
  }
};

template<>
struct TagLeaver<DW_TAG_subprogram> {
  static void leave(Context &ctxt)
  {
    if (isValid(ctxt, ctxt.die) && hasAttr(ctxt.die, DW_AT_decl_file))
      ctxt.currentRoutine.pop();
  }

  static void leaveDuplicate(Context &ctxt)
  {
    auto rtn = ctxt.getRoutine(ctxt.duplicate);
    if (rtn != nullptr) {
      ctxt.currentRoutine.pop();
    }
  }
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_TAGSUBPROGRAM_H
