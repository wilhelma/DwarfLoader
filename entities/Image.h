//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_IMAGE_H
#define DWARFLOADER_IMAGE_H

#include <string>

namespace pcv {
namespace dwarf {

struct SoftwareEntity;

struct Image {
  std::string name;
  std::vector<SoftwareEntity*> entities;

  explicit Image(std::string name) : name(name) {}
};

}
}

#endif //DWARFLOADER_IMAGE_H
