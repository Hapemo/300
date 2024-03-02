/*!*************************************************************************
/*!*************************************************************************
****
\file Script.cpp
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

(Updated M2 (3/11/2023))
Functions that provides the logic to get and set variables from
scripts for lua reflection
****************************************************************************
***/
#define SOL_NO_EXCEPTIONS 1
#define SOL_ALL_SAFETIES_ON 1

#include "Script.h"
#include "ScriptingSystem.h"

void Script::Load(Entity entity)
{
    id = (int)entity.id;

    //PINFO("Script Loaded")

    env = { systemManager->mScriptingSystem->luaState, sol::create, systemManager->mScriptingSystem->luaState.globals() };

    sol::protected_function_result result = systemManager->mScriptingSystem->luaState.script_file(scriptFile, env);

    if (!result.valid())
    {
        // print what error was invoked when the script was loading
        sol::error err = result;
        std::cout << "Error opening file! " << "Entity ID: " << (int)entity.id << " FileName: " << scriptFile << std::endl;
        std::cout << err.what() << std::endl;
    }

    env["script_entity"] = env.create_with("id", entity.id);
    systemManager->mScriptingSystem->luaState.script("script_entity = readonlytable(script_entity)", env);
    LoadEnvVar();
}

void Script::Run(const char* funcName)
{
    // Calling a function in lua
    sol::protected_function func = env[funcName];

    if (!func)
    {
        std::cout << "not func: " << funcName << std::endl;
        return;
    }

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

bool Script::CheckVariableTypeEqual(sol::object& value, const std::type_info& info)
{
    switch (value.get_type())
    {
    case sol::type::number:
    {
        return typeid(float) == info;
    }
    case sol::type::string:
    {
        return typeid(std::string) == info;
    }
    case sol::type::boolean:
    {
        return typeid(bool) == info;
    }
    case sol::type::userdata:
    {
        if (value.is<glm::vec3>())
            return typeid(glm::vec3) == info;
    }
    }

    return false;
}

void Script::SetVariable(const std::string& name, sol::object& value)
{
    switch (value.get_type())
    {
    case sol::type::number:
    {
        variables[name] = value.as<float>();
        break;
    }
    case sol::type::string:
    {
        variables[name] = value.as<std::string>();
        break;
    }
    case sol::type::boolean:
    {
        variables[name] = value.as<bool>();
        break;
    }
    case sol::type::userdata:
    {
        if (value.is<glm::vec3>())
            variables[name] = value.as<glm::vec3>();
        break;
    }
    }
}

void Script::LoadEnvVar()
{
    std::map<std::string, bool> deleted;

    // intially store all exisiting variables in container variables
    for (auto& elem : variables)
    {
        // set them to true by default
        deleted[elem.first] = true;
    }

    for (auto& e : env)
    {
        std::string name = e.first.as<std::string>();
        auto& value = e.second;

        if (name == "script_entity_id")
            continue;

        //found
        if (variables.find(name) != variables.end())
        {
            // visit all data and update accordingly
            std::visit([&](auto&& v) {

                // find type of value
                const std::type_info& info = typeid(decltype(v));

                // if the type of the variable did not change, update its value only
                if (CheckVariableTypeEqual(value, info))
                    env[name] = v;
                // if the type of the variable changed, remove it from the map
                // and add it in again with its new type
                else
                {
                    variables.erase(name);
                    SetVariable(name, value);
                }
                }, variables[name]);
            // since we had found the variable, it should not be deleted
            deleted[name] = false;
        }
        else
        {
            // varaible is not found so add it to the variables map
            SetVariable(name, value);
        }
    }

    // check if its deleted
    for (auto& elem : deleted)
    {
        // since the deleted flag is set to true, we can remove it from variables map
        if (elem.second)
        {
            variables.erase(elem.first);
        }
    }
}
