//
// Created by wilhelma on 1/25/17.
//

#ifndef DWARFLOADER_DUPLICATE_H
#define DWARFLOADER_DUPLICATE_H

#include <libdwarf.h>
#include <string>
#include <unordered_map>

namespace pcv {
namespace dwarf {

class Context;

inline void hash_combine(std::size_t &seed) {}

template <typename T, typename... Rest>
inline void hash_combine(std::size_t &seed, const T &v, Rest... rest)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  hash_combine(seed, rest...);
}

typedef struct DieIdentifier_t {
  Dwarf_Half tag;
  Dwarf_Unsigned line;
  std::string file;
  explicit DieIdentifier_t(Dwarf_Half tag, Dwarf_Unsigned line, const std::string &file)
      : tag(tag), line(line), file(file) {}
} DieIdentifier_t;

class DieDuplicate {
 public:
  DieDuplicate() = default;
  ~DieDuplicate() = default;

  Dwarf_Off isDuplicate(const Context& ctxt) const;
  void addDie(const Context& ctxt);
  void addDuplicate(const Context& ctxt);

  /* disable copy/move construction and assignment operators */
  DieDuplicate(const DieDuplicate &) = delete;
  DieDuplicate(DieDuplicate &&) = delete;
  DieDuplicate &operator=(const DieDuplicate &) = delete;
  DieDuplicate &operator=(DieDuplicate &&) = delete;

 private:
  std::unordered_map<size_t, Dwarf_Off> duplicates_;
  std::unordered_map<Dwarf_Off, Dwarf_Off> mappings_;

  std::size_t getHash(const Context& /* ctxt */) const;
};

}  // namespace dwarf
}  // namespace pcv

namespace std {

template<>
class hash<pcv::dwarf::DieIdentifier_t> {
 public:
  size_t operator()(const pcv::dwarf::DieIdentifier_t &identifier) const
  {
    std::size_t ret{0};
    pcv::dwarf::hash_combine(ret, identifier.tag, identifier.line, identifier.file);
    return ret;
  }
};

}  // namespace std

#endif //DWARFLOADER_DUPLICATE_H
