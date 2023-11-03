/*!*************************************************************************
****
\file Script.h
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 3-11-2023
\brief
This file contains scripting component logic.
The scripting system contains the functions:
- Load
Load all scripts

- Run
Run any function in the script
(When Run("Dead) is passed in, this runs the function dead in script)

- Run (newly added in M2)
Run any function in the script with any number of arguments passed in
For example : This functions runs the OnTrigger(Entity) function in script

(Updated M2 (3/11/2023))
- RunWithReturnValue
Returns the value returned from a function in the script.
This is used in the scripts when a specific value of a variable in
the script from a specific entity has to be referenced in another script

Functions that provides the logic to get and set variables from 
scripts for lua reflection
****************************************************************************
***/
#pragma once
#include "sol/sol.hpp"
#include "ECS/ECS.h"
#include "ECS/ECS_Systems.h"

using scriptVariant = std::variant<bool, float, std::string, glm::vec3>;

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
	int id{};

	void Load(Entity entityID);
	// Updates according to the script (so if variables are removed, the check
	// will be done here and removed.
	void LoadEnvVar();
	void Run(const char* funcName);
	template<typename ...args>
	void Run(const char* funcName, args... arguments)
	{
		sol::protected_function func = env[funcName];
		sol::protected_function_result result = func(arguments...);

		if (!result.valid())
		{
			sol::error err = result;
			std::cout << "Error running function! From " << scriptFile << " of function " << funcName << std::endl;
			std::cout << err.what() << std::endl;
			return;
		}
	}
	template<typename T>
	T RunWithReturnValue(const char* funcName)
	{
		sol::protected_function func = env[funcName];
		auto result = func();

		if (!result.valid())
		{
			sol::error err = result;
			std::cout << "error running function! from " << scriptFile << " of function " << funcName << std::endl;
			std::cout << err.what() << std::endl;
			return T();
		}

		return result;
	}
	// Check if the type of the variable had changed
	bool CheckVariableTypeEqual(sol::object& value, const std::type_info& info);
	// Set the value of the variable stored in the map
	void SetVariable(const std::string& name, sol::object& obj);
	//void AddScript(Entity id, std::string fileName);
	// Make sure error only print once (WIP)
	static bool isOnce;

	// Contains variables from LUA
	std::map<std::string, scriptVariant> variables;
};
