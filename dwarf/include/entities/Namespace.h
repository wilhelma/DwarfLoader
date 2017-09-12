//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_NAMESPACE_H
#define DWARFLOADER_NAMESPACE_H

#include <string>
#include <vector>

#include "SoftwareEntity.h"

namespace pcv {
namespace entity {

struct Namespace : public SoftwareEntity {
    std::string name;
    Namespace *parent;
    std::vector<Namespace*> children;
    std::vector<SoftwareEntity*> entities;

    Namespace(std::string &&name, Namespace *parent) : SoftwareEntity(), name(name), parent(parent) {}

    EntityType getEntityType() const override { return EntityType::Namespace; }
};

}  // namespace pcv
}  // namespace entity

#endif //DWARFLOADER_NAMESPACE_H
