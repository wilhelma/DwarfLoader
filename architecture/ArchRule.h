//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_COMMAND_H
#define DWARFLOADER_COMMAND_H

namespace pcv {
namespace dwarf {

class Context;
struct ArchSet_t;

class ArchRule {
 public:
  ArchRule() = default;
  virtual ~ArchRule() = default;

  virtual void execute(ArchSet_t &archSet, const Context& ctxt) = 0;
  virtual void append(ArchSet_t &archSet, const Context& ctxt) = 0;

  /* disable copy/move construction and assignment operators */
  ArchRule(const ArchRule &) = delete;
  ArchRule(ArchRule &&) = delete;
  ArchRule &operator=(const ArchRule &) = delete;
  ArchRule &operator=(ArchRule &&) = delete;
};

}  // namespace dwarf
}  // namespace pcv


#endif //DWARFLOADER_COMMAND_H
