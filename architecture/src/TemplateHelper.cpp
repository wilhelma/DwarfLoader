//
// Created by wilhelma on 10/4/17.
//

#include <string>

#include "TemplateHelper.h"

namespace pcv {

TemplateHelper::TemplateHelper(Artifact_t *defaultArtifact)
   : _defaultArtifact(defaultArtifact) {}

Artifact_t* TemplateHelper::processRoutine(const Routine* routine)
{
  auto firstAngleBracket = routine->name.find_first_of('<');
  auto closingAngleBracket = routine->name.find_first_of(">::");

  if (firstAngleBracket != std::string::npos && closingAngleBracket == std::string::npos) {
    auto pureRoutineName = routine->name.substr(0, firstAngleBracket);

    // in case the templatized routine has not been seen so far => append it to the default artifact
    if (_artifacts.find(pureRoutineName) == std::end(_artifacts)) {
      _defaultArtifact->children.emplace_back(std::unique_ptr<pcv::Artifact_t> {
         new pcv::Artifact_t(pureRoutineName, _defaultArtifact)
      });
      _artifacts[pureRoutineName] = _defaultArtifact->children.back().get();
    }

    return _artifacts[pureRoutineName];
  } else {
    return _defaultArtifact;
  }
}

}

