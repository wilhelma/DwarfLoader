//
// Created by Faris Cakaric on 08.10.17.
//

#ifndef DWARFLOADER_CLASSHIERARCHY_H
#define DWARFLOADER_CLASSHIERARCHY_H

#include "ClassRule.h"

namespace pcv {
  class ClassHierarchyRule : public ClassRule {
  public:
    explicit ClassHierarchyRule(const std::string &artifactName,
                       const std::string &regex,
                       const std::string &fileRegex = ".*");

    ClassHierarchyRule() = default;

    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;
  };
}
#endif //DWARFLOADER_CLASSHIERARCHY_H
