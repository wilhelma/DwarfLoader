//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_CLASS_H
#define DWARFLOADER_CLASS_H

#include <vector>

#include "SoftwareEntity.h"

namespace pcv {
namespace dwarf {

struct Routine;
struct Variable;

struct Class : public SoftwareEntity {
  std::vector<Routine *> methods;
  std::vector<Variable *> members;
  std::vector<Class *> baseClasses;
  std::vector<Class *> inheritClasses;
  std::vector<Class *> composites;

  explicit Class(std::string name,
                 Image &img,
                 Namespace &nmsp,
                 const Class *cls,
                 std::string file,
                 int line,
                 Dwarf_Off offset)
      : SoftwareEntity(name, img, nmsp, cls, file, line, offset)
  {
    img.entities.push_back(this);
  }

  EntityType getEntityType() const override { return EntityType::Class; }
};

}  // namespace dwarf
}  // namespace pcv
#endif //DWARFLOADER_CLASS_H
