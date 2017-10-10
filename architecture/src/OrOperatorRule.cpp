//
// Created by Faris Cakaric on 17.06.17.
//

#include <iostream>
#include <NamespaceRule.h>
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
    artifact_->entity = nullptr;

    Artifact_t* firstArtifactSet = firstArtifact_->getArchSet();
    Artifact_t* secondArtifactSet = secondArtifact_->getArchSet();

    Artifact_t newArtifact("newArtifact", nullptr);
    Artifact_t nmspArtifact("newArtifact1", nullptr);

    ArchRule::added_t added;

    //consider namespaces
    std::vector<const Namespace *> namespaces;
    getNamespacesInArtifact(*firstArtifactSet, namespaces);
    getNamespacesInArtifact(*secondArtifactSet, namespaces);
    std::sort(std::begin(namespaces), std::end(namespaces), isParent);
    NamespaceRule namespaceRule;
    std::unordered_map<const Namespace *, Artifact_t *> namespacesAdded = namespaceRule.apply(&nmspArtifact, namespaces);

    // consider classes
    {
      std::unordered_set<const Class *> classes;
      getClassesInArtifact(*firstArtifactSet, classes);
      getClassesInArtifact(*secondArtifactSet, classes);
      ClassRule cRule;
      std::unordered_map<const SoftwareEntity *, Artifact_t *> addedCls = cRule.apply(&newArtifact, classes, false);
      for(auto cls : addedCls)
        added.insert(cls.first);
    }

    // consider routines
    {
      std::unordered_set<const Routine *> routines;
      getRoutinesInArtifact(*firstArtifactSet, routines, added);
      getRoutinesInArtifact(*secondArtifactSet, routines, added);
      FunctionRule fRule;
      auto fAdded = fRule.apply(&newArtifact, routines);
      added.insert(begin(fAdded), end(fAdded));
    }

    //consider global variables
    {
      std::unordered_set<const Variable *> variables;
      getGlobalVariablesInArtifact(*firstArtifactSet, variables, added);
      getGlobalVariablesInArtifact(*secondArtifactSet, variables, added);
      VariableRule vRule;
      vRule.apply(newArtifact, variables);
    }

    for(auto &child : artifact_->children) {
      auto nmsp = child.get()->entity->nmsp;
      if(nmsp && namespacesAdded[nmsp]) {
        child.get()->parent = namespacesAdded[nmsp];
      }
    }

    for(auto &child : newArtifact.children) {
      bool hasMapping = false;
      if(child.get()->entity && child.get()->entity->nmsp) {
        auto nmsp = namespacesAdded.begin();
        while(nmsp != std::end(namespacesAdded)) {
          if((*nmsp).first == child.get()->entity->nmsp) {
            child.get()->parent = (*nmsp).second;
            (*nmsp).second->children.push_back(std::move(child));
            hasMapping = true;
            break;
          }
          ++nmsp;
        }
      }
      if(!hasMapping) {
        child.get()->parent = artifact_;
        artifact_->children.push_back(std::move(child));
      }
    }

    for(auto &nmsp : nmspArtifact.children) {
      artifact_->children.push_back(std::move(nmsp));
    }

    return artifacts;
  }

  void matchNamespaces(Artifact_t* child, Artifact_t* nmsp) {

  }


  std::unique_ptr<ArchRule::artifacts_t> OrOperatorRule::append(Artifact_t &archSet, const dwarf::Context &ctxt)
  {
    return nullptr;
  }

}