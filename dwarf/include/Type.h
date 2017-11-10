//
// Created by wilhelma on 12/27/16.
//

#ifndef DWARFLOADER_TYPE_H
#define DWARFLOADER_TYPE_H

#include <string>
#include <typeinfo>
#include <cxxabi.h>
#include "DwarfException.h"

namespace pcv {
namespace dwarf {

std::string demangle(const char* name);
std::string demangleNameOnly(const char *name);

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_TYPE_H
