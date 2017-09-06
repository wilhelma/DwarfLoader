//
// Created by Faris Cakaric on 19.06.17.
//

#include <NamespaceRule.h>
#include "AndOperatorRule.h"
#include "ClassRule.h"
#include "FunctionRule.h"
#include "VariableRule.h"

using pcv::entity::SoftwareEntity;
using pcv::entity::EntityType;
namespace pcv {
  AndOperatorRule::AndOperatorRule(const std::string &artifactName, ArchRule *firstArtifact,
                                 ArchRule *secondArtifact) : artifactName_(artifactName),
                                                             firstArtifact_(firstArtifact),
                                                             secondArtifact_(secondArtifact) {}

  template <typename InIt1, typename InIt2, typename OutIt>
  OutIt unordered_set_intersection(InIt1 b1, InIt1 e1, InIt2 b2, InIt2 e2, OutIt out) {
    while (!(b1 == e1)) {
      if (!(std::find(b2, e2, *b1) == e2)) {
        *out = *b1;
        ++out;
      }
      ++b1;
    }

    return out;
  }

  std::unique_ptr<ArchRule::artifacts_t> AndOperatorRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};
    artifact_ = new Artifact_t(artifactName_, &archSet);
    artifact_->entity = nullptr;

    Artifact_t* firstArtifactSet = firstArtifact_->getArchSet();
    Artifact_t* secondArtifactSet = secondArtifact_->getArchSet();

    ArchRule::added_t added;

    std::unordered_set<const Namespace *> namespacesInFirstArtifact;
    std::unordered_set<const Namespace *> namespacesInSecondArtifact;
    getNamespacesInArtifact(*firstArtifactSet, namespacesInFirstArtifact);
    getNamespacesInArtifact(*secondArtifactSet, namespacesInSecondArtifact);
    std::unordered_set<const Namespace *> namespaces;
    unordered_set_intersection(
            namespacesInFirstArtifact.begin(), namespacesInFirstArtifact.end(),
            namespacesInSecondArtifact.begin(), namespacesInSecondArtifact.end(),
            std::inserter(namespaces, namespaces.begin())
    );
    NamespaceRule namespaceRule;
    std::unordered_map<const Namespace *, Artifact_t *> namespacesAdded = namespaceRule.apply(*artifact_, namespaces, false);

    // consider classes
    {
      std::unordered_set<const Class *> classesInFirstArtifact;
      std::unordered_set<const Class *> classesInSecondArtifact;
      getClassesInArtifact(*firstArtifactSet, classesInFirstArtifact);
      getClassesInArtifact(*secondArtifactSet, classesInSecondArtifact);
      std::unordered_set<const Class *> classes;
      unordered_set_intersection(
              classesInFirstArtifact.begin(), classesInFirstArtifact.end(),
              classesInSecondArtifact.begin(), classesInSecondArtifact.end(),
              std::inserter(classes, classes.begin())
      );
      ClassRule cRule;
      added = cRule.apply(*artifact_, classes);
    }

    // consider routines
    {
      std::unordered_set<const Routine *> routinesInFirstArtifact;
      std::unordered_set<const Routine *> routinesInSecondArtifact;
      getRoutinesInArtifact(*firstArtifactSet, routinesInFirstArtifact, added);
      getRoutinesInArtifact(*secondArtifactSet, routinesInSecondArtifact, added);
      std::unordered_set<const Routine *> routines;
      unordered_set_intersection(
              routinesInFirstArtifact.begin(), routinesInFirstArtifact.end(),
              routinesInSecondArtifact.begin(), routinesInSecondArtifact.end(),
              std::inserter(routines, routines.begin())
      );
      FunctionRule fRule;
      auto fAdded = fRule.apply(*artifact_, routines);
      added.insert(begin(fAdded), end(fAdded));
    }

    //consider global variables
    {
      std::unordered_set<const Variable *> variablesInFirstArtifact;
      std::unordered_set<const Variable *> variablesInSecondArtifact;
      getGlobalVariablesInArtifact(*firstArtifactSet, variablesInFirstArtifact, added);
      getGlobalVariablesInArtifact(*secondArtifactSet, variablesInSecondArtifact, added);
      std::unordered_set<const Variable *> variables;
      unordered_set_intersection(
              variablesInFirstArtifact.begin(), variablesInFirstArtifact.end(),
              variablesInSecondArtifact.begin(), variablesInSecondArtifact.end(),
              std::inserter(variables, variables.begin())
      );
      VariableRule vRule;
      vRule.apply(*artifact_, variables);
    }


    for(auto &child : artifact_->children) {
      auto nmsp = child.get()->entity->nmsp;
      if(nmsp && namespacesAdded[nmsp]) {
        child.get()->parent = namespacesAdded[nmsp];
      }
    }


    return artifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> AndOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt)
  {
    return nullptr;
  }


}