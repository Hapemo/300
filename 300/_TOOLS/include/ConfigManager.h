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

private:
	static std::unordered_map<std::string, std::string> mContainer; // Contain a map of key to value extracted from config file


};
