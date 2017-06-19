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
            "(.+)DwarfLoader(.+)",
            "(.+)boost(.+)"
    );
    DieDuplicate duplicate;
    DwarfReader reader(argv[1], duplicate, filter);
    reader.start();

    ArchBuilder builder(reader.getContext());

    //   ArchRule *nRule = new NamespaceRule("compN", "pcv");
    ArchRule *cRule = new ClassRule("compC", "Dwarf.*");
    ArchRule *fRule = new FunctionRule("compF", ".*OrOperatorRule.*");
    ArchRule *vRule = new VariableRule("compV", ".*artifactName.*");
   // ArchRule *orRule = new OrOperatorRule("compClassORVariable", vRule, cRule);
    //ArchRule *or1Rule = new OrOperatorRule("comp(ClassORVariable)ORFunction", orRule, fRule);
    ArchRule *setRule = new SetOperatorRule("proba", fRule, vRule, cRule);
    //   builder.apply(nRule);
    builder.apply(cRule);
    builder.apply(fRule);
    builder.apply(vRule);
   // builder.apply(orRule);
    //builder.apply(or1Rule);
    builder.apply(setRule);

    builder.finish();
    std::cout << builder;

    delete vRule;
    delete cRule;
    delete fRule;

  } catch (DwarfError &e) {
    std::cerr << e.what();
    return -1;
  }
}
