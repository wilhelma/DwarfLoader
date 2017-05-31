//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_REGEXFILERULE_H
#define DWARFLOADER_REGEXFILERULE_H

#include "RegexRule.h"

namespace pcv {
namespace dwarf {

class SoftwareEntity;

class RegexFileRule : public RegexRule {
 public:
  explicit RegexFileRule(const std::string &artifactName,
                         EntityType type,
                         const std::string &regexString);

  const std::string& getMatchString(const SoftwareEntity& entity) const override final;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_REGEXFILERULE_H
