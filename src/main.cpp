#include <cassert>
#include <cstdlib>
#include <stdlib.h>

#include "../include/Filter.h"
#include "../include/Duplicate.h"
#include "../include/DwarfReader.h"
#include "../architecture/ArchBuilder.h"
#include "../architecture/NamespaceRule.h"
#include "../architecture/ClassRule.h"
#include "../architecture/RegexFileRule.h"

using pcv::dwarf::DwarfReader;
using pcv::dwarf::ArchBuilder;
using pcv::dwarf::ArchRule;
using pcv::dwarf::NamespaceRule;
using pcv::dwarf::ClassRule;
using pcv::dwarf::RegexFileRule;
using pcv::dwarf::Filter;
using pcv::dwarf::DieDuplicate;
using pcv::dwarf::EntityType;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "No program argument given!\n";
    abort();
  }
  try {
    Filter filter(
        "(.+)Periscope(.+)",
        "(.+)boost(.+)"
    );
    DieDuplicate duplicate;
    DwarfReader reader(argv[1], duplicate, filter);
    reader.start();

    ArchBuilder builder(reader.getContext());

    ArchRule *nRule = new NamespaceRule();
    ArchRule *cRule = new ClassRule();
//    ArchRule *rRuleA = new RegexFileRule("architecture", EntityType::All, "(.+)/architecture/(.+)(cpp|h)");
//    ArchRule *rRuleE = new RegexFileRule("entities", EntityType::All, "(.+)/entities/(.+)(cpp|h)");
//    ArchRule *rRuleG = new RegexFileRule("guards", EntityType::All, "(.+)/guards/(.+)(cpp|h)");
//    ArchRule *rRuleB = new RegexFileRule("base", EntityType::All, "(.+)/(include|src)/(.+)(cpp|h)");
//    ArchRule *rRuleT = new RegexFileRule("tag", EntityType::All, "(.+)/(tag)/(.+)(cpp|h)");

    builder.apply( cRule );
//    builder.apply( rRuleA );
//    builder.apply( rRuleE );
//    builder.apply( rRuleG );
//    builder.apply( rRuleB );
//    builder.apply( cRule );
/*
    ArchRule *rRule= new RegexFileRule("parceive", EntityType::All, "(.+)/dc_cpp/(.+)(cpp|h)");
    builder.apply( rRule );
    builder.apply( rRule->append(cRule) );
    builder.apply( nRule );
    builder.apply( cRule );*/

    builder.finish();
    std::cout << builder;

    delete cRule;
//    delete nRule;
//    delete rRuleA;
//    delete rRuleE;
//    delete rRuleG;
//    delete rRuleB;
//    delete rRuleT;*/

  } catch (DwarfError& e) {
    std::cerr << e.what();
    return -1;
  }
}
