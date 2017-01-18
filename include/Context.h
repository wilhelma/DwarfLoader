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

  Image *currentImage { nullptr };
  Namespace *currentNamespace { nullptr };
  Class *currentClass { nullptr };
  Routine *currentRoutine { nullptr };

  std::vector<std::unique_ptr<Image>> images;
  std::vector<std::unique_ptr<Namespace>> namespaces;
  std::vector<std::unique_ptr<Routine>> routines;
  std::vector<std::unique_ptr<Class>> classes;
  std::vector<std::unique_ptr<Variable>> variables;

  std::vector<std::string> srcFiles;

  void addClass(Dwarf_Off off, std::unique_ptr<Class> cls);
  void addInheritance(Dwarf_Off baseOff, Dwarf_Off inhOff);
  void addComposition(Dwarf_Off first, Dwarf_Off second);
  void establishInheritance();
  void establishComposition();
  void reset();
  Class* getClass(Dwarf_Off off);

 private:
  std::unordered_map<Dwarf_Off, Class*> offClassMap_;
  std::vector<ClassRelation_t> inheritances_;
  std::vector<ClassRelation_t> compositions_;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_CONTEXT_H
