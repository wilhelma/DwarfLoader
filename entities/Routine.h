//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_ROUTINE_H
#define DWARFLOADER_ROUTINE_H

#include <utility>

#include "SoftwareEntity.h"

namespace pcv {
namespace dwarf {

struct Routine : public SoftwareEntity {
  explicit Routine(std::string name,
                   Image &img,
                   Namespace &nmsp,
                   std::string file,
                   int line,
                   Dwarf_Off offset)
    : SoftwareEntity (name, img, nmsp, file, line, offset) {}

  EntityType getEntityType() const override { return EntityType::Routine; }
};

}
}

#endif //DWARFLOADER_ROUTINE_H
