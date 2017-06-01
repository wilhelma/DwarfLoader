//
// Created by wilhelma on 12/23/16.
//

#include "DwarfReader.h"

#include "Filter.h"
#include "tag/TagGeneric.h"
#include "entities/Variable.h"

namespace {

using pcv::entity::Variable;

void postProcess(pcv::dwarf::Context *ctxt) {
  ctxt->establishInheritance();
  ctxt->establishComposition();
  ctxt->establishTypedefs();
}

std::unique_ptr<std::vector<Variable*>> getVariablesOfType(Variable::Type type, Context& ctxt)
{
  std::unique_ptr<std::vector<Variable*>> vec {new std::vector<Variable*>};

  for (auto& v : ctxt.variables) {
    if (v->type == type) {
      vec->emplace_back(v.get());
    }
  }

  return vec;
}

}

namespace pcv {
namespace dwarf {

using pcv::entity::Namespace;

void DwarfReader::errHandler(Dwarf_Error err, Dwarf_Ptr errArg)
{
  std::cerr << "err: " << dwarf_errmsg(err) << '\n';
  auto dbg = static_cast<Dwarf_Debug>(errArg);
  dwarf_dealloc(dbg, err, DW_DLA_ERROR);
}

DwarfReader::DwarfReader(const std::string &filename,
                         DieDuplicate &dieDuplicate,
                         const Filter &filter)
    : dieDuplicate_(dieDuplicate), filter_(filter)
{
  fileGuard_ = std::unique_ptr<FileGuard> {new FileGuard(filename.c_str())};
  dbgGuard_ = std::unique_ptr<DbgGuard> {new DbgGuard(fileGuard_->fd, errHandler)};
  ctxt_.dbg = dbgGuard_->dbg;

  ctxt_.namespaces.emplace_back(new Namespace(std::string(""), nullptr));
  ctxt_.emptyNamespace = ctxt_.currentNamespace = ctxt_.namespaces.back().get();
}

void DwarfReader::iterateCUs()
{
  Dwarf_Unsigned cu_header_length, abbrev_offset, next_cu_header, typeOffset;
  Dwarf_Half version_stamp, address_size, offset_size, extension_size;
  Dwarf_Sig8 signature{};
  Dwarf_Die cu_die;

  int res{DW_DLV_OK};
  while (res == DW_DLV_OK) {
    res = dwarf_next_cu_header_c(ctxt_.dbg, IS_INFO, &cu_header_length, &version_stamp,
                                 &abbrev_offset, &address_size, &offset_size, &extension_size,
                                 &signature, &typeOffset, &next_cu_header, nullptr);
    if (res == DW_DLV_ERROR) throw DwarfError("Error with dwarf_next_cu_header()");

    Dwarf_Error err;
    if (dwarf_siblingof_b(ctxt_.dbg, nullptr, IS_INFO, &cu_die, &err) == DW_DLV_OK) {
      ctxt_.reset();
      iterate(cu_die);
      dwarf_dealloc(ctxt_.dbg, cu_die, DW_DLA_DIE);
    }
  }
}

void DwarfReader::iterate(Dwarf_Die die) noexcept
{
  Dwarf_Die child_die{}, sib_die{};
  ctxt_.die = die;
  auto duplicate = ctxt_.duplicate = dieDuplicate_.isDuplicate(ctxt_);
  if (duplicate)
    dieDuplicate_.addDuplicate(ctxt_);
  else
    dieDuplicate_.addDie(ctxt_);

  if (!handle(die)) {
    if (dwarf_child(die, &child_die, nullptr) == DW_DLV_OK) {
      iterate(child_die);
      dwarf_dealloc(ctxt_.dbg, child_die, DW_DLA_DIE);
    }
  }
  ctxt_.duplicate = duplicate;
  leave(die);

  if (dwarf_siblingof_b(ctxt_.dbg, die, IS_INFO, &sib_die, nullptr) == DW_DLV_OK) {
    iterate(sib_die);
    dwarf_dealloc(ctxt_.dbg, sib_die, DW_DLA_DIE);
  }
}

void DwarfReader::leave(Dwarf_Die die)
{
  if (hasAttr(die, DW_AT_decl_file)) {
    Dwarf_Unsigned fileNo{};
    getAttrUint(ctxt_.dbg, die, DW_AT_decl_file, &fileNo);
//    if (!filter_.isValid(ctxt_.srcFiles[fileNo - 1]))
//      return;
  }

  Dwarf_Half tag{};
  if (dwarf_tag(die, &tag, nullptr) != DW_DLV_OK) throw DwarfError("dwarf_tag() failed");
  ctxt_.die = die;
  leaveDwarfDie(ctxt_, tag);
}

bool DwarfReader::handle(Dwarf_Die die)
{
  Dwarf_Half tag{};
  if (dwarf_tag(die, &tag, nullptr) != DW_DLV_OK) throw DwarfError("dwarf_tag() failed");

  if (hasAttr(ctxt_.die, DW_AT_decl_file)) {
    Dwarf_Unsigned fileNo{};
    getAttrUint(ctxt_.dbg, ctxt_.die, DW_AT_decl_file, &fileNo);
/*    if (!filter_.isValid(ctxt_.srcFiles[fileNo - 1]))
      return true;*/
  }

  return handleDwarfDie(ctxt_, tag);
}

void DwarfReader::start()
{
  iterateCUs();
  processContext();
}

void DwarfReader::processContext()
{
  ctxt_.establishInheritance();
  ctxt_.establishComposition();
  ctxt_.establishTypedefs();
}

}  // namespace dwarf
}  // namespace pcv

