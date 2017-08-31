//
// Created by Faris Cakaric on 27.08.17.
//

#ifndef DWARFLOADER_BINARYOPERATORRULE_H
#define DWARFLOADER_BINARYOPERATORRULE_H

#include "ArchRule.h"

#include "entities/Class.h"
#include "entities/Routine.h"
#include "entities/Variable.h"
#include "entities/Namespace.h"

namespace pcv {
  using entity::Class;
  using entity::Routine;
  using entity::Variable;
  using entity::Namespace;

  class BinaryOperatorRule : public ArchRule {

  public:
    void getNamespacesInArtifact(Artifact_t &artifact, std::unordered_set<const Namespace *> &namespaces);
    void getClassesInArtifact(Artifact_t &artifact, std::unordered_set<const Class *> &classes);
    void getRoutinesInArtifact(Artifact_t &artifact, std::unordered_set<const Routine *> &routines, ArchRule::added_t &added);
    void getGlobalVariablesInArtifact(Artifact_t &artifact, std::unordered_set<const Variable *> &variables, ArchRule::added_t &added);
  };

}  // namespace pcv

#endif //DWARFLOADER_BINARYOPERATORRULE_H
