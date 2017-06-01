//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_IMAGE_H
#define DWARFLOADER_IMAGE_H

#include <string>
#include <vector>

namespace pcv {
namespace entity {

struct SoftwareEntity;

struct Image {
  std::string name;
  std::vector<SoftwareEntity *> entities;

  explicit Image(std::string name) : name(name) {}
};

}  // namespace entity
}  // namespace pcv

#endif  //DWARFLOADER_IMAGE_H
