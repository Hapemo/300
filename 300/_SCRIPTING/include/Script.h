/*!*************************************************************************
****
\file Script.h
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 28-9-2023
\brief
This file contains scripting component logic.
The scripting system contains the functions:
- Load
Load all scripts
- Run
Run any function in the script
(When Run("Dead) is passed in, this runs the function dead in script)
****************************************************************************
***/
#pragma once
#include "sol/sol.hpp"
#include "ECS/ECS.h"
#include "ECS/ECS_Systems.h"

/***************************************************************************/
/*!
\brief
Contains the logic to Load and Run scripts.
*/
/***************************************************************************/
class Script {
public:
	// Environment
	sol::environment env{};
	std::string scriptFile{};

	void Load(Entity entityID);
	void Run(const char* funcName);

	// Make sure error only print once (WIP)
	static bool isOnce;
};
