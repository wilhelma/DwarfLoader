//
// Created by wilhelma on 12/23/16.
//

#ifndef DWARFLOADER_TAGCOMPILEUNIT_H
#define DWARFLOADER_TAGCOMPILEUNIT_H

#include <libdwarf.h>
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
    int lastSlash = pathStr.find("/../");
    int firstSlash = pathStr.rfind("/", lastSlash - 1);
    pathStr.erase(firstSlash, lastSlash + 3 - firstSlash);
  }

  return pathStr;
};

void parseSourceLocationInfo(pcv::dwarf::Context &ctxt)
{
  Dwarf_Unsigned version{0};
  Dwarf_Small count{0};
  Dwarf_Line_Context context{0};

  if (dwarf_srclines_b(ctxt.die, &version, &count, &context, nullptr) != DW_DLV_OK)
    throw DwarfError("dwarf_srclines_b");

  if (count == 1) {
    Dwarf_Line *lineBuf;
    Dwarf_Signed lineCount{0};

    if (dwarf_srclines_from_linecontext(context, &lineBuf, &lineCount, nullptr) != DW_DLV_OK)
      throw DwarfError("dwarf_srclines_from_linecontext");

    for (auto i = 0; i < lineCount; ++i) {
      Dwarf_Addr lineAddr{0};
      Dwarf_Unsigned lineNo{0}, fileNo{0};

      if (dwarf_lineaddr(lineBuf[i], &lineAddr, nullptr) != DW_DLV_OK)
        throw DwarfError("dwarf_lineaddr");

      if (dwarf_lineno(lineBuf[i], &lineNo, nullptr) != DW_DLV_OK)
        throw DwarfError("dwarf_lineno");

      if (dwarf_line_srcfileno(lineBuf[i], &fileNo, nullptr) != DW_DLV_OK)
        throw DwarfError("dwarf_line_srcfileno");

      ctxt.addSourceLocation(lineAddr, std::unique_ptr<SourceLocation> {
        new SourceLocation(lineNo, ctxt.srcFiles[fileNo - 1])
      });
    }
  }

  dwarf_srclines_dealloc_b(context);
}


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

    parseSourceLocationInfo(ctxt);

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
