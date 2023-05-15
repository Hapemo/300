/*!*****************************************************************************
\file FilePathManager.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-01-2023
\brief
File Path Manager is a manager that manages the filepaths of directory in
project flexibly. Instead of hardcoding the filepath directly into the code,
user can use filepath manager to get the file path.
*******************************************************************************/
#include "FilePathManager.h"
#include "Logger.h"

std::map<std::string, std::string> FilePathManager::mFilePaths;

void FilePathManager::Init(std::string const& path) {
  LOG_CREATE("FILEPATHMANAGER");

  // Opening file
  std::fstream file;
  file.open(path, std::ios_base::in);
  if (!file.is_open()) {
    LOG_WARN("File " + path + " not found.\n");
    return;
  }

  mFilePaths = Util::TextFileToMap(file);
}


std::string FilePathManager::GetFilePath(std::string const& pathName) {
  std::string filePath;
  try {
     filePath = mFilePaths.at(pathName);
  } catch (std::out_of_range const& err) {
    (void)err;
    LOG_CUSTOM("FILEPATHMANAGER", "Unable to get filepath of: " + pathName);
  }

  return filePath;
}
























































