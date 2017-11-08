//
// Created by wilhelma on 1/17/17.
//

#ifndef DWARFLOADER_CLASSRULE_H
#define DWARFLOADER_CLASSRULE_H

#include <regex>
#include <algorithm>
#include <unordered_map>
#include <entities/Routine.h>
#include "ArchRule.h"
#include "ArchBuilder.h"

namespace pcv {
  using pcv::entity::Class;
  using pcv::entity::Routine;

class ClassRule : public ArchRule {
 public:
  explicit ClassRule(const std::string &artifactName,
                     const std::string &regex,
                     const std::string &fileRegex = ".*");

  ClassRule() = default;

  std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
  std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;

  /**
   * @brief Applies the class rule to build class hierarchies on the given classes and append
   * them on the given artifact.
   * @returns the set of added software entities
   */
  std::unordered_map<const SoftwareEntity *, Artifact_t *> apply(
     Artifact_t* artifact,                             /** [in,out] The base artifact. */
     const std::unordered_set<const Class*>& classes,  /** [in] The classes to consider. */
     bool useAllClassesFromCtxt = true);

 protected:
  const std::string artifactName_;
  const std::regex rx_;
  const std::regex fileRx_;
  std::unordered_map<const SoftwareEntity *, Artifact_t *> added;

  const Class* getBaseClass(const Class *currentClass);

  void addMethod(const Routine *routine, Artifact_t *artifact);

  void traverseHierarchy(const Class *cls,
                         Artifact_t *artifact,
                         const std::vector<const Class *> &classes,
                         bool useAllClassesFromCtxt = true);
};

}  // namespace pcv

#endif //DWARFLOADER_CLASSRULE_H
