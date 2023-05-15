/*!*****************************************************************************
\file PlayGameButton.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-11-2022
\brief
The PlayGameButton script starts the game (level 1) on button press.
*******************************************************************************/

#include "PlayGameButton.h"

REGISTER_SCRIPT(ScriptComponent, PlayGameButton);

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void PlayGameButton::Alive(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void PlayGameButton::Init(Entity const& gob) {
	(void)gob;
	Entity e = gob;

	//e.AddComponent(ParticleSystem{
	//	ParticleSystem::ParticleInfo{
	//		20, // mScale			
	//		90, // mFacing		
	//		5, // mLifespan	
	//		Sprite{
	//			RED, // color
	//			SPRITE::SQUARE, // sprit
	//			0, // texture
	//			e.GetComponent<Sprite>().layer + 1  // layer
	//		}, // mSprite		
	//		2, // mRotation	
	//		100, // mSpeed			
	//		true // mFading			
	//	},
	//	1, // mDensity 
	//	e.GetComponent<Transform>().translation, // mCenter
	//	20, // mAreaWidth 
	//	0.5, // mDirection
	//	2, // mSpread 
	//	10, // mDuration 
	//	true, // mIsActive 
	//	1.f // mSlow
	//});
	//LOG_INFO(gob.GetComponent<General>().name + "Added component particle system============================================");
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void PlayGameButton::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void PlayGameButton::Update(Entity const& _e) {
	static bool first = true;
	if (first) {
		VI::iAudio::PlayBGM("SwampNightTime_Loop");
		first = false;
	}
	if (!VI::iAudio::IsBgmPlaying())
		VI::iAudio::PlayBGM("SwampNightTime_Loop");

	static float x = _e.GetComponent<Transform>().scale.x;
	static float y = _e.GetComponent<Transform>().scale.y;

	if (_e.GetComponent<Button>().isClick)
	{
		if (_e.GetComponent<Transform>().scale.x > x - 10.f)
			_e.GetComponent<Transform>().scale.x -= 250.f * (float)FPSManager::dt;
		if (_e.GetComponent<Transform>().scale.y > y - 10.f)
			_e.GetComponent<Transform>().scale.y -= 250.f * (float)FPSManager::dt;

	}
	else if (_e.GetComponent<Button>().isHover) {
		if (_e.GetComponent<Transform>().scale.x < x + 15.f)
			_e.GetComponent<Transform>().scale.x += 100.f * (float)FPSManager::dt;
		if (_e.GetComponent<Transform>().scale.y < y + 15.f)
			_e.GetComponent<Transform>().scale.y += 100.f * (float)FPSManager::dt;

	}
	else
	{
		if (_e.GetComponent<Transform>().scale.x < x)
			_e.GetComponent<Transform>().scale.x += 250.f * (float)FPSManager::dt;
		if (_e.GetComponent<Transform>().scale.y < y)
			_e.GetComponent<Transform>().scale.y += 250.f * (float)FPSManager::dt;
		if (_e.GetComponent<Transform>().scale.x > x)
			_e.GetComponent<Transform>().scale.x -= 250.f * (float)FPSManager::dt;
		if (_e.GetComponent<Transform>().scale.y > y)
			_e.GetComponent<Transform>().scale.y -= 250.f * (float)FPSManager::dt;
	}

	if(_e.HasComponent<Button>())
		if (_e.GetComponent<Button>().activated) {
			VI::iGameState::ChangeGameState("CutScene");
			(VI::iScene::Select("Settings")).Pause(true);
			(VI::iScene::Select("How_To_Play")).Pause(true);
			(VI::iScene::Select("Pause")).Pause(true);
			(VI::iScene::Select("Level1")).Pause(false);
			VI::iAudio::PlayBGM("Bon_Voyage_BGM");
			first = false;
		}
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void PlayGameButton::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void PlayGameButton::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void PlayGameButton::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("Play button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void PlayGameButton::Dead(Entity const& _e) {
	(void)_e;
}
