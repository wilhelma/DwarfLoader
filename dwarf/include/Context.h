//
// Created by wilhelma on 12/27/16.
//

#ifndef DWARFLOADER_CONTEXT_H
#define DWARFLOADER_CONTEXT_H

#include <libdwarf.h>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <map>

#include "entities/Image.h"
#include "entities/Namespace.h"
#include "entities/Routine.h"
#include "entities/Class.h"
#include "entities/Variable.h"

#include <iostream>

namespace pcv {

class Parceive;

namespace dwarf {

using pcv::entity::SoftwareEntity;
using pcv::entity::Routine;
using pcv::entity::Image;
using pcv::entity::Namespace;
using pcv::entity::Class;
using pcv::entity::Variable;

struct ClassRelation{
  Dwarf_Off first;
  Dwarf_Off second;
  ClassRelation(Dwarf_Off baseOff, Dwarf_Off inhOff) : first(baseOff), second(inhOff) {}
};

struct SourceLocation {
  uint lineNo;
  std::string fileName;
  explicit SourceLocation(uint lineNo, const std::string& fileName)
    : lineNo(lineNo), fileName(fileName) {}
};

struct Context {
  Context();

  Dwarf_Debug dbg {};
  Dwarf_Die die {};

  Image *currentImage{ nullptr };
  Namespace *currentNamespace{ nullptr };
  Namespace *emptyNamespace{ nullptr };
  std::vector<Class*> currentClass;
  std::stack<Routine*> currentRoutine;

  Dwarf_Off duplicate{0};

  std::vector<std::unique_ptr<Image>> images;
  std::vector<std::unique_ptr<Namespace>> namespaces;
  std::vector<std::unique_ptr<Routine>> routines;
  std::vector<std::unique_ptr<Class>> classes;
  std::vector<std::unique_ptr<Variable>> variables;

  std::vector<std::string> srcFiles;

  void addTypedef(Dwarf_Off off, const std::string &name);
  void addInheritance(Dwarf_Off baseOff, Dwarf_Off inhOff);
  void addComposition(Dwarf_Off first, Dwarf_Off second);
  void establishInheritance();
  void establishComposition();
  void establishTypedefs();
  void reset() noexcept;

  // class
  void addClass(Dwarf_Off off, Class *cls);
  Class* getClass(const Namespace& nmsp, const Class::name_t& className) const;
  std::string getClassSpecifier(const std::string& namespaceName,
                                const Class::name_t& className) const;

  // routine
  void addRoutine(Dwarf_Off off, std::unique_ptr<Routine> rtn);
  void linkNameToRoutine(const std::string& rtnName, Routine* rtn);
  Routine* getRoutine(const Routine::name_t& name) const;

  // member
  void addMember(Dwarf_Off off, Variable* member);

  // source locations
  void addSourceLocation(const Dwarf_Addr &ip,
                         std::unique_ptr<pcv::dwarf::SourceLocation> loc) noexcept;
  SourceLocation *getSourceLocation(Dwarf_Addr ip) const;

  template<typename T>
  T* get(Dwarf_Off off) const;
  void add(Dwarf_Off off, SoftwareEntity* entity);

  const Routine::name_t getFixedConstructorName(const Routine::name_t &cstr) const;

  void clearCache() noexcept;

  /**
   * @brief Reserve the vector to store #<elements> pointers to software entities from debug
   * information.
   */
  void reserveEntityVector(
    size_t elements   /**<[in] the number of elements to reserve */);

  /**
   * @brief Finalize the mapped source locations such that they contain a final dummy entry.
   */
  void finalizeSourceLocations();

 private:
  size_t vectorSize_ {0};
  std::unique_ptr<std::vector<SoftwareEntity*>> entities_;
  std::map<Dwarf_Addr, std::unique_ptr<pcv::dwarf::SourceLocation>> sourceLocations_;
  std::unordered_map<Dwarf_Off, std::string> offTypedefName_;
  std::unordered_map<Class::name_t, Class*> nameClassMap_;
  std::unordered_map<Routine::name_t, Routine*> nameRtnMap_;
  std::vector<ClassRelation> inheritances_;
  std::vector<ClassRelation> compositions_;
};

template<typename T>
T *Context::get(Dwarf_Off off) const
{
  if (off < vectorSize_)
    return static_cast<T*>(entities_->at(off));
  return nullptr;
}

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_CONTEXT_H
