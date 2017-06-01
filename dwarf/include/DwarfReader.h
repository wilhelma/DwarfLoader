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

#include "Context.h"
#include "guards/FileGuard.h"
#include "guards/DbgGuard.h"
#include "Duplicate.h"
#include "Filter.h"

namespace pcv {
namespace dwarf {

using pcv::entity::Class;
using pcv::entity::Routine;

/*!
 * @brief DwarfReader is responsible for reading dwarf information and providing an API for
 * providing symbol information.
 */
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

  // processing
  void iterateCUs();
  void iterate(Dwarf_Die die) noexcept;
  bool handle(Dwarf_Die die);
  void leave(Dwarf_Die die);

  // post-processing
  void processContext();

  // members
  const Filter &filter_;
  DieDuplicate dieDuplicate_;
  std::unique_ptr<FileGuard> fileGuard_;
  std::unique_ptr<DbgGuard> dbgGuard_;
  Context ctxt_;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_DWARFREADER_H
