/*!*************************************************************************
****
\file ScriptingSystem.cpp
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 28-9-2023
\brief
This file contains scripting system class, where the logic
of how scripts will be loaded, read and unloaded is done.

The functions
- Init()
Registers scripting system and lua state. Data like enums
will be passed to lua here. Init also calls the functions
from file LuaEngine, where it contains the passing in
of more data.

- Update()
Updates scripting system by ensuring that the scripts
load when appropriate.

- LoadHelper()
Load Helper.lua script into sol. Helper.lua enables
developers to add in Helper functions while scripting
in lua.

- Script Reload()
Implemented when the reload button on the editor is pressed,
this reloads the script and any new changes to it will be
processed. (Hot reloads)
****************************************************************************
***/
#include "LuaEngine.h"
#include "ScriptingSystem.h"
#include "Debug/Logger.h"
#include "Debug/EnginePerformance.h"
#include "GameState/GameStateManager.h"
#include "FPSManager.h"

void ScriptingSystem::Init()
{
    luaState.open_libraries();

    // Function to set a lua table as readyonly
    luaState.script(R"(function readonlytable(table)
                      return setmetatable({}, {
                             __index = table,
                             __newindex = function(table, key, value)
                                         error("Attempt to modify read-only table")
                                         end,
                            __metatable = false
                       });
                    end)");

    // Load Helper.lua
    LoadHelper();

    /******************************************************************************/
    /*!
        Register Vec2, Vec3, Vec4
     */
     /******************************************************************************/

    luaState.new_usertype<glm::vec2>(
        "Vec2", sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
        "x", &glm::vec2::x,
        "y", &glm::vec2::y
    );

    luaState.new_usertype<glm::vec3>(
        "Vec3", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z
    );

    luaState.new_usertype<glm::vec4>(
        "Vec4", sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
        "x", &glm::vec4::x,
        "y", &glm::vec4::y,
        "z", &glm::vec4::z,
        "w", &glm::vec4::w
    );

    /******************************************************************************/
    /*!
        Insert engine functions to LUA
     */
     /******************************************************************************/
    LuaEngine();
    LuaECS();
    LuaEntity();
    LuaGeneral();
    LuaCamera();
    LuaFPSManager();
    LuaTransform();
    LuaAnimator();
    LuaRigidBody();
    LuaBoxCollider();
    LuaSphereCollider();
    LuaScript();
    LuaScripts();
    LuaParent();
    LuaChildren();
    LuaInput();
    LuaAudio();
    LuaInputMapSystem();
    LuaGraphicsSystem();
    LuaPhysics();
    LuaScripting();
    LuaPointLight();
    LuaMeshRenderer();
    LuaVFX();
    LuaGameState();
    LuaAIManager();
    LuaUIrenderer();
    LuaAISetting();
    LuaHealth();
    LuaButton();

    /******************************************************************************/
    /*!
        Insert input system to LUA
     */
     /******************************************************************************/
    luaState.new_enum("State",
        "HOLD", E_STATE::HOLD,
        "NOTPRESS", E_STATE::NOTPRESS, "PRESS", E_STATE::PRESS,
        "RELEASE", E_STATE::RELEASE);
    luaState.new_enum("Key",
        "MOUSE_LEFT", E_KEY::M_BUTTON_L, "MOUSE_RIGHT", E_KEY::M_BUTTON_R,
        "KEY_0", E_KEY::_0, "KEY_1", E_KEY::_1,
        "KEY_2", E_KEY::_2, "KEY_3", E_KEY::_3,
        "KEY_4", E_KEY::_4, "KEY_5", E_KEY::_5,
        "KEY_6", E_KEY::_6, "KEY_7", E_KEY::_7,
        "KEY_8", E_KEY::_8, "KEY_9", E_KEY::_9,
        "KEY_A", E_KEY::A, "KEY_B", E_KEY::B,
        "KEY_C", E_KEY::C, "KEY_D", E_KEY::D,
        "KEY_E", E_KEY::E, "KEY_F", E_KEY::F,
        "KEY_G", E_KEY::G, "KEY_H", E_KEY::H,
        "KEY_I", E_KEY::I, "KEY_J", E_KEY::J,
        "KEY_K", E_KEY::K, "KEY_L", E_KEY::L,
        "KEY_M", E_KEY::M, "KEY_N", E_KEY::N,
        "KEY_O", E_KEY::O, "KEY_P", E_KEY::P,
        "KEY_Q", E_KEY::Q, "KEY_R", E_KEY::R,
        "KEY_S", E_KEY::S, "KEY_T", E_KEY::T,
        "KEY_U", E_KEY::U, "KEY_V", E_KEY::V,
        "KEY_W", E_KEY::W, "KEY_X", E_KEY::X,
        "KEY_Y", E_KEY::Y, "KEY_Z", E_KEY::Z,
        "KEY_SPACE", E_KEY::SPACE, "KEY_ESCAPE", E_KEY::ESCAPE,
        "UP", E_KEY::UP, "DOWN", E_KEY::DOWN, "LEFT", E_KEY::LEFT, "RIGHT", E_KEY::RIGHT);

    luaState.new_enum("AudioType",
        "AUDIO_BGM", AUDIOTYPE::AUDIO_BGM,
        "AUDIO_SFX", AUDIOTYPE::AUDIO_SFX,
        "AUDIO_NULL", AUDIOTYPE::AUDIO_NULL,
        "UNDEFINED", AUDIOTYPE::UNDEFINED);

    luaState.new_enum("MaterialType",
        "DIFFUSE", MaterialType::DIFFUSE,
        "NORMAL", MaterialType::NORMAL,
        "SHININESS", MaterialType::SHININESS,
        "EMISSION", MaterialType::EMISSION,
        "SPECULAR", MaterialType::SPECULAR);

    luaState.new_enum("MATERIAL",
        "RUBBER", MATERIAL::RUBBER,
        "WOOD", MATERIAL::WOOD,
        "METAL", MATERIAL::METAL,
        "ICE", MATERIAL::ICE,
        "CONCRETE", MATERIAL::CONCRETE,
        "GLASS", MATERIAL::GLASS,
        "UNDEFINED", MATERIAL::UNDEFINED);

    luaState.new_enum("SUBTAG",
        "ACTIVE", SUBTAG::ACTIVE,
        "BACKGROUND", SUBTAG::BACKGROUND,
        "UNDEFINED", SUBTAG::UNDEFINED);
}

void ScriptingSystem::Update(float dt)
{
    auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();

    // Load the scripts and call the "Start" function 
    //if (!once)
    //{
    //    for (Entity entity : scriptEntities)
    //    {
    //        for (Script script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
    //        {
    //            script.Load(entity);
    //            script.Run("Alive");
    //        }
    //    }
    //    once = true;
    //}

    // Call the "Update" function 
    for (Entity entity : scriptEntities)
    {   
        for (Script* script : entity.GetComponent<Scripts>().scriptsContainer)
        {
            script->Run("Update");
        }
    }
}

void ScriptingSystem::Exit()
{
    // Empty for now
}

void ScriptingSystem::LoadHelper()
{
    sol::environment env = { systemManager->mScriptingSystem->luaState, sol::create, systemManager->mScriptingSystem->luaState.globals() };

    sol::protected_function_result result = systemManager->mScriptingSystem->luaState.script_file("../assets/Scripts/Helper.lua", env);
    if (!result.valid())
    {
        // print what error was invoked when the script was loading
        sol::error err = result;
        std::cout << "Error opening file!" << std::endl;
        std::cout << err.what() << std::endl;
    }

    luaState["Helper"] = env["Helper"];
    luaState.script("Helper = readonlytable(Helper)");
}

void ScriptingSystem::TestingFromScriptSys(std::vector<std::string> const& vect)
{
    if (vect.size() <= 0) return;
    for (auto& elem : vect)
    {
        std::cout << elem << std::endl;
    }
}

void ScriptingSystem::ScriptReload()
{
    once = false;
}

std::vector<std::string> ScriptingSystem::CreateVectorString()
{
    return std::vector<std::string>{};
}
