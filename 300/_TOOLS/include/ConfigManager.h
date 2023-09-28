/*!*****************************************************************************
\file ConfigManager.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Pepe Production
\date 24-09-2023
\brief
Allows the program to access manually set values from the config file of the
application

Config file layout:
"Key": "Value"\n

*******************************************************************************/
#pragma once
#include "pch.h"

class ConfigManager {
public:
	static void Init(std::string const& _path);
	static inline std::string const& GetValue(std::string const& _key) { return mContainer[_key]; }

private: // Helper functions
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


private:
	static std::unordered_map<std::string, std::string> mContainer; // Contain a map of key to value extracted from config file


};
