//
// Created by Faris Cakaric on 17.06.17.
//

#include <iostream>
#include "OrOperatorRule.h"
#include "ArchBuilder.h"
#include "Context.h"
#include "ClassRule.h"
#include "FunctionRule.h"
#include "VariableRule.h"
#include "entities/Variable.h"


namespace pcv {
  using entity::Variable;

  OrOperatorRule::OrOperatorRule(const std::string &artifactName, ArchRule* firstArtifact,
                    ArchRule* secondArtifact) : artifactName_(artifactName),
                                                                      firstArtifact_(firstArtifact),
                                                                      secondArtifact_(secondArtifact) {}

  void getClassesInArtifact(Artifact_t &artifact, std::unordered_set<const Class *> &classes) {
    if (artifact.entity && artifact.entity->getEntityType() == pcv::entity::EntityType::Class) {
      classes.insert(static_cast<const Class*>(artifact.entity));
    }
    for(auto &child : artifact.children) {
      getClassesInArtifact(*child, classes);
    }
  }

  void getRoutinesInArtifact(Artifact_t &artifact, std::unordered_set<const Routine *> &routines, ArchRule::added_t &added) {
    if (artifact.entity && artifact.entity->getEntityType() == pcv::entity::EntityType::Routine) {
      if(added.find(artifact.entity) == std::end(added)) {
        routines.insert(static_cast<const Routine*>(artifact.entity));
      }
    }
    for(auto &child : artifact.children) {
      getRoutinesInArtifact(*child, routines, added);
    }
  }

  void getGlobalVariablesInArtifact(Artifact_t &artifact, std::unordered_set<const Variable *> &variables, ArchRule::added_t &added) {
    if (artifact.entity && artifact.entity->getEntityType() == pcv::entity::EntityType::Variable) {
      if(added.find(artifact.entity) == std::end(added)) {
        variables.insert(static_cast<const Variable*>(artifact.entity));
      }
    }
    for(auto &child : artifact.children) {
      getGlobalVariablesInArtifact(*child, variables, added);
    }
  }

  std::unique_ptr<ArchRule::artifacts_t> OrOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};
    artifact_ = new Artifact_t(artifactName_, &archSet);

    Artifact_t* firstArtifactSet = firstArtifact_->getArchSet();
    Artifact_t* secondArtifactSet = secondArtifact_->getArchSet();

    ArchRule::added_t added;
    // consider classes
    {
      std::unordered_set<const Class *> classes;
      getClassesInArtifact(*firstArtifactSet, classes);
      getClassesInArtifact(*secondArtifactSet, classes);
      ClassRule cRule;
      added = cRule.apply(*artifact_, classes);
    }

    // consider routines
    {
      std::unordered_set<const Routine *> routines;
      getRoutinesInArtifact(*firstArtifactSet, routines, added);
      getRoutinesInArtifact(*secondArtifactSet, routines, added);
      FunctionRule fRule;
      auto fAdded = fRule.apply(*artifact_, routines);
      added.insert(begin(fAdded), end(fAdded));
    }

    //consider global variables
    {
      std::unordered_set<const Variable *> variables;
      getGlobalVariablesInArtifact(*firstArtifactSet, variables, added);
      getGlobalVariablesInArtifact(*secondArtifactSet, variables, added);
      VariableRule vRule;
      vRule.apply(*artifact_, variables);
    }

    return artifacts;
  }


  std::unique_ptr<ArchRule::artifacts_t> OrOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt)
  {
    return nullptr;
  }

}