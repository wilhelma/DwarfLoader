//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_NAMESPACERULE_H
#define DWARFLOADER_NAMESPACERULE_H

#include <regex>
#include "ArchRule.h"

namespace pcv {

  class NamespaceRule : public ArchRule {
    const std::string artifactName_;
    const std::regex rx_;
  public:
    NamespaceRule(const std::string &artifactName, const std::string &regexString);

    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;

    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;
  };

}  // namespace pcv

#endif //DWARFLOADER_NAMESPACERULE_H
