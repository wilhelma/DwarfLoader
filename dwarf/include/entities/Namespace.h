//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_NAMESPACE_H
#define DWARFLOADER_NAMESPACE_H

#include <string>
#include <vector>

#include "SoftwareEntity.h"

#include <iostream>

namespace pcv {
  namespace entity {

    struct Namespace : public SoftwareEntity {
      Namespace *parent;
      std::vector<Namespace *> children;
      std::vector<SoftwareEntity *> entities;

      Namespace(Id_t id,
                name_t name,
                Image *img,
                Namespace *nmsp,
                file_t file,
                line_t line,
                Namespace *parent)
              : SoftwareEntity(id, name, img, nmsp, nullptr, file, line), parent(parent) {}

      EntityType getEntityType() const override { return EntityType::Namespace; }
    };

  }  // namespace pcv
}  // namespace entity

#endif //DWARFLOADER_NAMESPACE_H
