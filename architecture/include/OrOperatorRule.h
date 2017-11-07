//
// Created by Faris Cakaric on 17.06.17.
//

#ifndef DWARFLOADER_ANDRULE_H
#define DWARFLOADER_ANDRULE_H

#include <regex>
#include <memory>
#include "BinaryOperatorRule.h"

namespace pcv {

  class OrOperatorRule : public BinaryOperatorRule {
    std::string artifactName_;
    ArchRule* firstArtifact_;
    ArchRule* secondArtifact_;

  public:
    OrOperatorRule(const std::string &artifactName_, ArchRule* firstArtifact_,
            ArchRule* secondArtifact_);

    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;

  };

}  // namespace pcv

#endif //DWARFLOADER_ANDRULE_H
