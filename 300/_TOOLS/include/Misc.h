/*!*****************************************************************************
\file Misc.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Pepe Production
\date 24-09-2023
\brief
Miscellaneous helper functions that is useful across the game engine
*******************************************************************************/
#pragma once
#include <string>
#include <unordered_map>

struct Misc {
  /*!*****************************************************************************
  Check if there is memory leak
  *******************************************************************************/
	static void MemLeakCheck(int breakAlloc = -1);

  /*!*****************************************************************************
  Get file path and return file name, without directory and extension

  \param _filepath
  - File path in string format, with extension and directory

  \return std::string
  - File name in string format
  *******************************************************************************/
  static std::string GetFileName(std::string const& _filepath);

	/*!*****************************************************************************
	\brief
	Load a text file into a map. The format in text file should be as follows:
	<key1>: <value1>\n
	<key2>: <value2>\n
	<key3>: <value3>\n
	<key4>: <value4>\n

	\param std::fstream& file
	- Input file

	\return std::map<std::string, std::string>
	- Map containing data extracted from file
	*******************************************************************************/
	static std::unordered_map<std::string, std::string> TextFileToMap(std::fstream& _file);
};