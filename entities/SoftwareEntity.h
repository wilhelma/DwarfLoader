//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_SOFTWAREENTITY_H
#define DWARFLOADER_SOFTWAREENTITY_H

#include <string>
#include <utility>
#include <libdwarf.h>
#include "./Namespace.h"
#include "./Image.h"

namespace pcv {
namespace dwarf {

/// @brief The type of a software entity.
enum class EntityType : int
{
  None      = 0,
  Class     = 1 << 0,
  Routine   = 1 << 1,
  Variable  = 1 << 2,
  All       = Class | Routine | Variable
};

inline constexpr EntityType
operator&(EntityType  __x, EntityType  __y)
{
  return static_cast<EntityType>(static_cast<int>(__x) & static_cast<int>(__y));
}

inline constexpr EntityType
operator|(EntityType  __x, EntityType  __y)
{
  return static_cast<EntityType>(static_cast<int>(__x) | static_cast<int>(__y));
}

struct Class;
struct Artifact_t;

/// @brief The base structure of a software entitiy.
struct SoftwareEntity {
  std::string name;
  Image &img;
  Namespace &nmsp;
  const Class *cls;
  std::string file;
  int line;
  Dwarf_Off offset;

  /// @brief Constructor.
  /// @param name The name of the entity.
  /// @param img The containing image.
  /// @param nmsp The containing namespace.
  /// @param file The name of the containing file.
  /// @param line The line of the declaration.
  /// @param offset The cu offset of the dwarf information.
  explicit SoftwareEntity(std::string name, Image &img, Namespace &nmsp, const Class *cls,
                          std::string file, int line, Dwarf_Off off)
      : name(std::move(name)), img(img), nmsp(nmsp), cls(cls), file(std::move(file)), line(line),
        offset(off)
  {
    nmsp.entities.push_back(this);
    img.entities.push_back(this);
  }

  /// @brief Destructor.
  virtual ~SoftwareEntity() = default;

  /*!
   * @brief Returns the type of the concrete software entity.
   * @return The EntityType.
   */
  virtual EntityType getEntityType() const = 0;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_SOFTWAREENTITY_H
