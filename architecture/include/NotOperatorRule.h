//
// Created by Faris Cakaric on 19.07.17.
//

#ifndef DWARFLOADER_NOTOPERATOR_H
#define DWARFLOADER_NOTOPERATOR_H

#include <regex>
#include <memory>
#include "BinaryOperatorRule.h"

namespace pcv {

  class NotOperatorRule : public BinaryOperatorRule {
    std::string artifactName_;
    Artifact_t* operand_;

  public:
    NotOperatorRule(const std::string &artifactName_, Artifact_t* operand_);

    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;
  };

}  // namespace pcv

#endif //DWARFLOADER_NOTOPERATOR_H
