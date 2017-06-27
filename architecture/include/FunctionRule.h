//
// Created by Faris Cakaric on 15.06.17.
//

#ifndef DWARFLOADER_FUNCTIONRULE_H
#define DWARFLOADER_FUNCTIONRULE_H

#include <regex>
#include "ArchRule.h"

namespace pcv {

  class FunctionRule : public ArchRule {
    std::string artifactName_;
    const std::regex rx_;
  public:
    FunctionRule(const std::string &artifactName, const std::string &regexString);
    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;

    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;

  };

}  // namespace pcv

#endif //DWARFLOADER_FUNCTIONRULE_H
