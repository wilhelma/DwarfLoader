#include <cassert>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <FunctionRule.h>

#include "Filter.h"
#include "Duplicate.h"
#include "DwarfReader.h"
#include "ArchBuilder.h"
#include "OrOperatorRule.h"
#include "AndOperatorRule.h"
#include "SetOperatorRule.h"
#include "NamespaceRule.h"
#include "ClassRule.h"
#include "FunctionRule.h"
#include "VariableRule.h"
#include "RegexFileRule.h"
#include "RegexNameRule.h"

#include "../ast/nodes/Program.h"
#include "../ast/ast-from-json/CreateAstFromJson.h"
#include "../ast/visitor/ASTVisitor.h"

using pcv::dwarf::DwarfReader;
using pcv::ArchBuilder;
using pcv::ArchRule;
using pcv::OrOperatorRule;
using pcv::SetOperatorRule;
using pcv::AndOperatorRule;
using pcv::NamespaceRule;
using pcv::ClassRule;
using pcv::FunctionRule;
using pcv::VariableRule;
using pcv::RegexFileRule;
using pcv::RegexNameRule;
using pcv::dwarf::Filter;
using pcv::dwarf::DieDuplicate;
using pcv::entity::EntityType;

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "No program argument given!\n";
    abort();
  }
  try {
    std::string error;
    std::string line;
    std::string inputjson;
    std::ifstream inputfile(argv[2]);
    if (inputfile.is_open()) {
      while (getline(inputfile, line)) {
        inputjson += line;
      }
      inputfile.close();

      const auto json = Json::parse(inputjson, error);
      Program program = CreateAstFromJson::generateAst(json);
      ASTVisitor astVisitor;
      program.accept(astVisitor);
    } else {
      std::cerr << "Cannot opet file!";
      return -1;
    }

    Filter filter(
        "(.+)cppcheck(.+)",
        "(.+)boost(.+)"
    );
    DieDuplicate duplicate;
    DwarfReader reader(argv[1], duplicate, filter);
    reader.start();

    ArchBuilder builder(reader.getContext());

    ArchRule *sRule = new NamespaceRule("preprocessor", ".*simplecpp.*");
    ArchRule *xmlRule = new NamespaceRule("output", ".*tinyxml2.*");
    ArchRule *mRule = new FunctionRule("match", ".*::(M|m)atch.*");
    ArchRule *guiRule = new ClassRule("gui", "((?!ErrorLogger).)*", ".*gui.*");
    ArchRule *libRule = new ClassRule("library", ".*Library.*");
    ArchRule *valueRule = new ClassRule("ValueFlow", ".*Value.*");
    ArchRule *settingsRule = new ClassRule("settings", ".*Settings.*");
    ArchRule *cliRule = new ClassRule("cli", ".*", ".*cli.*");
    ArchRule *checkRule = new ClassRule("checks", "Check.*");


    builder.apply(sRule);
    builder.apply(xmlRule);
    builder.apply(mRule);
    builder.apply(guiRule);
    builder.apply(libRule);
    builder.apply(valueRule);
    builder.apply(settingsRule);
    builder.apply(cliRule);
    builder.apply(checkRule);

//    ArchRule *fileRule = new RegexFileRule("cliFile", EntityType::All, ".*cli.*");
//    ArchRule *classRule = new ClassRule("cliClass", ".*");
//    ArchRule *andRule = new AndOperatorRule("cli", fileRule, classRule);
//      builder.apply(fileRule);
//    builder.apply(classRule);
//    builder.apply(andRule);


    builder.finish();
    std::cout << builder;

  } catch (DwarfError &e) {
    std::cerr << e.what();
    return -1;
  }
}
