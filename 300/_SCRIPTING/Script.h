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
#include <sol.hpp>

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

	void Load(int& entityID);
	void Run(const char* funcName);
};

/***************************************************************************/
/*!
\brief
Script component that entities who need to have script will possess.
*/
/***************************************************************************/
class Scripts {
public:
	Scripts() = default;
	~Scripts() = default;
	void Inspect();

	//static void AddScript(EntityID id, std::string fileName);
	//static void LoadRunScript(EntityID entity);

	std::string scriptFile{};
	std::vector <Script> scriptsContainer;
};
