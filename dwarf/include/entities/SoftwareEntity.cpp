//
// Created by Faris Cakaric on 31.08.17.
//

#include "SoftwareEntity.h"
#include "Namespace.h"

namespace pcv {
  namespace entity {
    SoftwareEntity::SoftwareEntity(Id_t id, name_t name, Image *img, Namespace *nmsp, Class *cls, file_t file,
                                   line_t line) : id(id), name(std::move(name)), img(img), nmsp(nmsp), cls(cls), file(std::move(file)), line(line)
    {
      if(nmsp!= nullptr)
        nmsp->entities.push_back(this);
      if(img != nullptr)
        img->entities.push_back(this);
    }
  }
}

