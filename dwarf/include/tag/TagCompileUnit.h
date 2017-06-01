//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGCOMPILEUNIT_H
#define DWARFLOADER_TAGCOMPILEUNIT_H

#include "tag/TagGeneric.h"
#include "DwarfHelper.h"

namespace pcv {
namespace dwarf {

static std::string fixPath(const char *path)
{
  std::string pathStr(path);

  while (pathStr.find("/./") != std::string::npos)
    pathStr.erase(pathStr.find("/./") + 1, 2);

  while (pathStr.find("/../") != std::string::npos) {
    auto lastSlash = pathStr.find("/../");
    auto firstSlash = pathStr.rfind("/", lastSlash - 1);
    pathStr.erase(firstSlash, lastSlash + 3 - firstSlash);
  }

  return pathStr;
};

template<>
struct TagHandler<DW_TAG_compile_unit> {
  static bool handle(pcv::dwarf::Context &ctxt)
  {
    {  // register image
      char *name;
      if (!getDieName(ctxt.dbg, ctxt.die, &name)) throw DwarfError("die name");
      ctxt.images.emplace_back(
          std::unique_ptr<Image>{new Image(name)}
      );

      ctxt.currentImage = ctxt.images.back().get();
    }

    {  // collect src files of cu
      char **srcfiles;
      Dwarf_Signed cnt;

      if (dwarf_srcfiles(ctxt.die, &srcfiles, &cnt, nullptr) == DW_DLV_OK) {
        for (int i = 0; i < cnt; ++i) {
          ctxt.srcFiles.emplace_back(fixPath(srcfiles[i]));
          dwarf_dealloc(ctxt.dbg, srcfiles[i], DW_DLA_STRING);
        }
        dwarf_dealloc(ctxt.dbg, srcfiles, DW_DLA_LIST);
      }
    }

    return false;  // continue
  }
  static bool handleDuplicate(Context &ctxt)
  {
    return false;
  }
};

template<>
struct TagLeaver<DW_TAG_compile_unit> {
  static void leave(Context &ctxt)
  {
    ctxt.srcFiles.clear();
  }
  static void leaveDuplicate(Context &ctxt)
  {
    ctxt.srcFiles.clear();
  }
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_TAGCOMPILEUNIT_H
