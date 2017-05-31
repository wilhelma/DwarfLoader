//
// Created by wilhelma on 1/25/17.
//

#ifndef DWARFLOADER_CUFILEMANAGER_H
#define DWARFLOADER_CUFILEMANAGER_H

#include <vector>
#include <string>

namespace pcv {
namespace dwarf {

class CuFileManager {
 public:
  CuFileManager() = default;
  ~CuFileManager() = default;

  void addCuFile(const std::string &fileName);
  const std::string& getFileName(int index) const;
  void clear();

  /* disable copy/move construction and assignment operators */
  CuFileManager(const CuFileManager &) = delete;
  CuFileManager(CuFileManager &&) = delete;
  CuFileManager &operator=(const CuFileManager &) = delete;
  CuFileManager &operator=(CuFileManager &&) = delete;

 private:
  std::vector<std::string> cuFiles_;
};

}  // namespace dwarf
}  // namespace pcv

#endif //DWARFLOADER_CUFILEMANAGER_H
