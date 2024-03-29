/*!*************************************************************************
****
\file ScriptingSystem.h
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 16-8-2023
\brief
This file contains scripting system class, where the logic
of how scripts will be loaded, read and reloaded is done.

The functions
- Init()
- Update()
- LoadHelper()
- ScriptReload()
****************************************************************************
***/
#pragma once
#include <sol/sol.hpp>
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "Input/Input.h"
#include "Script.h"
#include <vector>

/******************************************************************************/
/*!
    ScriptingSystem Class
 */
 /******************************************************************************/
class ScriptingSystem
{
public:
    ScriptingSystem() = default;
    void Init();
    void Update(float dt);
    void Exit();
    sol::state luaState;
    bool once;

    void LoadHelper();

    // Call in Reload function
    void ScriptReload();

    void TestingFromScriptSys(std::vector<std::string> const& vect);

    float lerp(float a, float b, float t);

    std::vector<std::string> CreateVectorString();
};
