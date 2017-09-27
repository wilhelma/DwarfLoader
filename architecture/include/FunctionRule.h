//
// Created by Faris Cakaric on 15.06.17.
//

#ifndef DWARFLOADER_FUNCTIONRULE_H
#define DWARFLOADER_FUNCTIONRULE_H

#include <regex>
#include <entities/Routine.h>
#include "ArchRule.h"

namespace pcv {

using entity::Routine;

  class FunctionRule : public ArchRule {
    const std::string artifactName_;
    const std::regex rx_;

    added_t added_;
  public:
    FunctionRule() = default;
    explicit FunctionRule(const std::string &artifactName, const std::string &regexString);

    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;

    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;

    /**
     * @brief Applies the function rule to build function hierarchies on the given functions and
     * append them on the given artifact.
     * @returns the set of added software entities
     */
    added_t apply(
       Artifact_t* artifact,                               /** [in,out] The base artifact. */
       const std::unordered_set<const Routine*>& routines  /** [in] The routines to consider. */);
  };

}  // namespace pcv

#endif //DWARFLOADER_FUNCTIONRULE_H
