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
 public:
  using added_t = std::unordered_set<const Class*>;

  explicit ClassRule(const std::string &artifactName,
                     const std::string &regex,
                     const std::string &fileRegex = ".*");
  std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
  std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;

 private:
  const std::string artifactName_;
  const std::regex rx_;
  const std::regex fileRx_;
  added_t added;

  const Class* getBaseClass(const Class *currentClass);
  void traverseHierarchy(const Class* cls, Artifact_t* artifact);
};

}  // namespace pcv

#endif //DWARFLOADER_CLASSRULE_H
