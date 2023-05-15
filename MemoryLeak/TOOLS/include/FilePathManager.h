/*!*****************************************************************************
\file FilePathManager.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-01-2023
\brief
File Path Manager is a manager that manages the filepaths of directory in
project flexibly. Instead of hardcoding the filepath directly into the code,
user can use filepath manager to get the file path.
*******************************************************************************/
#pragma once
#include "pch.h"


class FilePathManager {
public:
  /*!*****************************************************************************
  Initialise all the file paths
  \param std::string
  - File directory to the text file that contains all the file path information
  *******************************************************************************/
	static void Init(std::string const&);
	
  /*!*****************************************************************************
  Get the file path directory
  \param std::string
  - Name of the file path
  \return std::string
  - Returns the file path directory
  *******************************************************************************/
	static std::string GetFilePath(std::string const&);

private:
	// Key: in-code name | Value: actual file path
	static std::map<std::string, std::string> mFilePaths;

};


































































