//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_NAMESPACERULE_H
#define DWARFLOADER_NAMESPACERULE_H

#include <regex>
#include <unordered_map>

#include "ArchRule.h"

namespace pcv {
  using entity::Namespace;

  class NamespaceRule : public ArchRule {
    std::string artifactName_;
    const std::regex rx_;
  public:
    NamespaceRule() = default;

    NamespaceRule(const std::string &artifactName, const std::string &regexString);

    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;

    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;

    /**
   * @brief Applies the namespace rule to build namespace hierarchies on the given namespaces and append
   * them on the given artifact.
   * @returns the set of added software entities
   */
    std::unordered_map<const Namespace *, Artifact_t *> apply(
            Artifact_t& artifact,                            /** [in,out] The base artifact. */
            const std::unordered_set<const Namespace*>& namespaces,  /** [in] The namespaces to consider. */
            bool abstraction /**[in] Indicates whether classes and routines should be considered. */);
  };

}  // namespace pcv

#endif //DWARFLOADER_NAMESPACERULE_H
