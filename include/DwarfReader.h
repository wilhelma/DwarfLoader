//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_DWARFREADER_H
#define DWARFLOADER_DWARFREADER_H

#include <libdwarf.h>
#include <dwarf.h>
#include <memory>
#include <type_traits>
#include <iostream>
#include <cstdlib>
#include <string>
#include <bits/unique_ptr.h>

#include "./Context.h"
#include "../guards/FileGuard.h"
#include "../guards/DbgGuard.h"

namespace pcv {
namespace dwarf {

class DwarfReader {
 public:
  explicit DwarfReader(const std::string &fileName);
  ~DwarfReader() = default;

  /* disable copy/move construction and assignment operators */
  DwarfReader(const DwarfReader &) = delete;
  DwarfReader(DwarfReader &&) = delete;
  DwarfReader &operator=(const DwarfReader &) = delete;
  DwarfReader &operator=(DwarfReader &&) = delete;

  /* the global error handler (required to deallocate error messages) */
  static void errHandler(Dwarf_Error err, Dwarf_Ptr errArg);

  void start();
  inline Context& getContext() {
    return ctxt_;
  }

 private:
  // processing
  void iterateCUs();
  void inspectDie(Dwarf_Die /* die */) noexcept;
  bool handleDie(Dwarf_Die /* die */);
  void leaveDie(Dwarf_Die /* die */);

  // post-processing
  void processContext();

  static constexpr Dwarf_Bool IS_INFO { true };

  std::unique_ptr<FileGuard> fileGuard_;
  std::unique_ptr<DbgGuard> dbgGuard_;
  Context ctxt_;
  std::vector<std::string> srcFiles_;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_DWARFREADER_H
