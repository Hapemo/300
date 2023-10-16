/*!*************************************************************************
****
\file ScriptingSystem.h
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 16-8-2023
\brief
This file contains scripting system class, where the logic
of how scripts will be loaded, read and unloaded is done.
This system will be further enhanced in the future
when it will be involved in the porting in of the gameplay.

The functions
- Init()
Registers scripting system and lua state. Making
neccessary engine functions known in Lua.

- Update()
Updates scripting system by ensuring that the scripts
load when appropriate. Timer functions are also called
here to get start and end time of the loop in order to
generate the overall performance of the scripting system.

(The other functions print and printVec exist for testing)
****************************************************************************
***/
#pragma once
#include <sol/sol.hpp>
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "Input/Input.h"
#include "Script.h"

/******************************************************************************/
/*!
    Example
 */
 /******************************************************************************/

//struct Vec2
//{
//    Vec2() = default;
//    Vec2(float _x, float _y)
//    {
//        x = _x;
//        y = _y;
//    }
//    float x, y;
//};
//
//struct Vec3
//{
//    Vec3() = default;
//    Vec3(float _x, float _y, float _z)
//    {
//        x = _x;
//        y = _y;
//        z = _z;
//    }
//    float x, y, z;
//};

//void print(const std::string& str);
//
//void printVec(const Vec2& vec);

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

    // To call selected script init and update functions
    void ScriptAlive(const Entity& script);
    void ScriptStart(const Entity& script);
    //void ScriptUpdate(const Entity& script);
    void ScriptExit(const Entity& script);
    void ScriptDead(const Entity& script);

    // To test ScriptStart and ScriptUpdate in main
    void TestSSSU();
    bool runOnce;

    // Call in Reload function
    void ScriptReload();

    // AddScripts function that checks for the existence of Script component then attach desired script to entity
    void AddScript(Entity id, std::string fileName);

    // To delete after port in to imGui
    void ScriptingInitTest();
    void ScriptingUpdateTest();
    static bool printOnce;

private:
    // for performance
    double startTime, endTime;
};
