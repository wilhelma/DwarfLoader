//
// Created by wilhelma on 1/17/17.
//

#ifndef DWARFLOADER_CLASSRULE_H
#define DWARFLOADER_CLASSRULE_H

#include <regex>
#include "ArchRule.h"

namespace pcv {

class ClassRule : public ArchRule {
  const std::string artifactName_;
  const std::regex rx_;
 public:
  ClassRule(const std::string &artifactName, const std::string &regexString);
  std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
  std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;
};

}  // namespace pcv

#endif //DWARFLOADER_CLASSRULE_H
