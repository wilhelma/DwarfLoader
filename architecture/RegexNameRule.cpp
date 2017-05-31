//
// Created by wilhelma on 1/10/17.
//

#include "RegexNameRule.h"

namespace pcv {
namespace dwarf {

RegexNameRule::RegexNameRule(const std::string &artifactName,
                             EntityType type,
                             const std::string &regexString)
    : RegexRule(artifactName, type, regexString) {}

const std::string&
RegexNameRule::getMatchString(const SoftwareEntity& entity) const
{
  return entity.name;
}

}  // namespace dwarf
}  // namespace pcv
