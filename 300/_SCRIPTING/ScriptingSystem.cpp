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
//#include "LuaEngine.h"
//#include "LuaComponent.h"
//#include "../GameState/GameStateManager.h"
#include "ScriptingSystem.h"

/******************************************************************************/
/*!
    Example
 */
 /******************************************************************************/
//void print(const std::string& str)
//{
//    std::cout << str << std::endl;
//}
//
//void printVec(const Vec2& vec)
//{
//    std::cout << "Vec " << vec.x << "  " << vec.y << std::endl;
//}

sol::state ScriptingSystem::luaState;
bool ScriptingSystem::once;

void ScriptingSystem::Init()
{
    //g_engine->entityMgr->RegisterSystem(SystemType<ScriptingSystem>(), this);
    //AddComponentSignature<Scripts>();

    luaState.open_libraries();

    /******************************************************************************/
    /*!
        Example
     */
     /******************************************************************************/
    //luaState.set_function("print", print);
    //luaState.set_function("printVec", printVec);

    //luaState.new_usertype<Vec2>(
    //    "Vec2", sol::constructors<Vec2(), Vec2(float, float)>(),
    //    "x", &Vec2::x,
    //    "y", &Vec2::y
    //    );

    /******************************************************************************/
    /*!
        Insert engine functions to LUA
     */
     /******************************************************************************/
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


    /******************************************************************************/
    /*!
        Insert input system to LUA
     */
     /******************************************************************************/
    luaState.new_enum("InputKeys",
        "MOUSE_LEFT", E_KEY::M_BUTTON_L,
        "MOUSE_RIGHT", E_KEY::M_BUTTON_R,
        "KEY_0", E_KEY::_0,
        "KEY_1", E_KEY::_1,
        "KEY_2", E_KEY::_2,
        "KEY_3", E_KEY::_3,
        "KEY_4", E_KEY::_4,
        "KEY_5", E_KEY::_5,
        "KEY_6", E_KEY::_6,
        "KEY_7", E_KEY::_7,
        "KEY_8", E_KEY::_8,
        "KEY_9", E_KEY::_9,
        "KEY_A", E_KEY::A,
        "KEY_B", E_KEY::B,
        "KEY_C", E_KEY::C,
        "KEY_D", E_KEY::D,
        "KEY_E", E_KEY::E,
        "KEY_F", E_KEY::F,
        "KEY_G", E_KEY::G,
        "KEY_H", E_KEY::H,
        "KEY_I", E_KEY::I,
        "KEY_J", E_KEY::J,
        "KEY_K", E_KEY::K,
        "KEY_L", E_KEY::L,
        "KEY_M", E_KEY::M,
        "KEY_N", E_KEY::N,
        "KEY_O", E_KEY::O,
        "KEY_P", E_KEY::P,
        "KEY_Q", E_KEY::Q,
        "KEY_R", E_KEY::R,
        "KEY_S", E_KEY::S,
        "KEY_T", E_KEY::T,
        "KEY_U", E_KEY::U,
        "KEY_V", E_KEY::V,
        "KEY_W", E_KEY::W,
        "KEY_X", E_KEY::X,
        "KEY_Y", E_KEY::Y,
        "KEY_Z", E_KEY::Z,
        "KEY_SPACE", E_KEY::SPACE,
        "KEY_ESCAPE", E_KEY::ESCAPE
    );
}

void ScriptingSystem::Update()
{
    //for (int step = 0; step <= Engine::currentNumberOfSteps - 1; ++step)
    ////{
    //Timer::GetTime(startTime);

    //// If "Pause" is checked
    //if (g_engine->gameStateMgr->isPaused)
    //{
    //    return;
    //}

    auto scriptEntities = ECS::GetInstance()->GetEntitiesWith<Scripts>();

    //if (g_engine->gameStateMgr->isPlaying)
    //{
        // Load the scripts and call the "Start" function
        if (!once)
        {
            for (Entity entity : scriptEntities)
            {   
                for (Script script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
                {
                    script.Load(entity);
                    script.Run("Start");
                }
            }
            once = true;
        }

        // Call the "Update" function
        for (Entity entity : scriptEntities)
        {
            for (Script script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
            {
                script.Run("Update");
            }
        }
    //}
    /******************************************************************************/
    /*!
        To implement when game state manager is ready
        Load scripts when scene is not playing & left-shift is pressed
     */
     /******************************************************************************/
    //else
    //{
        //if (Input::CheckKey(E_STATE::PRESS, E_KEY::LEFT_SHIFT))
        //{
        //    for (Entity entity : ECS::GetInstance()->GetEntitiesWith<Scripts>())
        //    {
        //        for (auto& script : entity.GetComponent<Scripts>().scriptsContainer)
        //        {
        //            int entityid = entity.id;
        //            script.Load(entityid);
        //        }
        //    }
        //}
        //once = false;
    //}

    //Timer::GetTime(endTime);
    //Timer::UpdateSystemMs(SystemType<ScriptingSystem>(), startTime, endTime);
    //}
}

void ScriptingSystem::Exit()
{
    // Empty for now
}
