#include <cassert>
#include <cstdlib>
#include <stdlib.h>

#include "../include/DwarfReader.h"
#include "../architecture/ArchBuilder.h"
#include "../architecture/NamespaceRule.h"
#include "../architecture/ClassRule.h"

using pcv::dwarf::DwarfReader;
using pcv::dwarf::ArchBuilder;
using pcv::dwarf::ArchRule;
using pcv::dwarf::NamespaceRule;
using pcv::dwarf::ClassRule;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "No program argument given!\n";
    abort();
  }
  try {
    DwarfReader reader(argv[1]);
    reader.start();

    ArchBuilder builder(reader.getContext());

    std::unique_ptr<ArchRule> nRule{ new NamespaceRule() };
    std::unique_ptr<ArchRule> cRule{ new ClassRule() };
    builder.apply( *nRule.get() );
    builder.append( *cRule.get() );
    std::cout << builder << '\n';

  } catch (DwarfError& e) {
    std::cerr << e.what();
    return -1;
  }
}
