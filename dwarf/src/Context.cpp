//
// Created by wilhelma on 1/7/17.
//

#include <cassert>
#include <algorithm>
#include "Context.h"

namespace pcv {

using entity::Class;

namespace dwarf {


void Context::addClass(Dwarf_Off off, Class *cls)
{
  add(off, cls);

  if (getClass(*cls->nmsp, cls->name) == nullptr) {
    auto classSpecifier = getClassSpecifier(cls->nmsp->name, cls->name);
    nameClassMap_[classSpecifier] = cls;
  }
  currentClass.push_back(cls);
}

Class* Context::getClass(const Namespace& nmsp, const Class::name_t& name) const
{
  auto classSpecifier = getClassSpecifier(nmsp.name, name);
  auto tmp = nameClassMap_.find(classSpecifier);
  if (tmp != std::end(nameClassMap_))
    return tmp->second;

  return nullptr;
}

void Context::addRoutine(Dwarf_Off off, std::unique_ptr<Routine> rtn)
{
  nameRtnMap_[rtn->name] = rtn.get();
  add(off, rtn.get());
  routines.emplace_back(std::move(rtn));
}

void Context::linkNameToRoutine(const std::string &rtnName, Routine *rtn) {
  nameRtnMap_[rtnName] = rtn;
}

Routine* Context::getRoutine(const Routine::name_t &name) const {
  auto fixedName = getFixedConstructorName(name);
  auto rtn = nameRtnMap_.find(fixedName);
  if (rtn != std::end(nameRtnMap_))
    return rtn->second;
  else
    return nullptr;
}

void Context::addVariable(Dwarf_Off off, std::unique_ptr<Variable> var)
{
  offVariableMap_[off] = var.get();
  variables.emplace_back(std::move(var));
}

Variable* Context::getVariable(Dwarf_Off off) const {
  auto tmp = offVariableMap_.find(off);
  if (tmp != std::end(offVariableMap_))
    return tmp->second;

  return nullptr;
}

void Context::addTypedef(Dwarf_Off off, const std::string &name)
{
  offTypedefName_[off] = name;
}

void Context::addInheritance(Dwarf_Off baseOff, Dwarf_Off inhOff)
{
  inheritances_.emplace_back(ClassRelation(baseOff, inhOff));
}

void Context::addComposition(Dwarf_Off first, Dwarf_Off second)
{
  compositions_.emplace_back(ClassRelation(first, second));
}

void Context::establishInheritance()
{
  for (auto &inh : inheritances_) {
    Class *inhClass = get<Class>(inh.second);
    Class *baseClass = get<Class>(inh.first);
    if (inhClass && baseClass) {
      inhClass->baseClasses.push_back(baseClass);
      baseClass->inheritClasses.push_back(inhClass);
    }
  }
}

void Context::establishComposition()
{
  for (auto &com : compositions_) {
    Class *inhClass = get<Class>(com.second);
    Class *baseClass = get<Class>(com.first);
    if (inhClass && baseClass) {
      inhClass->composites.push_back(baseClass);
    }
  }
}

void Context::establishTypedefs()
{
  for (auto &td : offTypedefName_) {
    auto ptr = get<Class>(td.first);
    if (ptr) {
      ptr->name = td.second;
    }
  }
}

void Context::reset() noexcept
{
  srcFiles.clear();
}

void Context::clearCache() noexcept
{
  offTypedefName_.clear();
}

std::string Context::getClassSpecifier(const std::string& namespaceName,
                                       const Class::name_t& className) const
{
  std::string specifier {namespaceName};

  if (!currentClass.empty()) {
    for (auto &cls : currentClass) {
      specifier += cls->name + "::";
    }
  }

  return specifier + className;
}

const Routine::name_t Context::getFixedConstructorName(const Routine::name_t &cstr) const
{
  // a workaround for the following problem (C1 and C2 constructor emitted):
  // http://stackoverflow.com/questions/6921295/dual-emission-of-constructor-symbols
  Routine::name_t altConstructor {cstr};
  std::size_t pos = cstr.find("C2E");
  if (pos != std::string::npos) {
    altConstructor.replace(pos, 3, "C1E");
  } else {
    pos = cstr.find("C1E");
    if (pos != std::string::npos)
      altConstructor.replace(pos, 3, "C2E");
  }

  return altConstructor;
}

Context::Context()
{
  //offClassMap_.set_empty_key(~0u);
}

void Context::reserveEntityVector(size_t e)
{
  vectorSize_ = e;
  entities_ = std::unique_ptr<std::vector<SoftwareEntity*>> {
    new std::vector<SoftwareEntity*>(e, nullptr)
  };
}

void Context::
addSourceLocation(const Dwarf_Addr &ip, std::unique_ptr<pcv::dwarf::SourceLocation> loc) noexcept
{
  sourceLocations_[ip] = std::move(loc);
}

SourceLocation *Context::getSourceLocation(Dwarf_Addr ip) const
{
  auto location = sourceLocations_.upper_bound(ip);
  if (location != std::begin(sourceLocations_)) {
    location--;
    return location->second.get();
  }
  return nullptr;
}

void Context::finalizeSourceLocations()
{
  assert(!sourceLocations_.empty());

  sourceLocations_[sourceLocations_.rbegin()->first + 1] = std::unique_ptr<SourceLocation> {
    new SourceLocation(0, "")
  };
}

void Context::add(Dwarf_Off off, SoftwareEntity *entity)
{
  (*entities_)[off] = entity;
}

void Context::addMember(Dwarf_Off off, Variable *member)
{
  currentClass.back()->members.emplace_back(member);
  add(off, member);
}

void Context::addDuplicate(Dwarf_Off original, Dwarf_Off duplicate)
{
  duplicates_[duplicate] = original;
}

Dwarf_Off Context::getDuplicate(Dwarf_Off original) const
{
  auto result = duplicates_.find(original);
  if (result == std::end(duplicates_))
    return 0;
  else
    return result->second;
}

}  // namespace dwarf
}  // namespace pcv

