//
// Created by Faris Cakaric on 26.06.17.
//

#include "VisitorContext.h"
#include "ArchBuilder.h"

using pcv::dwarf::Filter;
using pcv::dwarf::DieDuplicate;
using pcv::Artifact_t;

VisitorContext::VisitorContext(const Context &ctxt) : archBuilder_(ctxt) {

}

void VisitorContext::pushToArchRulesStack(ArchRule *archRule) {
  archRulesStack.push(archRule);
}

ArchRule* VisitorContext::popFromArchRulesStack() {
  ArchRule* topElement = archRulesStack.top();
  archRulesStack.pop();
  return topElement;
}

void VisitorContext::pushToArtifactNamesStack(const std::string artifactName) {
  artifactNamesStack.push(artifactName);
}

std::string VisitorContext::popFromArtifactNamesStack() {
  std::string artifactName = artifactNamesStack.top();
  artifactNamesStack.pop();
  return artifactName;
}

void VisitorContext::addArtifactToArchBuilder(std::string artifactName, ArchRule *archRule) {
  archRule->setArtifactName(artifactName);
  archBuilder_.getArchSet()->children.push_back(std::unique_ptr<Artifact_t>(archRule->getArchSet()));
}

Artifact_t* VisitorContext::getArtifactFromArchBuilder(std::string artifactName) {
  for(auto &child : archBuilder_.getArchSet()->children) {
    if(child->name == artifactName) {
      return child.get();
    }
  }
  return nullptr;
}

void VisitorContext::outputBuilderJson() {
  archBuilder_.finish();
  std::cout << archBuilder_;
}

void VisitorContext::applyRuleToBuilder(ArchRule *archRule) {
  archBuilder_.apply(archRule);
}