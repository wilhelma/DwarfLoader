//
// Created by wilhelma on 1/4/17.
//

#ifndef DWARFLOADER_FILEGUARD_H
#define DWARFLOADER_FILEGUARD_H

#include <fcntl.h>
#include <string>
#include <stdexcept>
#include <unistd.h>

namespace pcv {
namespace dwarf {

class FileException : public std::runtime_error {
 public:
  explicit FileException(const std::string& arg) : std::runtime_error(arg) {}
};

struct FileGuard {
  int fd {};

  FileGuard(const char *fileName)
  {
    fd = open(fileName, O_RDONLY);
    if (fd == -1)
      throw FileException(std::string("Can not open file ") + fileName);
  }

  ~FileGuard()
  {
    close(fd);
  }
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_FILEGUARD_H
