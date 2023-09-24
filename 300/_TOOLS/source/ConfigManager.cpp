/*!*****************************************************************************
\file ConfigManager.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 24-09-2023
\brief
Allows the program to access manually set values from the config file of the
application
*******************************************************************************/
#include "ConfigManager.h"

std::unordered_map<std::string, std::string> ConfigManager::mContainer;

void ConfigManager::Init(std::string const& _path) {
  // Opening file
  std::fstream file;
  file.open(_path, std::ios_base::in);
  if (!file.is_open()) {
    assert(false && std::string("File " + _path + " not found.\n").c_str());
    return;
  }

  mContainer = TextFileToMap(file);
}

std::unordered_map<std::string, std::string> ConfigManager::TextFileToMap(std::fstream& _file) {
  std::unordered_map<std::string, std::string> data;
  std::string line{};
  while (std::getline(_file, line)) {
    if (!line.size()) continue;
    size_t midpoint{ line.find(':') };
    data.insert({ line.substr(0, midpoint), line.substr(midpoint + 2, midpoint + 2 - line.length()) });
  }
  return data;
}













