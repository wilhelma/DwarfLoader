//
// Created by Faris Cakaric on 26.06.17.
//

#ifndef DWARFLOADER_VISITORCONTEXT_H
#define DWARFLOADER_VISITORCONTEXT_H

#include "ArchBuilder.h"
#include "DwarfReader.h"
#include "ArchRule.h"

#include <stack>
#include <algorithm>

using pcv::dwarf::DwarfReader;
using pcv::ArchBuilder;
using pcv::dwarf::Context;
using pcv::ArchRule;
using pcv::Artifact_t;

class VisitorContext {
  ArchBuilder archBuilder_;
  std::stack<ArchRule*> archRulesStack;
  std::stack<std::string> artifactNamesStack;
  std::ostream& out_;
public:
  VisitorContext(const Context& ctxt, std::ostream& out);

  void pushToArchRulesStack(ArchRule* archRule);

  ArchRule* popFromArchRulesStack();

  void pushToArtifactNamesStack(const std::string artifactName);

  std::string popFromArtifactNamesStack();

  void addArtifactToArchBuilder(std::string artifactName, ArchRule* archRule);

  Artifact_t* getArtifactFromArchBuilder(std::string artifactName);

  void outputBuilderJson();

  void applyRuleToBuilder(ArchRule* archRule);
};
#endif //DWARFLOADER_VISITORCONTEXT_H
