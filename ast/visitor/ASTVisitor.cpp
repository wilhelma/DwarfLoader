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

using pcv::AndOperatorRule;
using pcv::ClassRule;
using pcv::NamespaceRule;
using pcv::FunctionRule;
using pcv::OrOperatorRule;
using pcv::VariableRule;
using pcv::RegexFileRule;
using pcv::EntityType;
using pcv::SetOperatorRule;

ASTVisitor::ASTVisitor(VisitorContext* visitorContext) : visitorContext_(visitorContext) {}

void ASTVisitor::visit(AndExpression &el) {
    el.getLeft()->accept(*this);
    el.getRight()->accept(*this);
    ArchRule* archRule = new AndOperatorRule("", visitorContext_->popFromArchRulesStack(), visitorContext_->popFromArchRulesStack());
    visitorContext_->pushToArchRulesStack(archRule);
    visitorContext_->applyRuleToBuilder(archRule);
}

void ASTVisitor::visit(AtomExpression &el) {
    if(el.getRule() == "class") {
        ArchRule* archRule = new ClassRule("", el.getRegex());
        visitorContext_->pushToArchRulesStack(archRule);
        visitorContext_->applyRuleToBuilder(archRule);
    } else if(el.getRule() == "namespace") {
        ArchRule* archRule = new NamespaceRule("", el.getRegex());
        visitorContext_->pushToArchRulesStack(archRule);
        visitorContext_->applyRuleToBuilder(archRule);
    } else if(el.getRule() == "variable") {
        ArchRule* archRule = new VariableRule("", el.getRegex());
        visitorContext_->pushToArchRulesStack(archRule);
        visitorContext_->applyRuleToBuilder(archRule);
    } else if(el.getRule() == "function") {
        ArchRule* archRule = new FunctionRule("", el.getRegex());
        visitorContext_->pushToArchRulesStack(archRule);
        visitorContext_->applyRuleToBuilder(archRule);
    } else if(el.getRule() == "file") {
        ArchRule* archRule = new RegexFileRule("", EntityType::All, el.getRegex());
        visitorContext_->pushToArchRulesStack(archRule);
        visitorContext_->applyRuleToBuilder(archRule);
    }
}

void ASTVisitor::visit(Component &el) {
    visitorContext_->pushToArtifactNamesStack(el.getName());
}

void ASTVisitor::visit(DefinitionExpression &el) {
    el.getComponent()->accept(*this);
    el.getExpression()->accept(*this);
    ArchRule* archRule = visitorContext_->popFromArchRulesStack();
    visitorContext_->addArtifactToArchBuilder(visitorContext_->popFromArtifactNamesStack(), archRule);
}

void ASTVisitor::visit(Expression &el) {
}

void ASTVisitor::visit(NotExpression &el) {
    el.getOperand()->accept(*this);
}

void ASTVisitor::visit(OrExpression &el) {
    el.getLeft()->accept(*this);
    el.getRight()->accept(*this);
    ArchRule* archRule = new OrOperatorRule("", visitorContext_->popFromArchRulesStack(), visitorContext_->popFromArchRulesStack());
    visitorContext_->pushToArchRulesStack(archRule);
    visitorContext_->applyRuleToBuilder(archRule);
}

void ASTVisitor::visit(Program &el) {
    for(size_t i = 0; i < el.getExpressions().size(); i++) {
        el.getExpressions()[i]->accept(*this);
    }
    visitorContext_->outputBuilderJson();
}

void ASTVisitor::visit(SetExpression &el) {
    std::vector<Artifact_t*> artifactsFromStack;
    for(size_t i = 0; i < el.getTerms().size(); i++) {
      if(dynamic_cast<Component*>(el.getTerms()[i].get()))
        artifactsFromStack.push_back(visitorContext_->getArtifactFromArchBuilder(dynamic_cast<Component*>(el.getTerms()[i].get())->getName()));
      else {
        el.getTerms()[i]->accept(*this);
        artifactsFromStack.push_back(visitorContext_->popFromArchRulesStack()->getArchSet());
      }
    }
    ArchRule* archRule = new SetOperatorRule("", artifactsFromStack);
    visitorContext_->pushToArchRulesStack(archRule);
    visitorContext_->applyRuleToBuilder(archRule);
}
