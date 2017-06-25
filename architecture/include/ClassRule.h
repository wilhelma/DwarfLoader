//
// Created by wilhelma on 1/17/17.
//

#ifndef DWARFLOADER_CLASSRULE_H
#define DWARFLOADER_CLASSRULE_H

#include <regex>
#include <algorithm>
#include <unordered_map>
#include "ArchRule.h"
#include "ArchBuilder.h"

namespace pcv {
  using pcv::entity::Class;

class ClassRule : public ArchRule {
  const std::string artifactName_;
  const std::regex rx_;
  std::unordered_map<const Class*, Artifact_t*> added;

  Artifact_t* appendHierarchy(const Class *c, Artifact_t &as,
                              std::unordered_set<const SoftwareEntity*> *toRemove);
 public:

  ClassRule(const std::string &artifactName, const std::string &regexString);
  std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
  std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;
};

}  // namespace pcv

#endif //DWARFLOADER_CLASSRULE_H
