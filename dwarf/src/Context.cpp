//
// Created by wilhelma on 1/7/17.
//

#include <algorithm>
#include "Context.h"

namespace pcv {

using entity::Class;

namespace dwarf {


void Context::addClass(Dwarf_Off off, Class *cls)
{
  offClassMap_[off] = cls;
  if (getClass(*cls->nmsp, cls->name) == nullptr)
    nameClassMap_[cls->nmsp->name + cls->name] = cls;

  // add nested classes
  if (cls->cls != nullptr) {
    if (std::find(std::begin(cls->cls->nestedClasses),
                  std::end(cls->cls->nestedClasses),
                  cls) == std::end(cls->cls->nestedClasses))
    {
      cls->cls->nestedClasses.push_back(cls);
    }
  }

  currentClass.push(cls);
}

Class *Context::getClass(Dwarf_Off off) const
{
  auto tmp = offClassMap_.find(off);
  if (tmp != std::end(offClassMap_))
    return tmp->second;

  return nullptr;
}

Class* Context::getClass(const Namespace& nmsp, const Class::name_t& name) const
{
  auto tmp = nameClassMap_.find(nmsp.name + name);
  if (tmp != std::end(nameClassMap_))
    return tmp->second;

  return nullptr;
}

void Context::addRoutine(Dwarf_Off off, std::unique_ptr<Routine> rtn)
{
  nameRtnMap_[rtn->name] = rtn.get();
  offRoutineMap_[off] = rtn.get();
  routines.emplace_back(std::move(rtn));
}

void Context::linkNameToRoutine(const std::string &rtnName, Routine *rtn) {
  nameRtnMap_[rtnName] = rtn;
}

Routine* Context::getRoutine(Dwarf_Off off) const {
  auto tmp = offRoutineMap_.find(off);
  if (tmp != std::end(offRoutineMap_))
    return tmp->second;

  return nullptr;
}

Routine* Context::getRoutine(const Routine::name_t &name) const {
  auto rtn = nameRtnMap_.find(name);
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
  inheritances_.emplace_back(ClassRelation_t(baseOff, inhOff));
}

void Context::addComposition(Dwarf_Off first, Dwarf_Off second)
{
  compositions_.emplace_back(ClassRelation_t(first, second));
}

void Context::establishInheritance()
{
  for (auto &inh : inheritances_) {
    Class *inhClass = getClass(inh.second);
    Class *baseClass = getClass(inh.first);
    if (inhClass && baseClass) {
      inhClass->baseClasses.push_back(baseClass);
      baseClass->inheritClasses.push_back(inhClass);
    }
  }
}

void Context::establishComposition()
{
  for (auto &com : compositions_) {
    Class *inhClass = getClass(com.second);
    Class *baseClass = getClass(com.first);
    if (inhClass && baseClass) {
      inhClass->composites.push_back(baseClass);
    }
  }
}

void Context::establishTypedefs()
{
  for (auto &td : offTypedefName_) {
    auto ptr = getClass(td.first);
    if (ptr)
      ptr->name = td.second;
  }
}

void Context::reset() noexcept
{
  srcFiles.clear();
}

void Context::clearCache() noexcept
{
  offRoutineMap_.clear();
  offVariableMap_.clear();
  nameRtnMap_.clear();
  offTypedefName_.clear();
}

void Context::addStaticVariable(Dwarf_Off off, std::unique_ptr<Variable> var)
{
  variables.emplace_back(std::move(var));
  offStaticVariableMap_[off] = variables.back().get();
}

Variable* Context::getStaticVariable(Dwarf_Off off) const
{
  auto it = offStaticVariableMap_.find(off);
  if (it != end(offStaticVariableMap_))
    return it->second;
  else
    return nullptr;
}

}  // namespace dwarf
}  // namespace pcv

