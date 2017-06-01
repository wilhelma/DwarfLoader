//
// Created by wilhelma on 1/10/17.
//

#include "RegexFileRule.h"
#include "entities/SoftwareEntity.h"

namespace pcv {

RegexFileRule::RegexFileRule(const std::string &artifactName,
                             EntityType type,
                             const std::string &regexString)
    : RegexRule(artifactName, type, regexString) {}

const std::string&
RegexFileRule::getMatchString(const SoftwareEntity& entity) const
{
  return entity.file;
}

}  // namespace pcv
