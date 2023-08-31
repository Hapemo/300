/*!*************************************************************************
****
\file Script.h
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 1-11-2022
\brief
This file contains scripting component logic.
****************************************************************************
***/
#pragma once
//#include "../ECS/BaseComponent.h"
//#include "../ECS/EntityManager.h"
//#include "../Engine/Engine.h"
#include "../sol-3.2.3/sol/sol.hpp"

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

	void Load(entt::entity& entityID);
	void Run(const char* funcName);
};
