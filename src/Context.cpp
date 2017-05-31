//
// Created by wilhelma on 1/7/17.
//

#include "../include/Context.h"

namespace pcv {
namespace dwarf {


void Context::addClass(Dwarf_Off off, Class *cls)
{
  offClassMap_[off] = cls;
  currentClass.push(cls);
}

Class *Context::getClass(Dwarf_Off off)
{
  auto tmp = offClassMap_.find(off);
  if (tmp != std::end(offClassMap_))
    return tmp->second;

  return nullptr;
}

void Context::addRoutine(Dwarf_Off off, std::unique_ptr<Routine> rtn)
{
  offRoutineMap_[off] = rtn.get();
  routines.emplace_back(std::move(rtn));
}

Routine* Context::getRoutine(Dwarf_Off off) {
  auto tmp = offRoutineMap_.find(off);
  if (tmp != std::end(offRoutineMap_))
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

void Context::reset()
{
  srcFiles.clear();
}

}  // namespace dwarf
}  // namespace pcv

