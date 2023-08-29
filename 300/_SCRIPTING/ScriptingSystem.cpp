/*!*************************************************************************
****
\file ScriptingSystem.cpp
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 16-8-2022
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
//#include "../PCH/pch.h"
//#include "../Engine/Engine.h"
//#include "../ECS/EntityManager.h"
//#include "../Input/InputSystem.h"
//#include "LuaEngine.h"
//#include "LuaComponent.h"
//#include "../GameState/GameStateManager.h"
//#include "Script.h"
#include "ScriptingSystem.h"

sol::state ScriptingSystem::luaState{};
bool ScriptingSystem::once{ false };

//void print(const std::string& str)
//{
//    std::cout << str << std::endl;
//}
//
//void printVec(const Vec2& vec)
//{
//    std::cout << "Vec " << vec.x << "  " << vec.y << std::endl;
//}

void ScriptingSystem::Init()
{
    //g_engine->entityMgr->RegisterSystem(SystemType<ScriptingSystem>(), this);
    //AddComponentSignature<Scripts>();

    //luaState.open_libraries();

    //luaState.set_function("print", print);
    //luaState.set_function("printVec", printVec);

    //luaState.new_usertype<Vec2>(
    //    "Vec2", sol::constructors<Vec2(), Vec2(float, float)>(),
    //    "x", &Vec2::x,
    //    "y", &Vec2::y
    //    );

    //LuaEngine();
    //LuaGameStateMgr();
    //LuaEntityMgr();
    //LuaGraphicsComp();
    //LuaScriptComp();
    //LuaAnimation();
    //LuaGameObject();
    //LuaInput();
    //LuaLayer();
    //LuaTimer();
    //LuaAudio();
    //LuaFont();
    //LuaHelperFunc();
    //LuaRigidBody();

    //errorFlagCounter = 1000;

    // Bring keys and mouse buttons to LUA scripts
    /*luaState.new_enum("InputKeys",
        "MOUSE_LEFT", GLFW_MOUSE_BUTTON_LEFT,
        "MOUSE_RIGHT", GLFW_MOUSE_BUTTON_RIGHT,
        "KEY_0", GLFW_KEY_0,
        "KEY_1", GLFW_KEY_1,
        "KEY_2", GLFW_KEY_2,
        "KEY_3", GLFW_KEY_3,
        "KEY_4", GLFW_KEY_4,
        "KEY_5", GLFW_KEY_5,
        "KEY_6", GLFW_KEY_6,
        "KEY_7", GLFW_KEY_7,
        "KEY_8", GLFW_KEY_8,
        "KEY_9", GLFW_KEY_9,
        "KEY_A", GLFW_KEY_A,
        "KEY_B", GLFW_KEY_B,
        "KEY_C", GLFW_KEY_C,
        "KEY_D", GLFW_KEY_D,
        "KEY_E", GLFW_KEY_E,
        "KEY_F", GLFW_KEY_F,
        "KEY_G", GLFW_KEY_G,
        "KEY_H", GLFW_KEY_H,
        "KEY_I", GLFW_KEY_I,
        "KEY_J", GLFW_KEY_J,
        "KEY_K", GLFW_KEY_K,
        "KEY_L", GLFW_KEY_L,
        "KEY_M", GLFW_KEY_M,
        "KEY_N", GLFW_KEY_N,
        "KEY_O", GLFW_KEY_O,
        "KEY_P", GLFW_KEY_P,
        "KEY_Q", GLFW_KEY_Q,
        "KEY_R", GLFW_KEY_R,
        "KEY_S", GLFW_KEY_S,
        "KEY_T", GLFW_KEY_T,
        "KEY_U", GLFW_KEY_U,
        "KEY_V", GLFW_KEY_V,
        "KEY_W", GLFW_KEY_W,
        "KEY_X", GLFW_KEY_X,
        "KEY_Y", GLFW_KEY_Y,
        "KEY_Z", GLFW_KEY_Z,
        "KEY_SPACE", GLFW_KEY_SPACE,
        "KEY_ESCAPE", GLFW_KEY_ESCAPE
    );*/
}

void ScriptingSystem::Update(double /*dt*/)
{
    //for (int step = 0; step <= Engine::currentNumberOfSteps - 1; ++step)
    ////{
    //Timer::GetTime(startTime);

    ////// If "Pause" is checked
    ////if (g_engine->gameStateMgr->isPaused)
    ////{
    ////    return;
    ////}

    ////if (g_engine->gameStateMgr->isPlaying)
    ////{
    //    // Load the scripts and call the "Start" function
    //    if (!once)
    //    {
    //        for (auto& objects : g_engine->scriptingSys->entities)
    //        {
    //            Scripts* scripts = g_engine->entityMgr->GetComponent<Scripts>(objects);
    //            for (auto& i : scripts->scriptsContainer)
    //            {
    //                int entityID = objects;
    //                i.Load(entityID);
    //                i.Run("Start");
    //            }
    //        }
    //        once = true;
    //    }

    //    // Call the "Update" function
    //    for (auto& objects : g_engine->scriptingSys->entities)
    //    {
    //        Scripts* scripts = g_engine->entityMgr->GetComponent<Scripts>(objects);
    //        for (auto& i : scripts->scriptsContainer)
    //        {
    //            i.Run("Update");
    //        }
    //    }
    ////}
    ////else
    ////{
    //    // load scripts when scene is not playing & left-shift is pressed
    //    if (g_engine->inputsys->iskeypress(glfw_key_left_shift))
    //    {
    //        for (auto& objects : g_engine->scriptingsys->entities)
    //        {
    //            Scripts* scripts = g_engine->entitymgr->getcomponent<scripts>(objects);
    //            for (auto& i : scripts->scriptscontainer)
    //            {
    //                int entityid = objects;
    //                i.load(entityid);
    //            }
    //        }
    //    }
    //    once = false;
    ////}

    //Timer::GetTime(endTime);
    //Timer::UpdateSystemMs(SystemType<ScriptingSystem>(), startTime, endTime);
    //}
}

void ScriptingSystem::Free()
{
    // Empty for now
}
