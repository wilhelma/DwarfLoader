//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_VARIABLE_H
#define DWARFLOADER_VARIABLE_H

#include "SoftwareEntity.h"

namespace pcv {
namespace dwarf {

struct Variable : public SoftwareEntity {
  explicit Variable(std::string name,
                    Image &img,
                    Namespace &nmsp,
                    const Class *cls,
                    std::string file,
                    int line,
                    Dwarf_Off offset)
      : SoftwareEntity(name, img, nmsp, cls, file, line, offset) {}

  EntityType getEntityType() const override { return EntityType::Variable; }
};

}
}

#endif //DWARFLOADER_VARIABLE_H
