//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_ROUTINE_H
#define DWARFLOADER_ROUTINE_H

#include <utility>
#include <unordered_map>
#include <string>

#include "SoftwareEntity.h"

namespace pcv {
namespace entity {

struct Variable;

struct Routine : public SoftwareEntity {
  bool isConstructor;
  std::vector<Variable*> locals;

  explicit Routine(Id_t id,
                   std::string name,
                   Image *img,
                   Namespace *nmsp,
                   Class *cls,
                   std::string file,
                   int line)
    : SoftwareEntity(id, name, img, nmsp, cls, file, line), isConstructor(false) {}

  EntityType getEntityType() const override { return EntityType::Routine; }
};

}  // namespace entity
}  // namespace pcv

#endif //DWARFLOADER_ROUTINE_H
