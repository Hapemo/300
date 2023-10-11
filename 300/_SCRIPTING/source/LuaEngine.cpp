#include "LuaEngine.h"
#include "ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Input/Input.h"
#include "ECS/ECS_Systems.h"
#include "ECS/ECS_Components.h"
#include "Audio/AudioSystem.h"

void LuaComponentContainer()
{

}

void LuaEngine()
{
    systemManager->mScriptingSystem->luaState["systemManager"] = systemManager;

    systemManager->mScriptingSystem->luaState.new_usertype<SystemManager>(
        "SystemManager", sol::constructors<>(),
        "ecs", &SystemManager::ecs,
        "mPhysicsSystem", &SystemManager::GetPhysicsPointer,
        "mAudioSystem", &SystemManager::GetAudioPointer
        );
}

void LuaECS()
{
    systemManager->mScriptingSystem->luaState.new_usertype<ECS>(
        "ecs", sol::constructors<>(),
        "NewEntity", &ECS::NewEntity,
        "DeleteEntity", &ECS::DeleteEntity
        //"GetEntitiesWithGeneral", &ECS::GetEntitiesWith<General>,
        //"GetEntitiesWithTransform", &ECS::GetEntitiesWith<Transform>,
        //"GetEntitiesWithRigidBody", &ECS::GetEntitiesWith<RigidBody>,
        //"GetEntitiesWithBoxCollider", &ECS::GetEntitiesWith<BoxCollider>,
        //"GetEntitiesWithScripts", &ECS::GetEntitiesWith<Scripts>
        );
}

#define DECLARE_COMPONENT(str, type) str, sol::resolve<type&()>(&Entity::GetComponent<type>)

void LuaEntity()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Entity>(
        "Entity", sol::constructors<Entity(std::uint32_t)>(),
        "id", &Entity::id,
        DECLARE_COMPONENT("GetGeneralComponent", General),
        "HasGeneralComponent", &Entity::HasComponent<General>,
        DECLARE_COMPONENT("GetTransformComponent", Transform),
        "HasTransformComponent", &Entity::HasComponent<Transform>,
        DECLARE_COMPONENT("GetRigidBodyComponent", RigidBody),
        "HasRigidBodyComponent", &Entity::HasComponent<RigidBody>,
        DECLARE_COMPONENT("GetBoxColliderComponent", BoxCollider),
        "HasBoxColliderComponent", &Entity::HasComponent<BoxCollider>,
        DECLARE_COMPONENT("GetSphereColliderComponent", SphereCollider),
        "HasSphereColliderComponent", &Entity::HasComponent<SphereCollider>,
        DECLARE_COMPONENT("GetPlaneColliderComponent", PlaneCollider),
        "HasPlaneColliderComponent", &Entity::HasComponent<PlaneCollider>,
        DECLARE_COMPONENT("GetScriptsComponent", Scripts),
        "HasScriptsComponent", &Entity::HasComponent<Scripts>,
        DECLARE_COMPONENT("GetParentComponent", Parent),
        "HasParentComponent", &Entity::HasComponent<Parent>,
        DECLARE_COMPONENT("GetChildrenComponent", Children),
        "HasChildrenComponent", &Entity::HasComponent<Children>
        );
}


void LuaGeneral()
{
    systemManager->mScriptingSystem->luaState.new_usertype<General>(
        "General", sol::constructors<>(),
        "name", &General::name,
        //"tagid", &General::tagid,
        //"subtag", &General::subtag,
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
        "mDensity", &RigidBody::mDensity,
        "mMaterial", &RigidBody::mMaterial,
        "mMotion", &RigidBody::mMotion,
        "mVelocity", &RigidBody::mVelocity
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

void LuaSphereCollider()
{
    systemManager->mScriptingSystem->luaState.new_usertype<SphereCollider>(
        "SphereCollider", sol::constructors<>(),
        "mScaleOffset", &SphereCollider::mScaleOffset,
        "mTranslateOffset", &SphereCollider::mTranslateOffset
        );
}

void LuaPlaneCollider()
{
    systemManager->mScriptingSystem->luaState.new_usertype<PlaneCollider>(
        "PlaneCollider", sol::constructors<>(),
        "mNormal", &PlaneCollider::mNormal,
        "mTranslateOffset", &PlaneCollider::mTranslateOffset
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

void LuaParent()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Parent>(
        "Parent", sol::constructors<>(),
        "mPrevSibling", &Parent::mPrevSibling,
        "mNextSibling", &Parent::mNextSibling,
        "mParent", &Parent::mParent
        );
}

void LuaChildren()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Children>(
        "Children", sol::constructors<>(),
        "mNumChildren", &Children::mNumChildren,
        "mFirstChild", &Children::mFirstChild
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

void LuaAudio()
{
    //systemManager->mScriptingSystem->luaState["mAudioSystem"] = systemManager->GetAudioPointer();
    systemManager->mScriptingSystem->luaState.new_usertype<AudioSystem>(
        "mAudioSystem", sol::constructors<>(),
        "PlayAudio", &AudioSystem::PlayAudio,
        "PlaySFXAudio", &AudioSystem::PlaySFXAudio,
        "PlayBGMAudio", &AudioSystem::PlayBGMAudio,
        "SetSpecificChannelVolume", &AudioSystem::SetSpecificChannelVolume,
        "SetAllSFXVolume", &AudioSystem::SetAllSFXVolume,
        "SetAllBGMVolume", &AudioSystem::SetAllBGMVolume,
        "MuteSFX", &AudioSystem::MuteSFX,
        "MuteBGM", &AudioSystem::MuteBGM,
        "TogglePauseAllSounds", &AudioSystem::TogglePauseAllSounds,
        "TogglePauseSFXSounds", &AudioSystem::TogglePauseSFXSounds,
        "TogglePauseBGMSounds", &AudioSystem::TogglePauseBGMSounds,
        "TogglePauseSpecific", &AudioSystem::TogglePauseSpecific
        );
}
