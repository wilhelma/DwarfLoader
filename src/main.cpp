#include <cassert>
#include <cstdlib>
#include <stdlib.h>

#include "Filter.h"
#include "Duplicate.h"
#include "DwarfReader.h"
#include "ArchBuilder.h"
#include "NamespaceRule.h"
#include "ClassRule.h"
#include "RegexFileRule.h"
#include "RegexNameRule.h"

using pcv::dwarf::DwarfReader;
using pcv::ArchBuilder;
using pcv::ArchRule;
using pcv::NamespaceRule;
using pcv::ClassRule;
using pcv::RegexFileRule;
using pcv::RegexNameRule;
using pcv::dwarf::Filter;
using pcv::dwarf::DieDuplicate;
using pcv::entity::EntityType;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "No program argument given!\n";
    abort();
  }
  try {
    Filter filter(
        "(.+)pcv(.+)",
        "(.+)boost(.+)"
    );
    DieDuplicate duplicate;
    DwarfReader reader(argv[1], duplicate, filter);
    reader.start();

    ArchBuilder builder(reader.getContext());

    ArchRule *nRule = new NamespaceRule();

    ArchRule *ruleA = new RegexNameRule("functions", EntityType::Routine, "(.*)");

    builder.apply( nRule );
    builder.apply( ruleA );

    builder.finish();
    std::cout << builder;

    delete nRule;
    delete ruleA;

  } catch (DwarfError& e) {
    std::cerr << e.what();
    return -1;
  }
}
