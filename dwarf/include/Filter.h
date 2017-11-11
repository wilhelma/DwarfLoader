//
// Created by wilhelma on 1/23/17.
//

#ifndef DWARFLOADER_FILTER_H
#define DWARFLOADER_FILTER_H

#include <unordered_set>
#include <regex>

namespace pcv {
namespace dwarf {

class Filter {
 public:
  explicit Filter(const std::string &include, const std::string &exclude);

  bool isIncluded(const std::string &fileName) const;
  bool isExcluded(const std::string &fileName) const;
  bool isValid(const std::string &fileName, uint lineNo) const;

  /* disable copy/move construction and assignment operators */
  Filter(const Filter &) = delete;
  Filter(Filter &&) = delete;
  Filter &operator=(const Filter &) = delete;
  Filter &operator=(Filter &&) = delete;

 private:
  std::regex include_;
  std::regex exclude_;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_FILTER_H
