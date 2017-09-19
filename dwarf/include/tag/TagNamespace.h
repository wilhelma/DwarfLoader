//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGNAMESPACE_H
#define DWARFLOADER_TAGNAMESPACE_H

#include "./TagGeneric.h"

namespace pcv {
namespace dwarf {

std::string getNamespaceString(const std::string &nmspString, Namespace *nmsp)
{
  std::string s{nmspString};
  if (nmsp != nullptr) {
    s.append(std::string("::") + (nmsp->name.empty() ? "<>" : nmsp->name));
    for (Namespace *parent = nmsp->parent; parent != nullptr; parent = parent->parent) {
      s.append(std::string("::" + parent->name));
    }
  }
  return s;
}

bool handleNamespace(Context &ctxt)
{
  static std::unordered_map<std::string, Namespace*> addedNamespaces;
  char *nmspChar;

  if (getDieName(ctxt.dbg, ctxt.die, &nmspChar)) {
    auto nmsp = getNamespaceString(nmspChar, ctxt.currentNamespace);
    Dwarf_Off off{};
    Dwarf_Unsigned fileNo{}, lineNo{}, size{};
    if (dwarf_dieoffset(ctxt.die, &off, nullptr) != DW_DLV_OK) throw DwarfError("offset");
    getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_file, &fileNo);
    getAttrUint(ctxt.dbg, ctxt.die, DW_AT_decl_line, &lineNo);

    if (addedNamespaces.find(nmsp) == end(addedNamespaces)) {
      ctxt.namespaces.emplace_back(std::unique_ptr<Namespace>{
          new Namespace(off, std::string(nmspChar),
                        ctxt.currentImage,
                        ctxt.currentNamespace, ctxt.srcFiles[fileNo - 1],
                        lineNo,
                        (ctxt.currentNamespace == ctxt.emptyNamespace) ? nullptr
                                                                       : ctxt.currentNamespace)
      });
      auto pNewNmsp = ctxt.namespaces.back().get();
      pNewNmsp->parent = ctxt.currentNamespace;
      ctxt.currentNamespace->children.emplace_back(pNewNmsp);
      addedNamespaces[nmsp] = pNewNmsp;
    }

    ctxt.currentNamespace = addedNamespaces[nmsp];
  }

  return false; // continue
}

template<>
struct TagHandler<DW_TAG_namespace> {
  static bool handle(Context &ctxt)
  {
    return handleNamespace(ctxt);
  }
  static bool handleDuplicate(Context &ctxt)
  {
    return handleNamespace(ctxt);
  }
};

template<>
struct TagLeaver<DW_TAG_namespace> {
  static void leave(Context &ctxt)
  {
    if (hasAttr(ctxt.die, DW_AT_name)) {
      ctxt.currentNamespace = (ctxt.currentNamespace->parent == nullptr)
                              ? ctxt.emptyNamespace : ctxt.currentNamespace->parent;
    }
  }
  static void leaveDuplicate(Context &ctxt)
  {
    if (hasAttr(ctxt.die, DW_AT_name)) {
      ctxt.currentNamespace = (ctxt.currentNamespace->parent == nullptr)
                              ? ctxt.emptyNamespace : ctxt.currentNamespace->parent;
    }
  }
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_TAGNAMESPACE_H
