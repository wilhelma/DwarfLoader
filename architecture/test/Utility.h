//
// Created by wilhelma on 1/20/17.
//

#ifndef DWARFLOADER_UTILITY_H
#define DWARFLOADER_UTILITY_H

#include "../../entities/Class.h"

using pcv::dwarf::Artifact_t;
using pcv::dwarf::SoftwareEntity;


/// @brief Fill an architecture with the following setup:
///         [ {a {b {c {d, entityA} } } },
///           {a {b {e, entityA, entityB} } } ,
///           {x {b {c {f, entityB} } } } ]
/// @param ctxt The context that will be filled.
void fillArtifact(Artifact_t &artifact, const std::array<SoftwareEntity*, 2>& entities);



#endif //DWARFLOADER_UTILITY_H
