//
// Created by Faris Cakaric on 17.06.17.
//

#ifndef DWARFLOADER_VARIABLERULE_H
#define DWARFLOADER_VARIABLERULE_H

#include <regex>
#include "ArchRule.h"

namespace pcv {

  class VariableRule : public ArchRule {
    std::string artifactName_;
    const std::regex rx_;
  public:
    VariableRule(const std::string &artifactName, const std::string &regexString);
    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;

    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;

  };

}  // namespace pcv

#endif //DWARFLOADER_VARIABLERULE_H
