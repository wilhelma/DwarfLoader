//
// Created by Faris Cakaric on 15.06.17.
//

#include <iostream>
#include "FunctionRule.h"
#include "ArchBuilder.h"
#include "Context.h"

namespace pcv {
  FunctionRule::FunctionRule(const std::string &regexString) : rx_(regexString) {}

  std::unique_ptr<ArchRule::artifacts_t>
  FunctionRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    auto artifacts = std::unique_ptr<artifacts_t> {new artifacts_t};

    Artifact_t *parent = &archSet;
    std::cout << ctxt.routines.size() << std::endl;
    for (auto &f : ctxt.routines) {
      if (std::regex_match(f->name, rx_)) {
        if (!f->isConstructor)

          std::cout << f->name << std::endl;
        parent->children.emplace_back(
                std::unique_ptr<Artifact_t>{new Artifact_t(f->name, parent)}
        );

      }

    }
    artifacts->emplace_back(parent->children.back().get());

    std::cout << artifacts->size();
    return artifacts;
  }

  std::unique_ptr<ArchRule::artifacts_t> FunctionRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}
}
