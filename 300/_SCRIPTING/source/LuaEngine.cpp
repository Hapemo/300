#include "LuaEngine.h"
#include "ScriptingSystem.h"
#include "Input.h"
#include "ECS/ECS_Systems.h"

void LuaEngine()
{

}

void LuaInput()
{
    // Function pointer for 2 input functions
    //bool (*CheckKeyPointer)(E_STATE, E_KEY);
    //CheckKeyPointer = Input::CheckKey;
    //
    //double (*GetScrollPointer)();
    //GetScrollPointer = Input::GetScroll;

    systemManager->scriptingSystem->luaState.new_usertype<Input>(
        "Input", sol::constructors<>(),
        "CheckKey", &Input::CheckKey,
        "GetScroll", &Input::GetScroll
        );
}
