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

#include "../entities/Image.h"
#include "../entities/Namespace.h"
#include "../entities/Routine.h"
#include "../entities/Class.h"
#include "../entities/Variable.h"

namespace pcv {
namespace dwarf {

typedef struct ClassRelation_t{
  Dwarf_Off first;
  Dwarf_Off second;
  ClassRelation_t(Dwarf_Off baseOff, Dwarf_Off inhOff) : first(baseOff), second(inhOff) {}
} ClassRelation_t;

struct Context {
  Dwarf_Debug dbg {};
  Dwarf_Die die {};

  Image *currentImage{ nullptr };
  Namespace *currentNamespace{ nullptr };
  Namespace *emptyNamespace{ nullptr };
  std::stack<Class*> currentClass;
  Routine *currentRoutine{ nullptr };

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
  void reset();

  void addClass(Dwarf_Off off, Class *cls);
  Class* getClass(Dwarf_Off off);

  void addRoutine(Dwarf_Off off, std::unique_ptr<Routine> rtn);
  Routine* getRoutine(Dwarf_Off off);
 private:
  std::unordered_map<Dwarf_Off, Class*> offClassMap_;
  std::unordered_map<Dwarf_Off, Routine*> offRoutineMap_;
  std::unordered_map<Dwarf_Off, std::string> offTypedefName_;
  std::vector<ClassRelation_t> inheritances_;
  std::vector<ClassRelation_t> compositions_;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_CONTEXT_H
