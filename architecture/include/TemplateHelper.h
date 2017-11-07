//
// Created by wilhelma on 10/4/17.
//

#ifndef DWARFLOADER_TEMPLATEHELPER_H
#define DWARFLOADER_TEMPLATEHELPER_H

#include <memory>
#include <memory>

#include "entities/Routine.h"

#include "ArchBuilder.h"

namespace pcv {

using pcv::entity::Routine;

/*!
 * @brief The TemplateHelper class helps to build artifacts for instantiations of the same template
 * function.
 */
class TemplateHelper {
 public:
  explicit TemplateHelper(Artifact_t* defaultArtifact);
  ~TemplateHelper() = default;

  TemplateHelper(const TemplateHelper&) = delete;

  /*!
   * @brief checks a given routine if it is a template routine and, if yes, appends it to a separate
   * artifact that might be created.
   * @returns the artifact for a given routine.
   */
  Artifact_t* processRoutine(
     const Routine* routine   /**[in] the routine for which an artifact will be returned. */
  );

 private:
  Artifact_t* _defaultArtifact;
  std::unordered_map<SoftwareEntity::name_t, Artifact_t*> _artifacts;
};

}  // namespace pcv

#endif //DWARFLOADER_TEMPLATEHELPER_H
