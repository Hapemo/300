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
//#include "../PCH/pch.h"

#define SOL_NO_EXCEPTIONS 1
#define SOL_ALL_SAFETIES_ON 1

//#include "../Debug/Logger.h"
//#include "ScriptingSystem.h"
//#include "../Engine/Engine.h"
//#include "../ECS/EntityManager.h"
#include "Script.h"
#include "ScriptingSystem.h"

void Script::Load(Entity entityID)
{
    sol::protected_function_result result = ScriptingSystem::luaState.script_file(scriptFile, env);
    if (!result.valid())
    {
        // print what error was invoked when the script was loading
        sol::error err = result;
        //PERROR("Error opening file! %s.\n", err.what());
    }

    env["script_entity_id"] = entityID.id;
}

void Script::Run(const char* funcName)
{
    // Calling a function in lua
    sol::protected_function func = env[funcName];
    sol::protected_function_result result = func();

    // Check if function is valid
    if (!func.valid())
    {
        sol::error err = result;
        //PERROR("Error getting function: %s.\n", err.what());
        return;
    }

    // Will throw error if type is different
    if (!result.valid())
    {
        sol::error err = result;
        //PERROR("Error running function: %s.\n", err.what());
        return;
    }

    // Check return value
    if (result.return_count() == 1)
    {
        // safe way to print (we can check the type) (check 'is' then 'as')
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
            //PERROR("Wrong type %s.\n", err.what());
        }
    }
}

void Scripts::AddScript(Entity id, std::string fileName)
{
    Script temp;
    temp.scriptFile = fileName;
    temp.env = { ScriptingSystem::luaState, sol::create, ScriptingSystem::luaState.globals() };
    id.GetComponent<Scripts>().scriptsContainer.push_back(temp);
}
//
//void Scripts::LoadRunScript(Entity entity)
//{
//    for (auto& script : entity.GetComponent<Scripts>().scriptsContainer)
//    {
//        script.Load(entity.id);
//        script.Run("Start");
//    }
//}
