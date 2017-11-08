//
// Created by Faris Cakaric on 19.06.17.
//

#ifndef DWARFLOADER_ANDOPERATORRULE_H
#define DWARFLOADER_ANDOPERATORRULE_H

#include <regex>
#include <memory>
#include "BinaryOperatorRule.h"

namespace pcv {

  class AndOperatorRule : public BinaryOperatorRule {
    std::string artifactName_;
    ArchRule* firstArtifact_;
    ArchRule* secondArtifact_;

  public:
    AndOperatorRule(const std::string &artifactName_, ArchRule* firstArtifact_,
                   ArchRule* secondArtifact_);

    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;
  };

}  // namespace pcv

#endif //DWARFLOADER_ANDOPERATORRULE_H
