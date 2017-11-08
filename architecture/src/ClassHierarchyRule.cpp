//
// Created by Faris Cakaric on 08.10.17.
//

#include <Context.h>
#include "ClassHierarchyRule.h"

namespace pcv {
  ClassHierarchyRule::ClassHierarchyRule(const std::string &artifactName,
                                         const std::string &regex,
                                         const std::string &fileRegex)
          : ClassRule(artifactName, regex, fileRegex) {}

  std::unique_ptr<ArchRule::artifacts_t> ClassHierarchyRule::execute(Artifact_t &archSet, const dwarf::Context &ctxt) {
    artifact_ = new Artifact_t(artifactName_, &archSet);
    artifact_->entity = nullptr;
    auto newArtifact = artifact_;
    std::unordered_set<const Class *> classes;

    // consider only matching classes
    for (auto &c : ctxt.classes) {
      if (std::regex_match(c->name, rx_) && std::regex_match(c->file, fileRx_)) {
        classes.insert(c.get());
      }
    }

    apply(newArtifact, classes, false);
    return nullptr;
  }

  std::unique_ptr<ArchRule::artifacts_t> ClassHierarchyRule::append(Artifact_t &archSet, const dwarf::Context &ctxt) {}
}