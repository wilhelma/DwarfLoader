//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_NAMESPACERULE_H
#define DWARFLOADER_NAMESPACERULE_H

#include "ArchRule.h"

namespace pcv {
namespace dwarf {

class NamespaceRule : public ArchRule {
 protected:
  std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const Context &ctxt) override;
  std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const Context &ctxt) override;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_NAMESPACERULE_H
