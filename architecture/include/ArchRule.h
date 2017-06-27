//
// Created by wilhelma on 1/10/17.
//

#ifndef DWARFLOADER_COMMAND_H
#define DWARFLOADER_COMMAND_H

#include <vector>
#include <memory>
#include "ArchBuilder.h"

namespace pcv {

  namespace dwarf {
    struct Context;
  }
  struct Artifact_t;

/// @brief The base class for a rule to build an architecture.
  class ArchRule {
  protected:
    using artifacts_t = std::vector<Artifact_t *>;
    std::unique_ptr<artifacts_t> artifacts;
    Artifact_t *artifact_;

    virtual std::unique_ptr<artifacts_t> execute(Artifact_t &archSet, const dwarf::Context &ctxt) = 0;

    virtual std::unique_ptr<artifacts_t> append(Artifact_t &archSet, const dwarf::Context &ctxt) = 0;


  public:
    ArchRule() {};

    // ArchRule(std::string artifactName) : artifact_(artifactName, nullptr) {};
    virtual ~ArchRule() = default;

    void setArtifactName(std::string artifactName) {
      artifact_->name = artifactName;
    }

    void apply(Artifact_t &archSet, const dwarf::Context &ctxt) {
      artifacts = execute(archSet, ctxt);
      apply(artifacts, ctxt);
    }

    Artifact_t *getArchSet() {
      return artifact_;
    }

    void apply(std::unique_ptr<artifacts_t> &artifacts, const dwarf::Context &ctxt) {
      for (auto follower : followers_) {
        auto newArtifacts = std::unique_ptr<artifacts_t>(new artifacts_t);
        for (auto &artifact : *artifacts) {
          auto changedArtifacts = follower->append(*artifact, ctxt);
          for (auto ca : *changedArtifacts) {
            newArtifacts->emplace_back(ca);
          }
        }
        follower->apply(newArtifacts, ctxt);
      }
    }

    inline ArchRule *append(ArchRule *as) {
      followers_.emplace_back(as);
      return as;
    }

    template<typename T>
    friend ArchRule &operator<<(ArchRule &lhs, T &rhs);

    /* disable copy/move construction and assignment operators */
    ArchRule(const ArchRule &) = delete;

    ArchRule(ArchRule &&) = delete;

    ArchRule &operator=(const ArchRule &) = delete;

    ArchRule &operator=(ArchRule &&) = delete;

  private:
    std::vector<ArchRule *> followers_;
  };

  template<typename T>
  ArchRule &operator<<(ArchRule &lhs, T &rhs) {
    lhs.followers_.emplace_back(reinterpret_cast<ArchRule *>(&rhs));
    return lhs;
  }

}  // namespace pcv


#endif //DWARFLOADER_COMMAND_H
