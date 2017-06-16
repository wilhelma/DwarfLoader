#include <cassert>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <fstream>

#include "Filter.h"
#include "Duplicate.h"
#include "DwarfReader.h"
#include "ArchBuilder.h"
#include "NamespaceRule.h"
#include "ClassRule.h"
#include "RegexFileRule.h"
#include "RegexNameRule.h"

#include "../ast/nodes/Program.h"
#include "../ast/ast-from-json/CreateAstFromJson.h"
#include "../ast/visitor/ASTVisitor.h"


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

        ArchRule *nRule = new NamespaceRule("pcv");

        builder.apply(nRule);

        builder.finish();
        std::cout << builder;

        delete nRule;
        delete ruleA;

    } catch (DwarfError &e) {
        std::cerr << e.what();
        return -1;
    }
}
