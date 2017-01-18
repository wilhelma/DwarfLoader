//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_ARCHBUILDER_H
#define DWARFLOADER_ARCHBUILDER_H

#include <vector>
#include <unordered_set>
#include <memory>

#include "../entities/SoftwareEntity.h"

namespace pcv {
namespace dwarf {

class Context;
class ArchRule;

typedef struct ArchSet_t {
  std::string name;
  std::unordered_set<const SoftwareEntity*> entities;
  std::vector<std::unique_ptr<ArchSet_t>> children;
  const ArchSet_t* parent;

  explicit ArchSet_t(const std::string& name, const ArchSet_t* parent)
      : name(name), parent(parent) {}
} ArchSet_t;

class ArchBuilder {
 public:
  explicit ArchBuilder(const Context &ctxt) : ctxt_(ctxt), archSet_("default", nullptr) {}
  ~ArchBuilder() = default;

  /* disable copy/move construction and assignment operators */
  ArchBuilder(const ArchBuilder &) = delete;
  ArchBuilder(ArchBuilder &&) = delete;
  ArchBuilder &operator=(const ArchBuilder &) = delete;
  ArchBuilder &operator=(ArchBuilder &&) = delete;

  void apply(ArchRule &rule);
  void append(ArchRule &rule);
  friend std::ostream& operator<<(std::ostream& os, const ArchBuilder& obj);

 private:
  const Context &ctxt_;
  ArchSet_t archSet_;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_ARCHBUILDER_H
