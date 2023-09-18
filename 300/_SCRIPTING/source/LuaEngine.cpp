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

void LuaECS()
{
    systemManager->mScriptingSystem->luaState.new_usertype<ECS>(
        "ecs", sol::constructors<>(),
        "NewEntity", &ECS::NewEntity,
        "DeleteEntity", &ECS::DeleteEntity,
        "GetEntitiesWithGeneral", &ECS::GetEntitiesWith<General>,
        "GetEntitiesWithTransform", &ECS::GetEntitiesWith<Transform>,
        "GetEntitiesWithRigidBody", &ECS::GetEntitiesWith<RigidBody>,
        "GetEntitiesWithBoxCollider", &ECS::GetEntitiesWith<BoxCollider>,
        "GetEntitiesWithScripts", &ECS::GetEntitiesWith<Scripts>
        );
}

void LuaEntity()
{
    //systemManager->mScriptingSystem->luaState.new_usertype<Entity>(
    //    "Entity", sol::constructors<>(),
    //    "id", &Entity::id,
    //    ////"GetGeneralComponent", &Entity::LuaGetComponent<General>,
    //    "HasGeneralComponent", &Entity::HasComponent <General>,
    //    ////"GetTransformComponent", &Entity::LuaGetComponent<Transform>,
    //    //"HasTransformComponent", &Entity::HasComponent<Transform>,
    //    "HasRigidBodyComponent", &Entity::HasComponent<RigidBody>,
    //    //"GetRigidBodyComponent", sol::resolve<RigidBody& ()>(&Entity::GetComponent<RigidBody>)
    //    "GetBoxColliderComponent", &Entity::LuaGetComponent<BoxCollider>,
    //    "HasBoxColliderComponent", &Entity::HasComponent<BoxCollider>,
    //    //"GetRigidBodyComponent", static_cast<RigidBody & (Entity::*) ()>(&Entity::GetComponent<RigidBody>),
    //    //"GetSphereColliderComponent", &Entity::LuaGetComponent<SphereCollider>
    //    //"HasSphereColliderComponent", &Entity::HasComponent<SphereCollider>
    //    //"GetPlaneColliderComponent", &Entity::LuaGetComponent<PlaneCollider>,
    //    //"HasPlaneColliderComponent", &Entity::HasComponent<PlaneCollider>,
    //    //"GetScriptsComponent", &Entity::LuaGetComponent<Scripts>,
    //    "HasScriptsComponent", &Entity::HasComponent<Scripts>
    //    //"GetParentComponent", &Entity::LuaGetComponent<Parent>,
    //    //"HasParentComponent", &Entity::HasComponent<Parent>,
    //    //"GetChildrenComponent", &Entity::LuaGetComponent<Children>,
    //    //"HasChildrenComponent", &Entity::HasComponent<Children>
    //    //"GetComponents", &Entity::LuaGetComponents<sol::variadic_args>
    //    );

   //systemManager->mScriptingSystem->luaState.set_function("AddRigidBodyComponent", static_cast<RigidBody& (Entity::*) ()>(&Entity::AddComponent<RigidBody>));
   // systemManager->mScriptingSystem->luaState.set_function("GetRigidBodyComponent", sol::resolve<RigidBody& (Entity)>(&Entity::LuaGetComponent<RigidBody>));
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
        "mTranslate", &Transform::mTranslate
        );
}

void LuaRigidBody()
{
    systemManager->mScriptingSystem->luaState.new_usertype<RigidBody>(
        "RigidBody", sol::constructors<>(),
        "mMaterial", &RigidBody::mMaterial,
        "mMotion", &RigidBody::mMass
        );
}

void LuaBoxCollider()
{
    systemManager->mScriptingSystem->luaState.new_usertype<BoxCollider>(
        "BoxCollider", sol::constructors<>(),
        "mScaleOffset", &BoxCollider::mScaleOffset,
        "mTranslateOffset", &BoxCollider::mTranslateOffset
        );
}

void LuaScript()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Scripts>(
        "Scripts", sol::constructors<>(),
        "AddScript", &Scripts::AddScript,
        "mScriptFile", &Scripts::mScriptFile
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
