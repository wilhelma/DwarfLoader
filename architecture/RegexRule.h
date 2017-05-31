//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_REGEXRULE_H
#define DWARFLOADER_REGEXRULE_H

#include <regex>
#include "ArchRule.h"
#include "../entities/SoftwareEntity.h"

namespace pcv {
namespace dwarf {

struct Artifact_t;

class RegexRule : public ArchRule {
 public:
  explicit RegexRule(const std::string &artifactName,
                     EntityType type,
                     const std::string &regexString);

  virtual ~RegexRule() = default;

  std::unique_ptr<ArchRule::artifacts_t>
  execute(Artifact_t &archSet, const Context &ctxt) override final;

  std::unique_ptr<ArchRule::artifacts_t>
  append(Artifact_t &archSet, const Context &ctxt) override final;

  virtual const std::string& getMatchString(const SoftwareEntity& entity) const = 0;

 private:
  const std::string artifactName_;
  EntityType type_;
  const std::regex rx_;

  void fillArtifact(const std::vector<SoftwareEntity*> &entities, Artifact_t *toFill) const;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_REGEXRULE_H
