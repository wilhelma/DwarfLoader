//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_SOFTWAREENTITY_H
#define DWARFLOADER_SOFTWAREENTITY_H

#include <string>
#include <utility>
#include <libdwarf.h>
#include "./Namespace.h"
#include "./Image.h"

namespace pcv {
namespace dwarf {

enum class EntityType { Class, Routine, Variable };

struct SoftwareEntity {
  std::string name;
  Image &img;
  Namespace &nmsp;
  std::string file;
  int line;
  Dwarf_Off offset;

  explicit SoftwareEntity(std::string name,
                          Image &img,
                          Namespace &nmsp,
                          std::string file,
                          int line,
                          Dwarf_Off offset)
      : name(std::move(name)), img(img), nmsp(nmsp), file(std::move(file)), line(line),
        offset(offset)
  {
    nmsp.entities.push_back(this);
    img.entities.push_back(this);
  }

  virtual ~SoftwareEntity() = default;

  virtual EntityType getEntityType() const = 0;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_SOFTWAREENTITY_H
