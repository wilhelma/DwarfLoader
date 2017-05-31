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
#include "Duplicate.h"

namespace pcv {
namespace dwarf {

class Filter;

class DwarfReader {
 public:
  explicit DwarfReader(const std::string &fileName,
                       DieDuplicate &dieDuplicate,
                       const Filter &filter);
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
  static constexpr Dwarf_Bool IS_INFO { true };
  DieDuplicate &dieDuplicate_;
  const Filter &filter_;

  // processing
  void iterateCUs();
  void inspectDie(Dwarf_Die /* die */) noexcept;
  bool handleDie(Dwarf_Die /* die */);
  void leaveDie(Dwarf_Die /* die */);

  // post-processing
  void processContext();

  std::unique_ptr<FileGuard> fileGuard_;
  std::unique_ptr<DbgGuard> dbgGuard_;
  Context ctxt_;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_DWARFREADER_H
