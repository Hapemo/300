#include "LuaEngine.h"
#include "ScriptingSystem.h"
#include "../../_ENGINE/include/Physics/PhysicsSystem.h"
#include "Input.h"
#include "ECS/ECS_Systems.h"

void LuaEngine()
{
    systemManager->mScriptingSystem->luaState["systemManager"] = systemManager;

    systemManager->mScriptingSystem->luaState.new_usertype<SystemManager>(
        "SystemManager", sol::constructors<>(),
        "ecs", &SystemManager::ecs,
        "mPhysicsSystem", &SystemManager::GetPhysicsPointer
        );
}

void LuaEntity()
{
    systemManager->mScriptingSystem->luaState.new_usertype<ECS>(
        "ecs", sol::constructors<>(),
        "NewEntity", &ECS::NewEntity,
        "DeleteEntity", &ECS::DeleteEntity,
        "GetGeneral", &ECS::GetEntitiesWith<General>,
        "GetTransform", &ECS::GetEntitiesWith<Transform>,
        "GetRigidBody", &ECS::GetEntitiesWith<RigidBody>,
        "GetScripts", &ECS::GetEntitiesWith<Scripts>
        );
}

void LuaGeneral()
{
    systemManager->mScriptingSystem->luaState.new_usertype<General>(
        "General", sol::constructors<>(),
        "name", &General::name,
        "tag", &General::tag,
        "subtag", &General::subtag,
        "isActive", &General::isActive
        );
}

void LuaTransform()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Transform>(
        "Transform", sol::constructors<>(),
        "mScale", &Transform::mScale,
        "mRotate", &Transform::mRotate,
        "mTranslate", &Transform::mTranslate,
        "data", &Transform::data
        );
}

void LuaRigidBody()
{
    systemManager->mScriptingSystem->luaState.new_usertype<RigidBody>(
        "RigidBody", sol::constructors<>(),
        "mMaterial", &RigidBody::mMaterial
        );
}

void LuaScript()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Scripts>(
        "Scripts", sol::constructors<>(),
        "AddScript", &Scripts::AddScript,
        "scriptFile", &Scripts::mScriptFile
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
