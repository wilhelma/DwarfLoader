//
// Created by wilhelma on 12/22/16.
//

#ifndef DWARFLOADER_SOFTWAREENTITY_H
#define DWARFLOADER_SOFTWAREENTITY_H

#include <string>
#include <utility>
#include <libdwarf.h>
#include "./Image.h"

namespace pcv {
namespace entity {

using Id_t = std::conditional<true, Dwarf_Off, int>::type;

/// @brief The type of a software entity.
enum class EntityType : int
{
  None      = 0,
  Class     = 1 << 0,
  Routine   = 1 << 1,
  Variable  = 1 << 2,
  Namespace = 1 << 3,
  All       = Class | Routine | Variable | Namespace
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
  struct  Namespace;

/// @brief The base structure of a software entitiy.
struct SoftwareEntity {
  using name_t = std::string;
  using size_t = uint64_t;
  using file_t = std::string;
  using line_t = uint32_t;
  using offset_t = int64_t;

  Id_t id;
  name_t name;
  size_t size{};
  Image *img;
  Namespace *nmsp;
  Class *cls;
  file_t file;
  line_t line;

  /// @brief Destructor.
  virtual ~SoftwareEntity() = default;

  /// @brief Constructor.
  /// @param id The id of the symbol information.
  /// @param name The name of the entity.
  /// @param img The containing image.
  /// @param nmsp The containing namespace.
  /// @param cls The containing class.
  /// @param file The name of the containing file.
  /// @param line The line of the declaration.
  explicit SoftwareEntity(Id_t id, name_t name, Image *img, Namespace *nmsp, Class *cls,
                          file_t file, line_t line);

  SoftwareEntity() {}

  /*!
   * @brief Returns the type of the concrete software entity.
   * @return The EntityType.
   */
  virtual EntityType getEntityType() const = 0;
};

}  // namespace entity
}  // namespace pcv

#endif //DWARFLOADER_SOFTWAREENTITY_H
