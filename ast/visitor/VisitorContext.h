//
// Created by Faris Cakaric on 26.06.17.
//

#ifndef DWARFLOADER_VISITORCONTEXT_H
#define DWARFLOADER_VISITORCONTEXT_H

#include "ArchBuilder.h"
#include "DwarfReader.h"
#include "ArchRule.h"

#include <stack>

using pcv::dwarf::DwarfReader;
using pcv::ArchBuilder;
using pcv::dwarf::Context;
using pcv::ArchRule;

class VisitorContext {
  ArchBuilder archBuilder_;
  std::stack<ArchRule*> archRulesStack;
  std::stack<std::string> artifactNamesStack;
public:
  VisitorContext(Context& ctxt);

  void pushToArchRulesStack(ArchRule* archRule);

  ArchRule* popFromArchRulesStack();

  void pushToArtifactNamesStack(const std::string artifactName);

  std::string popFromArtifactNamesStack();

  void addArtifactToArchBuilder(std::string artifactName, ArchRule* archRule);

  void outputBuilderJson();

  void applyRuleToBuilder(ArchRule* archRule);
};
#endif //DWARFLOADER_VISITORCONTEXT_H
