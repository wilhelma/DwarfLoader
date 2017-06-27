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
    std::vector<ArchRule*> artifacts;

  public:
    template <class U>
    SetOperatorRule(const std::string artifactName, U n) {
      artifactName_ = artifactName;
      artifacts.push_back(n);
    }

    template <class U, class... Ts>
    SetOperatorRule(const std::string artifactName, U n, Ts... rest) : SetOperatorRule(artifactName, rest...) {
      artifactName_ = artifactName;
      artifacts.push_back(n);
    }

    Artifact_t* copyChildren(Artifact_t &parent, Artifact_t &artifact);

    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;

  };

}  // namespace pcv

#endif //DWARFLOADER_SETOPERATORRULE_H
