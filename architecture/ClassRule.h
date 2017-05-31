//
// Created by wilhelma on 1/17/17.
//

#ifndef DWARFLOADER_CLASSRULE_H
#define DWARFLOADER_CLASSRULE_H

#include "ArchRule.h"

namespace pcv {
namespace dwarf {

class ClassRule : public ArchRule {
 public:
  std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const Context &ctxt) override;
  std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const Context &ctxt) override;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_CLASSRULE_H
