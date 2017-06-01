//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_VARIABLE_H
#define DWARFLOADER_VARIABLE_H

#include "SoftwareEntity.h"

#include <string>

namespace pcv {
namespace entity {

struct Variable : public SoftwareEntity {
  enum class Type : uint8_t {
    STACK     = 1,
    HEAP      = 2,
    STATICVAR = 3,
    GLOBAL    = 4,
    UNKNOWN   = 5,
  };

  offset_t offset;
  Type type;
  const Class* classType;

  explicit Variable(Id_t id,
                    std::string name,
                    Image *img,
                    Namespace *nmsp,
                    Class *cls,
                    std::string file,
                    int line,
                    size_t size,
                    offset_t offset,
                    Type type,
                    const Class* classType = nullptr)
    : SoftwareEntity(id, name, img, nmsp, cls, file, line), offset(offset), type(type),
      classType(classType)
  {
    this->size = size;
  }

  EntityType getEntityType() const override { return EntityType::Variable; }
};

}  // namespace entity
}  // namespace pcv

#endif //DWARFLOADER_VARIABLE_H
