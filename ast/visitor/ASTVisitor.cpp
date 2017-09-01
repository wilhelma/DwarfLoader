//
// Created by Faris Cakaric on 25.05.17.
//

#include "ASTVisitor.h"
#include "AndOperatorRule.h"
#include "ClassRule.h"
#include "NamespaceRule.h"
#include "FunctionRule.h"
#include "OrOperatorRule.h"
#include "VariableRule.h"
#include "RegexFileRule.h"
#include "SetOperatorRule.h"
#include "NotOperatorRule.h"

using pcv::AndOperatorRule;
using pcv::ClassRule;
using pcv::NamespaceRule;
using pcv::FunctionRule;
using pcv::OrOperatorRule;
using pcv::VariableRule;
using pcv::RegexFileRule;
using pcv::EntityType;
using pcv::SetOperatorRule;
using pcv::NotOperatorRule;

ASTVisitor::ASTVisitor(VisitorContext *visitorContext) : visitorContext_(visitorContext) {}

void ASTVisitor::visit(AndExpression &el) {
  el.getLeft()->accept(*this);
  el.getRight()->accept(*this);
  ArchRule *archRule = new AndOperatorRule("", visitorContext_->popFromArchRulesStack(),
                                           visitorContext_->popFromArchRulesStack());
  visitorContext_->pushToArchRulesStack(archRule);
  visitorContext_->applyRuleToBuilder(archRule);
}

void ASTVisitor::visit(AssignmentExpression &el) {
  el.getExpression()->accept(*this);
  ArchRule *archRule = visitorContext_->popFromArchRulesStack();
  visitorContext_->addArtifactToArchBuilder(el.getArtifact().get()->getName(), archRule);
  expressions[el.getArtifact().get()->getName()] = archRule;
}

void ASTVisitor::visit(AtomExpression &el) {
  if (el.getRule() == "class") {
    ArchRule *archRule = new ClassRule("", el.getRegex());
    visitorContext_->pushToArchRulesStack(archRule);
    visitorContext_->applyRuleToBuilder(archRule);
  } else if (el.getRule() == "namespace") {
    ArchRule *archRule = new NamespaceRule("", el.getRegex());
    visitorContext_->pushToArchRulesStack(archRule);
    visitorContext_->applyRuleToBuilder(archRule);
  } else if (el.getRule() == "variable") {
    ArchRule *archRule = new VariableRule("", el.getRegex());
    visitorContext_->pushToArchRulesStack(archRule);
    visitorContext_->applyRuleToBuilder(archRule);
  } else if (el.getRule() == "function") {
    ArchRule *archRule = new FunctionRule("", el.getRegex());
    visitorContext_->pushToArchRulesStack(archRule);
    visitorContext_->applyRuleToBuilder(archRule);
  } else if (el.getRule() == "file") {
    ArchRule *archRule = new RegexFileRule("", EntityType::All, el.getRegex());
    visitorContext_->pushToArchRulesStack(archRule);
    visitorContext_->applyRuleToBuilder(archRule);
  }
}

void ASTVisitor::visit(Artifact &el) {
  visitorContext_->pushToArchRulesStack(expressions[el.getName()]);
}

void ASTVisitor::visit(DefinitionExpression &el) {
  el.getExpression()->accept(*this);
  ArchRule *archRule = visitorContext_->popFromArchRulesStack();
  expressions[el.getArtifact().get()->getName()] = archRule;

}

void ASTVisitor::visit(Expression &el) {
}

void ASTVisitor::visit(NotExpression &el) {
  ArchRule *archRule;
  if (dynamic_cast<Artifact *>(el.getOperand().get())) {
    archRule = new NotOperatorRule("", visitorContext_->getArtifactFromArchBuilder(
            dynamic_cast<Artifact *>(el.getOperand().get())->getName()));
  } else {
    el.getOperand()->accept(*this);
    archRule = new NotOperatorRule("", visitorContext_->popFromArchRulesStack()->getArchSet());
  }
  visitorContext_->pushToArchRulesStack(archRule);
  visitorContext_->applyRuleToBuilder(archRule);
}

void ASTVisitor::visit(OrExpression &el) {
  el.getLeft()->accept(*this);
  el.getRight()->accept(*this);
  ArchRule *archRule = new OrOperatorRule("", visitorContext_->popFromArchRulesStack(),
                                          visitorContext_->popFromArchRulesStack());
  visitorContext_->pushToArchRulesStack(archRule);
  visitorContext_->applyRuleToBuilder(archRule);
}

void ASTVisitor::visit(Program &el) {
  for (size_t i = 0; i < el.getExpressions().size(); i++) {
    el.getExpressions()[i]->accept(*this);
  }
  visitorContext_->outputBuilderJson();
}

void ASTVisitor::visit(SetExpression &el) {
  std::vector<Artifact_t *> artifactsFromStack;
  for (size_t i = 0; i < el.getTerms().size(); i++) {
      artifactsFromStack.push_back(expressions[el.getTerms()[i].get()->getName()]->getArchSet());
  }

  ArchRule *archRule = new SetOperatorRule("", artifactsFromStack);
  visitorContext_->pushToArchRulesStack(archRule);
  visitorContext_->applyRuleToBuilder(archRule);
}