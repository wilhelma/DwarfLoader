//
// Created by Faris Cakaric on 17.06.17.
//

#ifndef DWARFLOADER_VARIABLERULE_H
#define DWARFLOADER_VARIABLERULE_H

#include <regex>
#include "ArchRule.h"
#include <entities/Variable.h>

namespace pcv {

  using entity::Variable;

  class VariableRule : public ArchRule {
    std::string artifactName_;
    const std::regex rx_;

    added_t added_;
  public:
    VariableRule() = default;
    explicit VariableRule(const std::string &artifactName, const std::string &regexString);
    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;

    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;
    /**
    * @brief Applies the variable rule to build artifacts from the given variables and
    * append them on the given artifact.
    * @returns the set of added software entities
    */
    added_t apply(
            Artifact_t& artifact,                               /** [in,out] The base artifact. */
            const std::unordered_set<const Variable*>& variables  /** [in] The variables to consider. */);
  };

}  // namespace pcv

#endif //DWARFLOADER_VARIABLERULE_H
