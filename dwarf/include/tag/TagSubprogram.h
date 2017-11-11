//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGSUBPROGRAM_H
#define DWARFLOADER_TAGSUBPROGRAM_H

#include <cstring>

#include "tag/TagGeneric.h"
#include "entities/Routine.h"
#include "Type.h"
#include "DwarfReader.h"
#include "Context.h"

namespace {

static std::unordered_map<Dwarf_Off, pcv::dwarf::Routine*> handled;

bool isValid(const Context &ctxt, Dwarf_Die die)
{
  return !(ctxt.currentClass.empty() && pcv::dwarf::hasAttr(die, DW_AT_artificial));
}

}

namespace pcv {
namespace dwarf {

pcv::entity::Routine* getSpecRoutine(const Context &ctxt, Dwarf_Die die)
{
  Dwarf_Off specOff{};
  Dwarf_Die specDie{0};

  // specification
  if (hasAttr(die, DW_AT_declaration))
    specDie = die;

  // declaration
  else if (hasAttr(die, DW_AT_specification))
    specDie = jump(ctxt.dbg, die, DW_AT_specification);

  // abstract origin
  else if (hasAttr(die, DW_AT_abstract_origin)) {
    auto declDie = jump(ctxt.dbg, die, DW_AT_abstract_origin);
    if (hasAttr(declDie, DW_AT_specification)) {
      specDie = jump(ctxt.dbg, declDie, DW_AT_specification);
    }
    dwarf_dealloc(ctxt.dbg, declDie, DW_DLA_DIE);
  }

  if (!specDie) return nullptr;

  dwarf_dieoffset(specDie, &specOff, nullptr);
  if (!hasAttr(die, DW_AT_declaration)) dwarf_dealloc(ctxt.dbg, specDie, DW_DLA_DIE);
  return ctxt.get<Routine>(specOff);
}


bool inspectConstructors(Context &ctxt, Dwarf_Die die)
{
  char *rtnName = nullptr;
  if (getDieName(ctxt.dbg, die, &rtnName)) {
    std::string uName = demangleNameOnly(rtnName);
    size_t pos2{uName.rfind("::")};
    if (pos2 != std::string::npos) {
      size_t pos1{uName.rfind("::", pos2 - 1)};
      if (pos1 != std::string::npos) pos1 += 2;
      else pos1 = 0;

      std::string className = uName.substr(pos1, pos2 - pos1);
      auto pos = className.find_first_of('<');
      if (pos != std::string::npos)
        className = className.substr(0, pos);

      const std::string methodName = uName.substr(pos2 + 2);

      if (className == methodName) {
        if (hasAttr(die, DW_AT_abstract_origin)) {
          auto rtn = getSpecRoutine(ctxt, die);
          if (rtn != nullptr) {
            rtn->isConstructor = true;
            ctxt.linkNameToRoutine(rtnName, rtn);
          }

          return true;
        }
      }
    }
  }
  return false;
}

bool handleSubProgram(Context &ctxt, Dwarf_Die die, Dwarf_Off off = 0)
{
  if (isValid(ctxt, die)) {
    if (off) {
      Dwarf_Off specOff;
      if (dwarf_dieoffset(die, &specOff, nullptr) != DW_DLV_OK) throw DwarfError("offset");
      if (handled.find(specOff) != std::end(handled)) {
        ctxt.currentRoutine.emplace(handled[specOff]);
        ctxt.toClean.insert(ctxt.die);
        return false;  // continue
      }
    }

    char *rtnName{nullptr};
    if (!getDieName(ctxt.dbg, die, &rtnName)) throw DwarfError("diename");

    // handle constructors
    bool isConstructor {!ctxt.currentClass.empty() && (ctxt.currentClass.back()->name == rtnName)};
    if (std::strstr(rtnName, "C1") != nullptr ||
      std::strstr(rtnName, "C2") != nullptr ||
      std::strstr(rtnName, "C3") != nullptr) {
      inspectConstructors(ctxt, die);
    }

    if (!off && (dwarf_dieoffset(die, &off, nullptr) != DW_DLV_OK)) throw DwarfError("offset");

    Dwarf_Unsigned fileNo{0}, lineNo{0};
    std::string file{};
    if (hasAttr(die, DW_AT_decl_file)) {
      getAttrUint(ctxt.dbg, die, DW_AT_decl_file, &fileNo);
      file = ctxt.srcFiles[fileNo - 1];
      getAttrUint(ctxt.dbg, die, DW_AT_decl_line, &lineNo);
    }

    if (lineNo > 0 || isConstructor) {
      auto cls = ctxt.currentClass.empty() ? nullptr : ctxt.currentClass.back();
      if (cls == nullptr) {
        auto specRtn = getSpecRoutine(ctxt, die);
        if (specRtn != nullptr)
          cls = specRtn->cls;
      }

      auto rtn = std::unique_ptr<Routine> {
        new Routine(off,
                    demangle(rtnName),
                    ctxt.currentImage,
                    ctxt.currentNamespace,
                    cls,
                    file,
                    lineNo,
                    isConstructor)};

      ctxt.addRoutine(off, std::move(rtn));
      handled[off] = ctxt.routines.back().get();

      if (!ctxt.currentClass.empty())
        ctxt.currentClass.back()->methods.push_back(ctxt.routines.back().get());

      ctxt.currentRoutine.emplace(ctxt.routines.back().get());
      ctxt.toClean.insert(ctxt.die);

      return false; // continue
    }
  }
  return true; // stop
}

template<>
struct TagHandler<DW_TAG_subprogram> {
  static bool handle(Context &ctxt)
  {
    bool handled = false;

    if (hasAttr(ctxt.die, DW_AT_specification)) {
      Dwarf_Off off;
      auto specDie = jump(ctxt.dbg, ctxt.die, DW_AT_specification);
      if (dwarf_dieoffset(ctxt.die, &off, 0) == DW_DLV_OK)
        handled = handleSubProgram(ctxt, specDie, off);
      dwarf_dealloc(ctxt.dbg, specDie, DW_DLA_DIE);
    } else {
      handled = handleSubProgram(ctxt, ctxt.die);
    }

    return handled;
  }
  static bool handleDuplicate(Context &ctxt)
  {
    auto rtn = ctxt.get<Routine>(ctxt.duplicate);
    if (rtn) {
      Dwarf_Off off{};
      if (dwarf_dieoffset(ctxt.die, &off, nullptr) != DW_DLV_OK) throw DwarfError("dieoffset");
      handled[off] = rtn;
      ctxt.currentRoutine.emplace(rtn);
      ctxt.toClean.insert(ctxt.die);
    }

    return false;  // continue
  }
};

void leaveRoutine(Context &ctxt)
{
  auto it = ctxt.toClean.find(ctxt.die);
  if (it != std::end(ctxt.toClean)) {
    ctxt.currentRoutine.pop();
    ctxt.toClean.erase(it);
  }
}

template<>
struct TagLeaver<DW_TAG_subprogram> {
  static void leave(Context &ctxt)
  {
    leaveRoutine(ctxt);
  }

  static void leaveDuplicate(Context &ctxt)
  {
    leaveRoutine(ctxt);
  }
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_TAGSUBPROGRAM_H
