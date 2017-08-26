//
// Created by Faris Cakaric on 19.07.17.
//

#ifndef DWARFLOADER_NOTOPERATOR_H
#define DWARFLOADER_NOTOPERATOR_H

#include <regex>
#include <memory>
#include "ArchRule.h"
#include "ArchBuilder.h"

using pcv::entity::Class;

namespace pcv {

  class NotOperatorRule : public ArchRule {
    std::string artifactName_;
    Artifact_t* operand_;

    added_t added;

    const Class* getBaseClass(const Class *currentClass);

    void addMethod(const Routine* routine, Artifact_t* artifact);

    void traverseHierarchy(const Class* cls,
                           Artifact_t* artifact,
                           const std::unordered_set<const Class *>& classes);

    bool checkIfClassIsInherited(const Class *cls, const std::unordered_set<const Class *> &classes);

  public:
    NotOperatorRule(const std::string &artifactName_, Artifact_t* operand_);

    bool checkIfOperandContainsClass(const Class* cls, Artifact_t* artifact);

    std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) override;
    std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) override;
  };

}  // namespace pcv

#endif //DWARFLOADER_NOTOPERATOR_H
