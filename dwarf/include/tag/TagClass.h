//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGCLASS_H
#define DWARFLOADER_TAGCLASS_H

#include "./TagGeneric.h"

namespace pcv {
namespace dwarf {

bool isValid(const Context& ctxt)
{
  Dwarf_Unsigned lineNo{};
  getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_line, &lineNo);
  if (lineNo == 0)
    return false;

  return true;
}

bool handleStructClass(Context &ctxt)
{
  Dwarf_Off off{};
  if (dwarf_dieoffset(ctxt.die, &off, nullptr) != DW_DLV_OK) throw DwarfError("offset");

  if (hasAttr(ctxt.die, DW_AT_decl_file)) {
    char *clsChar{nullptr};
    std::string clsString{};

    if (getDieName(ctxt.dbg, ctxt.die, &clsChar))
      clsString = std::string(clsChar);
    else
      clsString = std::string("unnamed");

    Dwarf_Off off{};
    Dwarf_Unsigned fileNo{}, lineNo{}, size{};

    if (dwarf_dieoffset(ctxt.die, &off, nullptr) != DW_DLV_OK) throw DwarfError("offset");
    getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_file, &fileNo);
    getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_line, &lineNo);
    getAttrUint(ctxt.dbg, ctxt.die, DW_AT_byte_size, &size);

    auto cls = ctxt.getClass(*ctxt.currentNamespace, clsString);
    if (cls == nullptr) {
      // first occurrence of this class in the dwarf file
      ctxt.classes.emplace_back(
         std::unique_ptr<Class>{
            new Class(off,
                      clsString,
                      ctxt.currentImage,
                      ctxt.currentNamespace,
                      ctxt.currentClass.empty() ? nullptr : ctxt.currentClass.top(),
                      ctxt.srcFiles[fileNo - 1],
                      lineNo,
                      size)
         });

      ctxt.addClass(off, ctxt.classes.back().get());
    } else {
      // there where already declarations of this class in the dwarf file
      cls->cls = ctxt.currentClass.empty() ? nullptr : ctxt.currentClass.top();
      cls->file = ctxt.srcFiles[fileNo - 1];
      cls->line = lineNo;
      cls->size = size;
      ctxt.addClass(off, cls);
    }
  } else {
    char *clsChar{nullptr};
    std::string clsString{};

    if (!getDieName(ctxt.dbg, ctxt.die, &clsChar)) throw DwarfError("dieName");
    clsString = std::string(clsChar);

    auto cls = ctxt.getClass(*ctxt.currentNamespace, clsString);
    if (cls == nullptr) {
      // first occurrence of this class
      ctxt.classes.emplace_back(
         std::unique_ptr<Class>{
           new Class(off,
                     clsString,
                     ctxt.currentImage,
                     ctxt.currentNamespace,
                     nullptr,  // do set base class on definition
                     "",  // do set file on definition
                     0,
                     0)
       });

      ctxt.addClass(off, ctxt.classes.back().get());
    } else {
      // there has been a occurence (declarations or definitions) already
      ctxt.addClass(off, cls);
    }
  }

  return false; // continue
}

bool handleStructClassDuplicate(Context &ctxt)
{
  Dwarf_Off off;
  if (dwarf_dieoffset(ctxt.die, &off, nullptr) != DW_DLV_OK) throw DwarfError("offset");

  auto cls = ctxt.getClass(ctxt.duplicate);
  if (cls) ctxt.addClass(off, cls);

  return false;
}

template<>
struct TagHandler<DW_TAG_class_type> {
  static bool handle(Context &ctxt)
  {
    return handleStructClass(ctxt);
  }

  static bool handleDuplicate(Context &ctxt)
  {
    return handleStructClassDuplicate(ctxt);
  }
};

template<>
struct TagHandler<DW_TAG_structure_type> {
  static bool handle(Context &ctxt)
  {
    if (hasAttr(ctxt.die, DW_AT_decl_file) && !isValid(ctxt))
      return true;

    return handleStructClass(ctxt);
  }

  static bool handleDuplicate(Context &ctxt)
  {
    if (hasAttr(ctxt.die, DW_AT_decl_file) && !isValid(ctxt))
      return true;

    return handleStructClassDuplicate(ctxt);
  }
};

template<>
struct TagLeaver<DW_TAG_class_type> {
  static void leave(Context &ctxt)
  {
    if (hasAttr(ctxt.die, DW_AT_decl_file) && !isValid(ctxt))
        return;

    ctxt.currentClass.pop();
  }
  static void leaveDuplicate(Context &ctxt)
  {
    if (hasAttr(ctxt.die, DW_AT_decl_file) && !isValid(ctxt))
      return;

    ctxt.currentClass.pop();
  }
};

template<>
struct TagLeaver<DW_TAG_structure_type> {
  static void leave(Context &ctxt)
  {
    if (hasAttr(ctxt.die, DW_AT_decl_file) && !isValid(ctxt))
      return;

    ctxt.currentClass.pop();
  }
  static void leaveDuplicate(Context &ctxt)
  {
    if (hasAttr(ctxt.die, DW_AT_decl_file) && !isValid(ctxt))
      return;

    ctxt.currentClass.pop();
  }
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_TAGNAMESPACE_H
