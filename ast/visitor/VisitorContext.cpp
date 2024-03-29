//
// Created by Faris Cakaric on 26.06.17.
//

#include "VisitorContext.h"
#include "ArchBuilder.h"

using pcv::dwarf::Filter;
using pcv::dwarf::DieDuplicate;
using pcv::Artifact_t;

VisitorContext::VisitorContext(const Context &ctxt,
                               std::ostream& out) : archBuilder_(ctxt), out_(out) {

}

void VisitorContext::pushToArchRulesStack(ArchRule *archRule) {
  archRulesStack.push(archRule);
}

ArchRule* VisitorContext::popFromArchRulesStack() {
  ArchRule* topElement = archRulesStack.top();
  archRulesStack.pop();
  return topElement;
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
  out_ << archBuilder_;
}

void VisitorContext::applyRuleToBuilder(ArchRule *archRule) {
  archBuilder_.apply(archRule);
}