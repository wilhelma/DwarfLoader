//
// Created by wilhelma on 1/2/17.
//

#ifndef DWARFLOADER_DWARFEXCEPTION_H
#define DWARFLOADER_DWARFEXCEPTION_H

#include <exception>
#include <stdexcept>

struct DwarfError : public std::runtime_error {
  explicit DwarfError(const std::string& arg) : std::runtime_error(arg) {}
};

#endif //DWARFLOADER_DWARFEXCEPTION_H
