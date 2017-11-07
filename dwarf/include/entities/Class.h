//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_CLASS_H
#define DWARFLOADER_CLASS_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <climits>

#include "SoftwareEntity.h"

namespace pcv {
namespace entity {

struct Routine;
struct Variable;

struct Class : public SoftwareEntity {
  static constexpr Id_t NO_CLASS = UINT_MAX;  // no class constant

  std::vector<Routine *> methods;
  std::vector<Variable *> members;
  std::vector<Class *> baseClasses;
  std::vector<Class *> inheritClasses;
  std::vector<Class *> nestedClasses;
  std::vector<Class *> composites;

  explicit Class(Id_t id,
                 name_t name,
                 Image *img,
                 Namespace *nmsp,
                 Class *cls,
                 file_t file,
                 line_t line,
                 size_t size)
    : SoftwareEntity(id, name, img, nmsp, cls, file, line) {
    this->size = size;
    img->entities.push_back(this);
  }

  EntityType getEntityType() const override { return EntityType::Class; }
};

}  // namespace entity
}  // namespace pcv
#endif //DWARFLOADER_CLASS_H
