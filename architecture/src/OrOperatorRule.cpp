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