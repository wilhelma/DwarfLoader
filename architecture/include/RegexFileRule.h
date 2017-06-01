//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_REGEXFILERULE_H
#define DWARFLOADER_REGEXFILERULE_H

#include "RegexRule.h"

namespace pcv {

using entity::SoftwareEntity;

class RegexFileRule : public RegexRule {
 public:
  explicit RegexFileRule(const std::string &artifactName,
                         EntityType type,
                         const std::string &regexString);

  const std::string& getMatchString(const SoftwareEntity& entity) const override;
};

}  // namespace pcv

#endif //DWARFLOADER_REGEXFILERULE_H
