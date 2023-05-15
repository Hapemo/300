/*!*****************************************************************************
\file FPSPrint.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Course: GAM250
\par Group: Memory Leak Studios
\date 25-01-2023
\brief
Updates the fps count, for the fps printer in entity
*******************************************************************************/


#include "FPSPrint.h"

REGISTER_SCRIPT(ScriptComponent, FPSPrintScript);

namespace {
	Entity memoryFragment; // THIS SHOULD REMOVE WHEN REAL PARTICLE SYSTEM IS IMPLEMENTED
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void FPSPrintScript::Alive(Entity const& _e) {
	//memoryFragment = VI::iEntity::GetEntity("memoryfragment", "");
	//if (memoryFragment.id == 0) return;
	//memoryFragment.AddComponent(ParticleSystem{
	//	ParticleSystem::ParticleInfo{
	//		10, // mScale			
	//		90, // mFacing		
	//		3, // mLifespan	
	//		Sprite{
	//			Color(255,255,255,200), // color
	//			SPRITE::SQUARE, // sprit
	//			0, // texture
	//			memoryFragment.GetComponent<Sprite>().layer + 1  // layer
	//		}, // mSprite		
	//		2, // mRotation	
	//		20, // mSpeed			
	//		true, // mFading			
	//		true // mFadeIn
	//	},
	//	1, // mDensity 
	//	memoryFragment.GetComponent<Transform>().translation, // mCenter
	//	130, // mAreaWidth 
	//	90.f, // mDirection
	//	45.f, // mSpread 
	//	0.1f, // mDuration 
	//	true, // mIsActive 
	//	0.1f // mSlow
	//});

	//memoryFragment = VI::iEntity::GetEntity("Boat", "");
	//if (memoryFragment.id == 0) return;
	//memoryFragment.AddComponent(ParticleSystem{
	//	ParticleSystem::ParticleInfo{
	//		70, // mScale			
	//		90, // mFacing		
	//		8, // mLifespan	
	//		Sprite{
	//			Color(255,255,255,), // color
	//			SPRITE::SQUARE, // sprit
	//			0, // texture
	//			memoryFragment.GetComponent<Sprite>().layer + 1  // layer
	//		}, // mSprite		
	//		2, // mRotation	
	//		20, // mSpeed			
	//		true, // mFading			
	//		true // mFadeIn
	//	},
	//	1, // mDensity 
	//	memoryFragment.GetComponent<Transform>().translation, // mCenter
	//	130, // mAreaWidth 
	//	90.f, // mDirection
	//	45.f, // mSpread 
	//	0.1f, // mDuration 
	//	true, // mIsActive 
	//	0.1f // mSlow
	//	});

	//LOG_INFO(memoryFragment.GetComponent<General>().name + ": Added component particle system IMPROPERLY VIA C++============================================");
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void FPSPrintScript::Init(Entity const& _e) {
	(void)_e;
	//LOG_INFO("How to Play button script starts works!!!");
	VI::iSprite::SetColor(_e, 0, 0, 0, 0);
	VI::iText::UpdateText(_e, "");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void FPSPrintScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void FPSPrintScript::Update(Entity const& _e) {
	(void)_e;
	// Update every 2 seconds
	static float timeCounter{1};
	timeCounter += static_cast<float>(FPSManager::dt);
	if (timeCounter <= 2) return;
	timeCounter = 0;

	std::stringstream str;
	str << std::fixed << std::setprecision(1) << FPSManager::fps;
	//VI::iText::UpdateText("fpsDisplay", "", str.str());
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void FPSPrintScript::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void FPSPrintScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void FPSPrintScript::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("How to Play button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void FPSPrintScript::Dead(Entity const& _e) {
	(void)_e;
}
