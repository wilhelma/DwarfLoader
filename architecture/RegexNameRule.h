//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_REGEXNAMERULE_H
#define DWARFLOADER_REGEXNAMERULE_H

#include "RegexRule.h"

namespace pcv {
namespace dwarf {

class SoftwareEntity;

class RegexNameRule : public RegexRule {
 public:
  explicit RegexNameRule(const std::string &artifactName,
                         EntityType type,
                         const std::string &regexString);

  const std::string& getMatchString(const SoftwareEntity& entity) const override final;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_REGEXNAMERULE_H
