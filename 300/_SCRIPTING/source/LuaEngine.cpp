#include "LuaEngine.h"
#include "ScriptingSystem.h"
#include "Input.h"
#include "ECS/ECS_Systems.h"

void LuaEngine()
{
    systemManager->scriptingSystem->luaState["systemManager"] = systemManager;

    systemManager->scriptingSystem->luaState.new_usertype<SystemManager>(
        "SystemManager", sol::constructors<>()

        );
}

void LuaInput()
{
    systemManager->scriptingSystem->luaState.new_usertype<Input>(
        "Input", sol::constructors<>(),
        "CheckKey", &Input::CheckKey,
        "GetScroll", &Input::GetScroll
        );
}
