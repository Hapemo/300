/*!*************************************************************************
****
\file LuaEngine.cpp
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 28-9-2023
\brief
This file ports in engine functions to Lua, will be updated as scripts
require more engine functionalities.
****************************************************************************
***/
#include "LuaEngine.h"
#include "ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Input/Input.h"
#include "ECS/ECS_Systems.h"
#include "ECS/ECS_Components.h"
#include "Audio/AudioSystemRevamp.h"
#include "Input/InputMapSystem.h"
#include "GameState/GameStateManager.h"
#include "Graphics/GraphicsSystem.h"
#include "AI/AIManager.h"
#include "Graphics/Camera_Input.h"
#include "FPSManager.h"

void LuaEngine()
{
    systemManager->mScriptingSystem->luaState["systemManager"] = systemManager;

    systemManager->mScriptingSystem->luaState.new_usertype<SystemManager>(
        "SystemManager", sol::constructors<>(),
        "ecs", &SystemManager::ecs,
        "mPhysicsSystem", &SystemManager::GetPhysicsPointer,
        "mScriptingSystem", &SystemManager::GetScriptingPointer,
        "mAudioSystem", &SystemManager::GetAudioPointer,
        "mInputActionSystem", &SystemManager::GetInputMapSystemPointer,
        "mGameStateSystem", &SystemManager::GetGameStateSystem,
        "mGraphicsSystem", &SystemManager::GetGraphicsSystem,
        "mAISystem", &SystemManager::GetAIManager,
        "Quit", &SystemManager::Quit);
}

void LuaECS()
{
    systemManager->mScriptingSystem->luaState.new_usertype<ECS>(
        "ecs", sol::constructors<>(),
        "NewEntity", &ECS::NewEntity,
        "NewEntityByScene", &ECS::NewEntityByScene,
        "NewEntityFromPrefab", &ECS::NewEntityFromPrefab,
        "SetDeleteEntity", &ECS::SetDeleteEntity);
}

#define DECLARE_COMPONENT(str, type) str, sol::resolve<type &()>(&Entity::GetComponent<type>)
#define ADD_COMPONENT(str, type) str, sol::resolve<type &()>(&Entity::AddComponent<type>)

void LuaEntity()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Entity>(
        "Entity", sol::constructors<Entity(std::uint32_t)>(),
        "id", &Entity::id,
        DECLARE_COMPONENT("GetGeneral", General),
        "HasGeneral", &Entity::HasComponent<General>,

        DECLARE_COMPONENT("GetTransform", Transform),
        "HasTransform", &Entity::HasComponent<Transform>,

        DECLARE_COMPONENT("GetCamera", Camera),
        "HasCamera", &Entity::HasComponent<Camera>,

        ADD_COMPONENT("AddAnimator", Animator),
        DECLARE_COMPONENT("GetAnimator", Animator),
        "HasAnimator", &Entity::HasComponent<Animator>,

        ADD_COMPONENT("AddRigidBody", RigidBody),
        DECLARE_COMPONENT("GetRigidBody", RigidBody),
        "HasRigidBody", &Entity::HasComponent<RigidBody>,

        ADD_COMPONENT("AddBoxCollider", BoxCollider),
        DECLARE_COMPONENT("GetBoxCollider", BoxCollider),
        "HasBoxCollider", &Entity::HasComponent<BoxCollider>,

        ADD_COMPONENT("AddSphereCollider", SphereCollider),
        DECLARE_COMPONENT("GetSphereCollider", SphereCollider),
        "HasSphereCollider", &Entity::HasComponent<SphereCollider>,

        ADD_COMPONENT("AddCapsuleCollider", CapsuleCollider),
        DECLARE_COMPONENT("GetCapsuleCollider", CapsuleCollider),
        "HasCapsuleCollider", &Entity::HasComponent<CapsuleCollider>,

        ADD_COMPONENT("AddAudio", Audio),
        DECLARE_COMPONENT("GetAudio", Audio),
        "HasAudio", &Entity::HasComponent<Audio>,

        ADD_COMPONENT("AddScripts", Scripts),
        DECLARE_COMPONENT("GetScripts", Scripts),
        "HasScripts", &Entity::HasComponent<Scripts>,

        ADD_COMPONENT("AddParent", Parent),
        DECLARE_COMPONENT("GetParent", Parent),
        "HasParent", &Entity::HasComponent<Parent>,

        ADD_COMPONENT("AddChildren", Children),
        DECLARE_COMPONENT("GetChildren", Children),
        "HasChildren", &Entity::HasComponent<Children>,

        ADD_COMPONENT("AddPointLight", PointLight),
        DECLARE_COMPONENT("GetPointLight", PointLight),
        "HasPointLight", &Entity::HasComponent<PointLight>,

        ADD_COMPONENT("AddMeshRenderer", MeshRenderer),
        DECLARE_COMPONENT("GetMeshRenderer", MeshRenderer),
        "HasMeshRenderer", &Entity::HasComponent<MeshRenderer>,

        ADD_COMPONENT("AddVFX", VFX),
        DECLARE_COMPONENT("GetVFX", VFX),
        "HasVFX", &Entity::HasComponent<VFX>,

        ADD_COMPONENT("AddButton", Button),
        DECLARE_COMPONENT("GetButton", Button),
        "HasButton", &Entity::HasComponent<Button>,

        ADD_COMPONENT("AddCamera", Camera),
        DECLARE_COMPONENT("GetCamera", Camera),
        "HasCamera", &Entity::HasComponent<Camera>,

        ADD_COMPONENT("AddUIrenderer", UIrenderer),
        DECLARE_COMPONENT("GetUIrenderer", UIrenderer),
        "HasUIrenderer", &Entity::HasComponent<UIrenderer>,

        ADD_COMPONENT("AddAISetting", AISetting),
        DECLARE_COMPONENT("GetAISetting", AISetting),
        "HasAISetting", &Entity::HasComponent<AISetting>,

        ADD_COMPONENT("AddHealthbar", Healthbar),
        DECLARE_COMPONENT("GetHealthbar", Healthbar),
        "HasHealthbar", &Entity::HasComponent<Healthbar>);
}

void LuaGeneral()
{
    systemManager->mScriptingSystem->luaState.new_usertype<General>(
        "General", sol::constructors<>(),
        "name", &General::name,
        "tagid", &General::tagid,
        "subtag", &General::subtag,
        "isActive", &General::isActive,
        "GetTag", &General::GetTag,
        "SetTag", &General::SetTag);
}

void LuaCamera()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Camera_Scripting>(
        "Camera_Scripting", sol::constructors<>(),
        "SetPosition", &Camera_Scripting::SetPosition,
        "SetTarget", &Camera_Scripting::SetTarget,
        "SetCameraSpeed", &Camera_Scripting::SetCameraSpeed,
        "SetSensitivity", &Camera_Scripting::SetSensitivity,
        "GetPosition", &Camera_Scripting::GetPosition,
        "GetTarget", &Camera_Scripting::GetTarget,
        "GetDirection", &Camera_Scripting::GetDirection,
        "GetCameraSpeed", &Camera_Scripting::GetCameraSpeed,
        "GetSensitivity", &Camera_Scripting::GetSensitivity,
        "RotateCameraView", &Camera_Scripting::RotateCameraView,
        "SetFov", &Camera_Scripting::SetFov,
        "Cross", &Camera_Scripting::Cross, 
        "Rotate" , &Camera_Scripting::Rotate);
}

void LuaFPSManager()
{
    systemManager->mScriptingSystem->luaState.new_usertype<FPSManager>(
        "FPSManager", sol::constructors<>(),
        "GetDT", &FPSManager::GetDT);
}

void LuaTransform()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Transform>(
        "Transform", sol::constructors<>(),
        "mScale", &Transform::mScale,
        "mRotate", &Transform::mRotate,
        "mTranslate", &Transform::mTranslate);
}

void LuaAnimator()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Animator>(
        "Animator", sol::constructors<>(),
        "PauseAnimation", &Animator::PauseAnimation,
        "UnpauseAnimation", &Animator::UnpauseAnimation);
}

void LuaRigidBody()
{
    systemManager->mScriptingSystem->luaState.new_usertype<RigidBody>(
        "RigidBody", sol::constructors<>(),
        "mDensity", &RigidBody::mDensity,
        "mMaterial", &RigidBody::mMaterial,
        "mVelocity", &RigidBody::mVelocity);
}

void LuaBoxCollider()
{
    systemManager->mScriptingSystem->luaState.new_usertype<BoxCollider>(
        "BoxCollider", sol::constructors<>(),
        "mScaleOffset", &BoxCollider::mScaleOffset,
        "mTranslateOffset", &BoxCollider::mTranslateOffset);
}

void LuaSphereCollider()
{
    systemManager->mScriptingSystem->luaState.new_usertype<SphereCollider>(
        "SphereCollider", sol::constructors<>(),
        "mScaleOffset", &SphereCollider::mScaleOffset,
        "mTranslateOffset", &SphereCollider::mTranslateOffset);
}

void LuaScript()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Script>(
        "Script", sol::constructors<>(),
        "ReturnValueInt", &Script::RunWithReturnValue<int>,
        "SetValueVec3", &Script::SetValue<glm::vec3>,
        "SetValueFloat", &Script::SetValue<float>);
}

void LuaScripts()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Scripts>(
        "Scripts", sol::constructors<>(),
        "GetScript", &Scripts::GetScript);
}

void LuaParent()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Parent>(
        "Parent", sol::constructors<>(),
        "mPrevSibling", &Parent::mPrevSibling,
        "mNextSibling", &Parent::mNextSibling,
        "mParent", &Parent::mParent);
}

void LuaChildren()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Children>(
        "Children", sol::constructors<>(),
        "mNumChildren", &Children::mNumChildren,
        "mFirstChild", &Children::mFirstChild);
}

void LuaInput()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Input>(
        "Input", sol::constructors<>(),
        "CheckKey", &Input::CheckKey,
        "GetScroll", &Input::GetScroll,
        "CursorPos", &Input::CursorPos,
        "SetCursorCenter", &Input::SetCursorCenter,
        "GetCursorCenter", &Input::GetCursorCenter);
}

void LuaAudio()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Audio>(
        "Audio", sol::constructors<>(),
        "SetPlay", &Audio::SetPlay,
        "SetPause", &Audio::SetPause,
        "SetResume", &Audio::SetResume,
        "SetStop", &Audio::SetStop,
        "UpdateVolume", &Audio::UpdateVolume,
        "FadeIn", &Audio::FadeIn,
        "FadeOut", &Audio::FadeOut);
}

void LuaInputMapSystem()
{
    systemManager->mScriptingSystem->luaState.new_usertype<InputMapSystem>(
        "mInputActionSystem", sol::constructors<>(),
        "GetButton", &InputMapSystem::GetButton,
        "GetButtonUp", &InputMapSystem::GetButtonUp,
        "GetButtonDown", &InputMapSystem::GetButtonDown,
        "GetKey", &InputMapSystem::GetKey,
        "GetKeyUp", &InputMapSystem::GetKeyUp,
        "GetKeyDown", &InputMapSystem::GetKeyDown);
}

void LuaGraphicsSystem()
{
    systemManager->mScriptingSystem->luaState.new_usertype<GraphicsSystem>(
        "mGraphicsSystem", sol::constructors<>(),
        "EnableGlobalBloom", &GraphicsSystem::EnableGlobalBloom,
        "DisableGlobalBloom", &GraphicsSystem::DisableGlobalBloom,
        "SetGlobalBloomThreshold", &GraphicsSystem::SetGlobalBloomThreshold,
        "SetGlobalBloomExposure", &GraphicsSystem::SetGlobalBloomExposure,
        "mAmbientBloomThreshold", &GraphicsSystem::mAmbientBloomThreshold,
        "mAmbientBloomExposure", &GraphicsSystem::mAmbientBloomExposure,
        "mTexelOffset", &GraphicsSystem::mTexelOffset,
        "mSamplingWeight", &GraphicsSystem::mSamplingWeight,
        "mChromaticOffset", &GraphicsSystem::mChromaticOffset,
        "mChromaticStrength", &GraphicsSystem::mChromaticStrength,
        "m_EnableBloom", &GraphicsSystem::m_EnableBloom,
        "m_EnableChromaticAbberation", &GraphicsSystem::m_EnableChromaticAbberation,
        "m_GlobalTint", &GraphicsSystem::m_GlobalTint,
        "m_WindowWidth", &GraphicsSystem::m_Width,
        "m_WindowHeight", &GraphicsSystem::m_Height,
        "SetAllEntityBloomThreshold", &GraphicsSystem::SetAllEntityBloomThreshold);
}

void LuaPhysics()
{
    systemManager->mScriptingSystem->luaState.new_usertype<PhysicsSystem>(
        "mPhysicsSystem", sol::constructors<>(),
        "SetVelocity", &PhysicsSystem::SetVelocity,
        "SetPosition", &PhysicsSystem::SetPosition,
        "SetRotation", &PhysicsSystem::SetRotation);
}

void LuaScripting()
{
    systemManager->mScriptingSystem->luaState.new_usertype<ScriptingSystem>(
        "mScriptingSystem", sol::constructors<>(),
        "TestingFromScriptSys", &ScriptingSystem::TestingFromScriptSys,
        "CreateVectorString", &ScriptingSystem::CreateVectorString);
}

void LuaPointLight()
{
    systemManager->mScriptingSystem->luaState.new_usertype<PointLight>(
        "PointLight", sol::constructors<>(),
        "SetColor", &PointLight::SetColor);
}

void LuaMeshRenderer()
{
    systemManager->mScriptingSystem->luaState.new_usertype<MeshRenderer>(
        "MeshRenderer", sol::constructors<>(),
        "SetColor", &MeshRenderer::SetColor,
        "SetMesh", &MeshRenderer::SetMesh,
        "SetMeshDelayed", &MeshRenderer::SetMeshDelayed,
        "SetTexture", &MeshRenderer::SetTexture);
}

void LuaVFX()
{
    systemManager->mScriptingSystem->luaState.new_usertype<VFX>(
        "VFX", sol::constructors<>(),
        "EnableObjectBloom", &VFX::EnableObjectBloom,
        "DisableObjectBloom", &VFX::DisableObjectBloom,
        "SetEntityBloomThreshold", &VFX::SetEntityBloomThreshold);
}

void LuaGameState()
{
    systemManager->mScriptingSystem->luaState.new_usertype<GameStateManager>(
        "mGameStateSystem", sol::constructors<>(),
        "GetEntity", sol::resolve<Entity(std::string const &)>(&GameStateManager::GetEntity),
        "GetEntityByScene", sol::resolve<Entity(std::string const &, std::string const &)>(&GameStateManager::GetEntity),
        "ChangeGameState", sol::resolve<void(const std::string &)>(&GameStateManager::ChangeGameState));
}

void LuaAIManager()
{
    systemManager->mScriptingSystem->luaState.new_usertype<AIManager>(
        "mAISystem", sol::constructors<>(),
        "SetPredictiveVelocity", &AIManager::SetPredictiveVelocity,
        "PredictiveShootPlayer", &AIManager::PredictiveShootPlayer,
        "GetDirection", &AIManager::GetDirection,
        "ConeOfSight", &AIManager::ConeOfSight,
        "LineOfSight", &AIManager::LineOfSight);
}

void LuaButton()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Button>(
        "Button", sol::constructors<>(),
        "IsInteractable", &Button::IsInteractable,
        "IsHovered", &Button::IsHovered,
        "IsClicked", &Button::IsClicked,
        "IsActivated", &Button::IsActivated);
}

void LuaUIrenderer()
{
    systemManager->mScriptingSystem->luaState.new_usertype<UIrenderer>(
        "UIrenderer", sol::constructors<>(),
        "SetDegree", &UIrenderer::SetDegree,
        "SetTexture", &UIrenderer::SetTexture,
        "mColor", &UIrenderer::mColor

    );
}

void LuaAISetting()
{
    systemManager->mScriptingSystem->luaState.new_usertype<AISetting>(
        "AISetting", sol::constructors<>(),
        "SetTarget", &AISetting::SetTarget,
        "GetTarget", sol::resolve<Entity()>(&AISetting::GetTarget),
        "GetTargetConst", sol::resolve<Entity() const>(&AISetting::GetTarget),
        "mTargetName", &AISetting::mTargetName,
        "mStayAway", &AISetting::mStayAway);
}

void LuaHealth()
{
    systemManager->mScriptingSystem->luaState.new_usertype<Healthbar>(
        "Healthbar", sol::constructors<>(),
        "maxHealth", &Healthbar::mMaxHealth,
        "health", &Healthbar::mHealth);
}
