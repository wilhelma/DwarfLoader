//
// Created by Faris Cakaric on 25.05.17.
//

#include "ASTVisitor.h"


void ASTVisitor::visit(AndExpression &el) {
    std::cout << "Visiting AndExpression\n";
    el.getLeft()->accept(*this);
    el.getRight()->accept(*this);
}

void ASTVisitor::visit(AtomExpression &el) {
    std::cout << "Visiting AtomExpression " << el.getRule() << " " << el.getRegex() << "\n";
}

void ASTVisitor::visit(Component &el) {
    std::cout << "Visiting Component " << el.getName() << "\n";
}

void ASTVisitor::visit(DefinitionExpression &el) {
    std::cout << "Visiting DefinitionExpression\n";
    el.getComponent()->accept(*this);
    el.getExpression()->accept(*this);
}

void ASTVisitor::visit(Expression &el) {
    std::cout << "Visiting Expression\n";
}

void ASTVisitor::visit(NotExpression &el) {
    std::cout << "Visiting NotExpression\n";
    el.getOperand()->accept(*this);
}

void ASTVisitor::visit(OrExpression &el) {
    std::cout << "Visiting OrExpression\n";
    el.getLeft()->accept(*this);
    el.getRight()->accept(*this);
}

void ASTVisitor::visit(Program &el) {
    for(int i = 0; i < el.getExpressions().size(); i++) {
        el.getExpressions()[i]->accept(*this);
    }
}

void ASTVisitor::visit(SetExpression &el) {
    std::cout << "Visiting SetExpression\n";
    for(int i = 0; i < el.getTerms().size(); i++) {
        el.getTerms()[i]->accept(*this);
    }
}