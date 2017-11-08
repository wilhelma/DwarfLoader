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
using pcv::Artifact_t;

int main(int argc, char **argv) {
  Filter filter("(.+)container(.+)", "(.+)boost(.+)");
  DieDuplicate duplicate;
  auto reader = std::unique_ptr<DwarfReader> { new DwarfReader(argv[1], duplicate, filter) };

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
      reader->start();

      std::stringstream test;

      VisitorContext visitorContext(reader->getContext(), std::cout);
      ASTVisitor astVisitor(&visitorContext);

      Program program = CreateAstFromJson::generateAst(json);
      program.accept(astVisitor);

    } else {
      std::cerr << "Cannot open file!";
      return -1;
    }

  } catch (DwarfError &e) {
    std::cerr << e.what();
    return -1;
  }
}
