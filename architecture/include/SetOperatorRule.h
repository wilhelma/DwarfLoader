//
// Created by Faris Cakaric on 18.06.17.
//

#ifndef DWARFLOADER_SETOPERATORRULE_H
#define DWARFLOADER_SETOPERATORRULE_H

#include <regex>
#include <memory>
#include "ArchRule.h"

namespace pcv {

  class SetOperatorRule : public ArchRule {
    std::string artifactName_;
    std::vector<Artifact_t*> artifacts;

  public:
    SetOperatorRule(const std::string artifactName, std::vector<Artifact_t*> artifacts) : artifactName_(artifactName), artifacts(artifacts) {}

    void copyChildren(Artifact_t &parent, Artifact_t &artifact);

    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;

  };

}  // namespace pcv

#endif //DWARFLOADER_SETOPERATORRULE_H
