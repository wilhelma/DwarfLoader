//
// Created by wilhelma on 1/9/17.
//

#ifndef DWARFLOADER_DWARFHELPER_H
#define DWARFLOADER_DWARFHELPER_H

#include <functional>

#include <libdwarf.h>
#include <entities/SoftwareEntity.h>
#include <entities/Variable.h>

namespace pcv {

using entity::Id_t;

namespace dwarf {

struct VariableArgs {
  entity::Variable::name_t name;
  entity::Variable::size_t size;
  Dwarf_Unsigned file;
  Dwarf_Unsigned line;
  Id_t classId;
};

using locationHandler_t = std::function<void(Dwarf_Debug, Dwarf_Die, entity::Variable::offset_t)>;

/*!
 * @brief Resolves the dwarf offset of the inherent pointer type.
 * @return Returns true if type was resolved, otherwise false.
 */
bool getPtrOff(
  Dwarf_Debug dbg,                        /**< [IN] The debug handle.*/
  Dwarf_Die die,                          /**< [IN] The die.*/
  Dwarf_Off *off                          /**< [OUT] The dwarf offset.*/);

/*!
 * @return Returns true if die has a given attribute, otherwise false.
 */
bool hasAttr(
  Dwarf_Die die,                          /**< [IN] The die.*/
  Dwarf_Half attr                         /**< [IN] The attribute.*/);

/*!
 * @brief Resolves the text of a specific die attribute.
 * @return Returns true if there is an attribute that is a std::string, otherwise false.
 */
bool getAttrText(
  Dwarf_Debug dbg,                        /**< [IN] The debug handle.*/
  Dwarf_Die die,                          /**< [IN] The die.*/
  Dwarf_Half attr,                        /**< [IN] The attribute.*/
  char **text                             /**< [OUT] The text of the attribute.*/);

/*!
 * @brief Resolves the unsigned integer of a specific die attribute.
 */
void getAttrUint(
  Dwarf_Debug dbg,                        /**< [IN] The debug handle.*/
  Dwarf_Die die,                          /**< [IN] The die.*/
  Dwarf_Half attr,                        /**< [IN] The attribute.*/
  Dwarf_Unsigned* uint                    /**< [OUT] The uint of the attribute.*/);

/*!
 * @brief Loads the die in the given attribute (don't forget to deallocate this die!).
 * @return The new die.
 */
Dwarf_Die jump(
  Dwarf_Debug dbg,                        /**< [IN] The debug handle.*/
  Dwarf_Die die,                          /**< [IN] The die of the attribute.*/
  Dwarf_Half attr                         /**< [IN] The attribute.*/);

/*!
 * @return Returns the name of the given die.
 */
bool getDieName(
  Dwarf_Debug dbg,                        /**< [IN] The debug handle.*/
  Dwarf_Die die,                          /**< [IN] The die.*/
  char** name                             /**< [OUT] The name of the die.*/);

/*!
 * @brief Wrapper function to handle location expressions in Dwarf.
 */
void handleLocListFromExpr(
  Dwarf_Debug dbg,                                /**< [IN] The debug handle.*/
  Dwarf_Die die,                                  /**< [IN] The die.*/
  const locationHandler_t& stackHandler,                 /**< [IN] The handler for stack variables.*/
  const locationHandler_t& staticGlobalHandler           /**< [IN] The handler for static/global vars.*/);

/*!
 * @return The variable size of dbg/die.
 */
Dwarf_Unsigned getVariableSize(
  Dwarf_Debug dbg,                        /**< [IN] The debug handle.*/
  Dwarf_Die die                           /**< [IN] The die.*/);

/*!
 * @return The size of an array die.
 */
Dwarf_Unsigned getArraySize(
  Dwarf_Debug dbg,                        /**< [IN] The debug handle.*/
  Dwarf_Die die                           /**< [IN] The die.*/);
/*!
 * @return The variable name of dbg/die.
 */
const char* getVariableName(
  Dwarf_Debug dbg,                        /**< [IN] The debug handle.*/
  Dwarf_Die die                           /**< [IN] The die.*/);

/*!
 * @returns the class ptr of the variable type, if the type is a class/struct, otherwise nullptr.
 */
entity::Class* getClassOfVariable(
  Dwarf_Debug dbg,                        /**< [IN] The debug handle.*/
  Dwarf_Die die,                          /**< [IN] The die.*/
  const Context& ctxt                     /**< [IN] The context.*/);

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_DWARFHELPER_H
