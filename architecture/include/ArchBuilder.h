//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_ARCHBUILDER_H
#define DWARFLOADER_ARCHBUILDER_H

#include <vector>
#include <unordered_set>
#include <memory>

#include <iostream>

#include "entities/SoftwareEntity.h"

class ArchBuilderFixture;

namespace pcv {

namespace dwarf {
struct Context;
}

using pcv::entity::SoftwareEntity;

constexpr char DELIM {'/'};

class ArchRule;

/// @brief The artifact structure that builds the architecture.
typedef struct Artifact_t {
  std::string name;
  const SoftwareEntity* entity;
  std::unordered_set<const SoftwareEntity*> entities;
  std::vector<std::unique_ptr<Artifact_t>> children;
  Artifact_t* parent;

  /// @brief Constructor.
  /// @param name The name of the artifact.
  /// @param parent The parent artifact (nullptr if it is a root artifact).
  explicit Artifact_t(const std::string& name, Artifact_t* parent) : name(name), parent(parent) {}

  Artifact_t(const Artifact_t&) = delete;
  Artifact_t& operator=(const Artifact_t&) = delete;
  Artifact_t(const Artifact_t&&) = delete;
  Artifact_t&operator=(const Artifact_t&&) = delete;

  ~Artifact_t() = default;
} Artifact_t;

/// @brief The architecture builder class. It is responsible to apply architecture rules
/// to build a hierarchy of artifacts (an architecture) based on the debug information.
class ArchBuilder {
 public:

  /// @brief Constructor.
  /// @param [IN] ctxt A filled context with debug information.
  explicit ArchBuilder(const dwarf::Context &ctxt) : ctxt_(ctxt), artifact_("default", nullptr) {}

  /// @brief Destructor.
  ~ArchBuilder() = default;

  /// @brief Applies an architecture rule to create new artifacts on the root level.
  /// @param [IN] rule The architecture rule to apply.
  void apply(ArchRule *rule) noexcept;

   /// @brief Resolves duplicated software entities in the software artifacts by creating
   /// new artifacts on the root level.
  void finish();

  /// @brief The overloaded << operator that writes Json output.
  /// @param [out] os The ostream ref to write to.
  /// @param [in] obj The internal ArchBuilder reference.
  /// @return The ostream ref to write to.
  friend std::ostream& operator<<(std::ostream& os, const ArchBuilder& obj);

  /* disable copy/move construction and assignment operators */
  ArchBuilder(const ArchBuilder &) = delete;
  ArchBuilder(ArchBuilder &&) = delete;
  ArchBuilder &operator=(const ArchBuilder &) = delete;
  ArchBuilder &operator=(ArchBuilder &&) = delete;

 private:
  const dwarf::Context &ctxt_;
  Artifact_t artifact_;

  friend ArchBuilderFixture;
};


}  // namespace pcv

#endif //DWARFLOADER_ARCHBUILDER_H
