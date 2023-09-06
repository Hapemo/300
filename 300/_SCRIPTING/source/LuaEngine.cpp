#include "LuaEngine.h"
#include "ScriptingSystem.h"
#include "../_TOOLS/include/Input.h"
#include "../_ENGINE/include/ECS/ECS_Systems.h"

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

    //ScriptingSystem::GetInstance()->luaState.new_usertype<Input>(
    //    "Input", sol::constructors<>(),
    //    "CheckKey", &Input::CheckKey,
    //    "GetScroll", &Input::GetScroll
    //    );
}
