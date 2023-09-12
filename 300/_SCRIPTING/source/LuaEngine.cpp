#include "LuaEngine.h"
#include "ScriptingSystem.h"
#include "Input.h"
#include "ECS/ECS_Systems.h"

void LuaEngine()
{
    systemManager->mScriptingSystem->luaState["systemManager"] = systemManager;

    systemManager->mScriptingSystem->luaState.new_usertype<SystemManager>(
        "SystemManager", sol::constructors<>()

        );
}

void LuaInput()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Input>(
        "Input", sol::constructors<>(),
        "CheckKey", &Input::CheckKey,
        "GetScroll", &Input::GetScroll
        );
}
