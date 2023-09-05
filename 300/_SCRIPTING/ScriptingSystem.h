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
#include "Input.h"
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
    void Init();
    void Update(/*double dt*/);
    void Exit();
    sol::state luaState;
    bool once = false;

private:
};
