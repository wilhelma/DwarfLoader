//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_NAMESPACE_H
#define DWARFLOADER_NAMESPACE_H

#include <string>
#include <vector>

namespace pcv {
namespace dwarf {

struct SoftwareEntity;

struct Namespace {
    std::string name;
    Namespace *parent;
    std::vector<Namespace*> children;
    std::vector<SoftwareEntity*> entities;

    Namespace(std::string &&name, Namespace *parent) : name(name), parent(parent) {}
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_NAMESPACE_H
