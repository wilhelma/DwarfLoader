//
// Created by wilhelma on 1/23/17.
//

#include "../include/Filter.h"

namespace pcv {
namespace dwarf {

Filter::Filter(const std::string &include, const std::string &exclude)
    : include_{include}, exclude_{exclude} {};

bool Filter::isIncluded(const std::string &fileName) const
{
  auto included = std::regex_match(fileName, include_);
  return included;
}

bool Filter::isExcluded(const std::string &fileName) const
{
  auto excluded = std::regex_match(fileName, exclude_);
  return excluded;
}

bool Filter::isValid(const std::string &fileName) const
{
  return !isExcluded(fileName) && isIncluded(fileName);
}

}  // namespace dwarf
}  // namespace pcv

