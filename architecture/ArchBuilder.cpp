//
// Created by wilhelma on 1/10/17.
//

#include "./ArchBuilder.h"
#include "./ArchRule.h"
#include <json11.hpp>

using json11::Json;

namespace pcv {
namespace dwarf {

void ArchBuilder::apply(ArchRule &rule)
{
  rule.execute(archSet_, ctxt_);
}

void ArchBuilder::append(ArchRule &rule)
{
  rule.append(archSet_, ctxt_);
}

void addArtifact(Json::array* entities, const ArchSet_t *set, const std::string& artifactName = "")
{
  for (auto &entity : set->entities) {
    entities->emplace_back(
        Json::object {
            { "id", artifactName + entity->name },
            { "Id", static_cast<int>(entity->offset) },
            { "Type", static_cast<int>(entity->getEntityType()) },
            { "Name", entity->name },
            { "Image", entity->img.name },
            { "Namespace", entity->nmsp.name },
            { "File", entity->file },
            { "Line", entity->line },
        }
    );
  }

  for (auto &child : set->children) {
    auto childObjects = Json::array {};
    addArtifact(&childObjects, child.get(), artifactName + "." + child->name);

    entities->emplace_back(
      Json::object {
          { "id", child->name },
          { "children", childObjects },
      }
    );
  }
}

std::ostream& operator<<(std::ostream& os, const ArchBuilder& obj)
{
  Json::array childs{};
  addArtifact(&childs, &obj.archSet_);

  Json::array links{};

  auto sets = Json::object {
      { "id", "root" },
      { "children", childs },
      { "links", links },
  };

  os << Json(sets).dump();
  return os;
}

}  // namespace dwarf
}  // namespace pcv
