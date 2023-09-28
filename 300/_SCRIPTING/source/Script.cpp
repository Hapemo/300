/*!*************************************************************************
****
\file Script.cpp
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 1-11-2022
\brief
This file contains the logic to Load and Run scripts.
****************************************************************************
***/
#define SOL_NO_EXCEPTIONS 1
#define SOL_ALL_SAFETIES_ON 1

#include "Script.h"
#include "ScriptingSystem.h"

// Declare static variable (WIP as mentioned in h file)
//bool Script::isOnce = false;

void Script::Load(Entity entity)
{
    sol::protected_function_result result = systemManager->mScriptingSystem->luaState.script_file(scriptFile, env);
    if (!result.valid())
    {
        // print what error was invoked when the script was loading
        sol::error err = result;
        std::cout << "Error opening file!" << std::endl;
        std::cout << err.what() << std::endl;
    }

    env["script_entity_id"] = entity.id;
}

void Script::Run(const char* funcName)
{
    // Calling a function in lua
    sol::protected_function func = env[funcName];
    sol::protected_function_result result = func();

    // Will throw error if type is different
    if (!result.valid())
    {
        sol::error err = result;
        std::cout << "Error running function! From " << scriptFile << " of function " << funcName << std::endl;
        std::cout << err.what() << std::endl;
        return;
    }

    // Check return value
    if (result.return_count() == 1)
    {
        sol::object obj = result.get<sol::object>();
        // check if variable is type Vec2
        if (obj.is<Vec2>())
        {
            // get value as type Vec2
            Vec2 vec = obj.as<Vec2>();
        }
        else
        {
            sol::error err = result;
            std::cout << "Wrong type!" << std::endl;
            std::cout << err.what() << std::endl;
        }
    }
}

void Scripts::AddScript(Entity id, std::string fileName)
{
    Script temp;
    temp.scriptFile = fileName;
    temp.env = { systemManager->mScriptingSystem->luaState, sol::create, systemManager->mScriptingSystem->luaState.globals() };
    id.GetComponent<Scripts>().scriptsContainer.push_back(temp);
}
