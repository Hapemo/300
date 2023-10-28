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

- Script Alive()/Start()/Exit()/Dead()
These functions run the respective script function (like Alive())
of all scripts.

- Test SSSU
For testing of the above functions created.

- Script Reload
Implemented when the reload button on the editor is pressed,
this reloads the script and any new changes to it will be
processed. (Hot reloads)

- Scripting Init Test and Scripting Update Test
These functions are called when testing outside of the editor
has to be done (Attachment of scripts and running of scripts)
****************************************************************************
***/
#include "LuaEngine.h"
#include "ScriptingSystem.h"
#include "Debug/Logger.h"
#include "CustomCompCont.h"
#include "Debug/EnginePerformance.h"
#include "GameState/GameStateManager.h"

bool ScriptingSystem::printOnce = false;

namespace sol {
    template <>
    struct is_container<CustomCompCont> : std::false_type { };
}

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
        Vec2 and Vec3 library
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


    luaState.new_usertype<CustomCompCont>("CustomCompCont",
        sol::constructors<CustomCompCont>(),
        "begin", &CustomCompCont::begin,
        "end", &CustomCompCont::end,
        "size", &CustomCompCont::size
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
    //LuaGFXCamera();
    LuaTransform();
    LuaAnimator();
    LuaRigidBody();
    LuaBoxCollider();
    LuaSphereCollider();
    LuaScript();
    LuaParent();
    LuaChildren();
    LuaInput();
    LuaAudioSystem();
    LuaAudioSource();
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
        for (Script script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
        {
            script.Run("Update");
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

void ScriptingSystem::ScriptAlive(const Entity& entity)
{
    auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();
    for (Script& script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
    {
        script.Load(entity);
        script.Run("Alive");
    }
}

void ScriptingSystem::ScriptStart(const Entity& entity)
{
    auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();
    for (Script& script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
    {
        script.Run("Start");
    }
}

void ScriptingSystem::ScriptExit(const Entity& entity)
{
    auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();
    for (Script& script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
    {
        script.Run("Exit");
    }
}

void ScriptingSystem::ScriptDead(const Entity& entity)
{
    auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();
    for (Script& script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
    {
        script.Run("Dead");
    }
}

void ScriptingSystem::LoadRunScript(Entity& entity)
{
    auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();
    for (Entity entity : scriptEntities)
    {
        for (Script script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
        {
            script.Load(entity);
            script.Run("Alive");
        }
    }
}

void ScriptingSystem::AddScript(Entity& entity, std::string fileName)
{
    // Check if there is script component, else add it before the script is added
    if (!entity.HasComponent<Scripts>())
        entity.AddComponent<Scripts>();
    Script temp;
    std::replace(fileName.begin(), fileName.end(), '\\', '/');
    temp.scriptFile = fileName;
    temp.env = { systemManager->mScriptingSystem->luaState, sol::create, systemManager->mScriptingSystem->luaState.globals() };
    temp.Load(entity);
    ScriptAlive(entity);
    entity.GetComponent<Scripts>().scriptsContainer.push_back(temp);
}

void ScriptingSystem::TestSSSU()
{
    //std::string entityID;
    //if (Input::CheckKey(E_STATE::PRESS, E_KEY::_4))
    //{
    //    std::cout << "Select entity that you want to run its functions in scripts: ";
    //    std::cin >> entityID;

    //    ScriptingSystem::ScriptAlive(Entity((entt::entity)std::stoul(entityID)));

    //    ScriptingSystem::ScriptStart(Entity((entt::entity)std::stoul(entityID)));

    //    ScriptingSystem::ScriptExit(Entity((entt::entity)std::stoul(entityID)));

    //    ScriptingSystem::ScriptDead(Entity((entt::entity)std::stoul(entityID)));
    //}
}

void ScriptingSystem::ScriptReload()
{
    once = false;
}

/******************************************************************************/
/*!
    These functions to be used when scripting is to be tested without
    editor present
 */
 /******************************************************************************/
void ScriptingSystem::ScriptingInitTest()
{
    //Entity player = systemManager->ecs->NewEntity();
    //std::cout << "Player (EntityID: " << std::to_string(unsigned int(player.id)) << ")added." << std::endl;
    //Entity enemy1 = systemManager->ecs->NewEntity();
    //std::cout << "Enemy1 (EntityID: " << std::to_string(unsigned int(enemy1.id)) << ")added." << std::endl;
    //Entity enemy2 = systemManager->ecs->NewEntity();
    //std::cout << "Enemy2 (EntityID: " << std::to_string(unsigned int(enemy2.id)) << ")added." << std::endl;

    //player.AddComponent<Transform>();
    //player.AddComponent<Scripts>();

    //enemy1.AddComponent<Transform>();
    //enemy1.AddComponent<Scripts>();

    //enemy2.AddComponent<Scripts>();
}

void ScriptingSystem::ScriptingUpdateTest()
{
    // Get all exisiting entities
    //if (!printOnce)
    //{
    //    for (auto& elem : systemManager->ecs->GetEntitiesWith<General>())
    //    {

    //        std::cout << "Player (EntityID: " << std::to_string(unsigned int(elem)) << ")added." << std::endl;
    //    }
    //    std::cout << std::endl;
    //    printOnce = true;
    //}
    //if (Input::CheckKey(E_STATE::PRESS, E_KEY::Q))
    //{
    //    Entity entity = systemManager->ecs->NewEntity();
    //    /*std::cout << entity.GetComponent<General>().name << std::endl;*/
    //}

    //if (Input::CheckKey(E_STATE::PRESS, E_KEY::T))
    //    printOnce = false;
    //// Button press number 1 to add new script 
    //if (Input::CheckKey(E_STATE::PRESS, E_KEY::_1))
    //{
    //    std::string scriptName, line;
    //    // get prefered name for script 
    //    std::cout << "Enter new script name: ";
    //    std::getline(std::cin, scriptName);
    //    std::stringstream ss;
    //    std::ifstream defScript{ "../assets/Scripts/DefaultTemplate.lua" };
    //    std::ofstream output;
    //    ss << "../assets/Scripts/" << scriptName << ".lua";
    //    // create the script 
    //    std::string path = ss.str();
    //    output.open(path.c_str(), std::ios_base::out);
    //    // copy the default template into the newly created script 
    //    while (getline(defScript, line))
    //    {
    //        output << line << std::endl;
    //    }
    //    std::cout << scriptName << ".lua added to assets/Scripts." << std::endl;
    //}

    // Button press number 2 to add script to entity 
    //if (Input::CheckKey(E_STATE::PRESS, E_KEY::_2))
    //{
    //    std::string entityID, scriptName;
    //    std::cout << "Indicate entity (by their id) to add script to: ";
    //    std::cin >> entityID;
    //    std::cout << "Enter script name to add to entity(Reference from Resources/Scripts: ";
    //    std::cin >> scriptName;
    //    std::string temp = "../assets/Scripts/" + scriptName + ".lua";
    //    auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();
    //    for (Entity entity : scriptEntities)
    //    {
    //        if (entity.id == static_cast<entt::entity>(std::stoul(entityID)))
    //        {
    //            // if entity does not contain any script, just add 
    //            if (scriptEntities.get<Scripts>(entity.id).scriptsContainer.size() == 0)
    //            {
    //                Script script;
    //                script.scriptFile = "../assets/Scripts/" + scriptName + ".lua";
    //                script.env = { systemManager->mScriptingSystem->luaState, sol::create, systemManager->mScriptingSystem->luaState.globals() };
    //                scriptEntities.get<Scripts>(entity.id).scriptsContainer.push_back(script);
    //                std::cout << "Script " << scriptName << ".lua added to entity " << entityID << std::endl;
    //            }
    //            // if entity already has scripts attached, check if duplicate 
    //            else
    //            {
    //                bool hasScript{ };

    //                for (auto& elem : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
    //                {
    //                    if (elem.scriptFile == temp)
    //                    {
    //                        hasScript = true;
    //                        std::cout << "Script is already attached! " << std::endl;
    //                        break;
    //                    }
    //                }

    //                if (!hasScript)
    //                {
    //                    Script script;
    //                    script.scriptFile = "../assets/Scripts/" + scriptName + ".lua";
    //                    script.env = { systemManager->mScriptingSystem->luaState, sol::create, systemManager->mScriptingSystem->luaState.globals() };
    //                    scriptEntities.get<Scripts>(entity.id).scriptsContainer.push_back(script);
    //                    std::cout << "Script " << scriptName << ".lua added to entity " << entityID << std::endl;
    //                }
    //            }
    //        }
    //    }
    //}

    ////Print all scripts that each entity has 
    //if (Input::CheckKey(E_STATE::PRESS, E_KEY::_3))
    //{
    //    std::cout << "/******************************************************************************/" << std::endl;
    //    auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();
    //    for (Entity entity : scriptEntities)
    //    {
    //        for (Script script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
    //        {
    //            std::cout << "Entity " << std::to_string(unsigned int(entity.id)) << " has script: " << script.scriptFile << std::endl;;
    //        }
    //    }
    //}
}
