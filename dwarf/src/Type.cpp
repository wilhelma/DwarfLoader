//
// Created by wilhelma on 12/27/16.
//

#include "../include/Type.h"

#ifdef __GNUG__
#include <cxxabi.h>
#include <memory>

namespace pcv {
namespace dwarf {

std::string demangle(const char* name)
{
  int status {};

  std::unique_ptr<char, void(*)(void*)> res {
      abi::__cxa_demangle(name, nullptr, 0, &status),
      std::free
  };

  return (status==0) ? res.get() : name ;
}

std::string demangleNameOnly(const char *name)
{
  std::string demangledName = demangle(name);

  { // remove parameters
    auto firstParenthese = demangledName.find_first_of('(');
    if (firstParenthese != std::string::npos)
      demangledName = demangledName.substr(0, firstParenthese);
  }

  return demangledName;
}

}  // namespace dwarf
}  // namespace pcv

#else

namespace pcv {
namespace dwarf {

// does nothing if not g++
std::string demangle(const char* name) {
    return name;
}

}  // namespace dwarf
}  // namespace pcv

#endif

