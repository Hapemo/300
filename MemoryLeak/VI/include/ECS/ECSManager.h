/*!*****************************************************************************
\file ECSManager.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains the initialisation of ECS system. Registering all systems and
components.
TODO: take note not to change the component registration order. It will break prefab
*******************************************************************************/
#pragma once
#include "pch.h"
#include "ECS_tools.h"

#include "Graphics/TransformManager.h"
#include "Graphics/SpriteManager.h"
//#include "Graphics/RenderManager.h"
#include "Graphics/RenderManager.h"
#include "ColliderTransformManager.h"

#include "../Physics/Physics2DManager.h"
#include "../Physics/Collision2DManager.h"
#include "../LayerManager.h"

#include "EditorManager.h"
#include "Graphics/Animator.h"
#include "Graphics/SheetAnimator.h"
#include "AudioManager.h"
#include "Serialization.h"
#include "DialogManager.h"
#include "AI.h"
#include "LogicSystem.h"
#include "Graphics/ShadowManager.h"
#include "ButtonManager.h"
#include "MovementAIManager.h"
#include "ColorAIManager.h"
#include "PlayerDataManager.h"
#include "ParticleManager.h"

//-------------------------------------------------------------------
// System pointers with declaration from ECSManager.cpp
//-------------------------------------------------------------------
extern std::shared_ptr<LifespanManager> lifespanManager;
extern std::shared_ptr<TransformManager> transformManager;
extern std::shared_ptr<SpriteManager> spriteManager;
extern std::shared_ptr<RenderManager> renderManager;
extern std::shared_ptr<Animator> animator;
extern std::shared_ptr<SheetAnimator> sheetAnimator;
extern std::shared_ptr<Physics2DManager> physics2DManager;
extern std::shared_ptr<Collision2DManager> collision2DManager;
extern std::shared_ptr<LayerManager> layerManager;
extern std::shared_ptr<EditorManager> editorManager;
extern std::shared_ptr<AudioManager> audioManager;
extern std::shared_ptr<SerializationManager> serializationManager;
extern std::shared_ptr<DialogManager> dialogManager;
extern std::shared_ptr<AIManager> aiManager;
extern std::shared_ptr<LogicSystem> logicSystem;
extern std::shared_ptr<ShadowManager> shadowManager;
extern std::shared_ptr<ButtonManager> buttonManager;
extern std::shared_ptr<MovementAIManager> movementAIManager;
extern std::shared_ptr<ColorAIManager> colorAIManager;
extern std::shared_ptr<PlayerDataManager> playerDataManager;
extern std::shared_ptr<ParticleManager> particleManager;

/*!*****************************************************************************
\brief
ECSManager encapsulates the functions to register all managers and components 
in ECS
*******************************************************************************/
class ECSManager {
private:

	//-------------------------------------------------------------------
	// All these functions are to register the system with correct 
	// components and signatures
	//-------------------------------------------------------------------
	static void RegisterLifespanManager();
	static void RegisterTransformManager();
	static void RegisterSpriteManager();
	static void RegisterRenderManager();
	static void RegisterAnimator();
	static void RegisterSheetAnimator();
	static void RegisterPhysics2DManager();
	static void RegisterCollision2DManager();
	static void RegisterLayerManager();
	static void RegisterEditorManager();
	static void RegisterAudioManager();
	static void RegisterSerializationManager();
	static void RegisterDialogManager();
	static void RegisterAIManager();
	static void RegisterLogicSystem();
	static void RegisterShadowManager();
	static void RegisterButtonManager();
	static void RegisterMovementAIManager();
	static void RegisterColorAIManager();
	static void RegisterPlayerDataManager();
	static void RegisterParticleManager();
	
	// More to come

	/*!*****************************************************************************
	Register the all systems to ECS
	*******************************************************************************/
	static void RegisterAllSystems();

	/*!*****************************************************************************
	Register the all components to ECS
	*******************************************************************************/
	static void RegisterAllComponents();
public:

	/*!*****************************************************************************
	Initialise ECS with all systems and components. Should be called once before 
	the ECS system.
	*******************************************************************************/
	static void ECS_init();
};
