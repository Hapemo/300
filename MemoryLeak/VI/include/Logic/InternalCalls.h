/*!*****************************************************************************
\file InternalCalls.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 22-11-2022
\brief
This file contains the function declarations of the class InternalCalls.
InternalCalls are where all functions that scripts need to use from the engine
is being stored.
*******************************************************************************/

#pragma once
#include <mono/jit/jit.h>
#include "Singleton.h"
#include "Input.h"
#include "vec2.h"
#include "ECS_items.h"
//#include "Helper.h"
//#include "GameStateManager.h"
//#include "Scene.h"
//#include "DialogManager.h"

class Scene; 
class GameState;
struct Transform;

#define FUNC InternalCalls::GetInstance()
#define VI InternalCalls

class InternalCalls : public Singleton<InternalCalls> {
public:
	/*!*****************************************************************************
	\brief
	Test internal function for string.
	*******************************************************************************/
	static void TestArgString(MonoString* _thingToPrint);
	static MonoString* TestReturnString();

	/*!*****************************************************************************
	\brief
	Error logger helper function.
	*******************************************************************************/
	static void MissingComponent(std::string _entityName, std::string _componentName);

	/*!*****************************************************************************
	\brief
	Logger function.
	*******************************************************************************/
	static void LogGame(std::string _log);
	static void LogTest(std::string _log);

	/*!*****************************************************************************
	\brief
	Gets the delta time in double.
	*******************************************************************************/
	static double GetDeltaTime();

	/*!*****************************************************************************
	\brief
	Player data.
	*******************************************************************************/
	static void LoadPlayerData(std::string _name);
	static void SavePlayerData();

	static std::string GetPlayerName();
	static int GetLevelAt();
	static void SetLevelAt(int _level);

	static int GetData1(int _index);
	static void SetData1(int _index, int _value);
	static int GetData2(int _index);
	static void SetData2(int _index, int _value);

	struct iInput {
		/*!*****************************************************************************
		\brief
		Checks for the state of certain key
		\param STATE
		- State of the key pressed
		\param KEY
		- Key to check the state of
		\return bool
		- Returns true if inputted state matches current state of specified key
		*******************************************************************************/
		static bool CheckKey(E_STATE _state, E_KEY _key);
		struct iKey {
			static bool Press(int _key);
			static bool Hold(int _key);
			static bool Release(int _key);
			static bool Idle(int _key);
		};

		struct iButton {
			/*!*****************************************************************************
			\brief
			This is for checking button activity.
			*******************************************************************************/
			static bool Clicked(const Entity& _e);
			static bool Clicked(const int _eId);
			static bool Clicked(std::string const& _entityName, std::string const& _sceneName);
			
			static bool Released(const Entity& _e);
			static bool Released(const int _eId);
			static bool Released(std::string const& _entityName, std::string const& _sceneName);
			
			static bool Hover(const Entity& _e);
			static bool Hover(const int _eId);
			static bool Hover(std::string const& _entityName, std::string const& _sceneName);

			static bool AllNotHover();
			static bool AllNotClick();
		};

		struct iMouse {
			/*!*****************************************************************************
			\brief
			Get world mouse position.
			*******************************************************************************/
			static Math::Vec2 GetWorldPos();
			static float GetWorldPosX();
			static float GetWorldPosY();
		};
	};

	struct iPhysics {
		/*!*****************************************************************************
		\brief
		ApplyImpulse function that adds a velocity impulse to the entity
		\param const Entity &
		A reference to a read-only Entity to
		\param const Math::Vec2 &
		A reference to a read-only variable containing the velocity impulse
		\param const Math::Vec2 &
		A reference to a read-only variable containing the rotation impulse
		calculation
		\return void
		NULL
		*******************************************************************************/
		static void ApplyImpulse(const Entity& _e, const Math::Vec2& _impulse, const Math::Vec2& _rotation);
		static void ApplyImpulse(const int _eId, const float _impulseX, const float _impulseY, const float _rotationX, const float _rotationY);
		static void ApplyImpulse(std::string const& _entityName, std::string const& _sceneName, const float _impulseX, const float _impulseY, const float _rotationX, const float _rotationY);

		/*!*****************************************************************************
		\brief
		IsCollided function that checks if two given entities have collided by
		checking whether if a contact with the two entities exists
		\param const Entity &
		A reference to a read-only entity to compare with
		\param const Entity &
		A reference to a read-only entity to compare against
		\return bool
		Evaluated result of whether a collision happened between the two given entities
		*******************************************************************************/
		static bool IsCollided(const Entity& _e1, const Entity& _e2);
		static bool IsCollided(const int _eId1, const int _eId2);
		static bool IsCollided(std::string const& _entityName1, std::string const& _entityName2, std::string const& _sceneName);

		static bool CheckCollision(const Entity& _e1, const Entity& _e2, const bool& _dynamicCheck = false);
		static bool CheckCollision(const int _eId1, const int _eId2, const bool& _dynamicCheck = false);
		static bool CheckCollision(std::string const& _entityName1, std::string const& _entityName2, std::string const& _sceneName, bool const& _dynamicCheck = false);
	};

	struct iDialogue {
		/*!*****************************************************************************
		\brief
			Loads the dialogs from a json file to the dialog manager

		\return
		None.
		*******************************************************************************/
		static void LoadScript(std::string _filename);

		/*!*****************************************************************************
		\brief
		Retrieves the dialog text of the dialog with a specific id using function from
		DialogManager.

		\param int _id
		The id of the dialog to get.

		\return
		Returns the dialog text in string.
		*******************************************************************************/
		static std::string GetLine(int _id);

		/*!*****************************************************************************
		\brief
		Retrieves the next dialog id using function from DialogManager.

		\param int _id
		The id of the current dialog.

		\return
		Returns the next dialog id.
		*******************************************************************************/
		static int GetNextId(int _id);
		static std::string GetNext(int _id);

		/*!*****************************************************************************
		\brief
		Checks if/Get the dialog with a specific id has choices using function from
		DialogManager.

		\param int _id
		The id of the dialog to be checked.

		\return
		Returns true if the dialog has choices else returns false.
		*******************************************************************************/
		static bool HaveChoices(int _id);
		static std::pair<int, int> GetChoices(int _id);
		static int GetChoice1(int _id);
		static int GetChoice2(int _id);

		/*!*****************************************************************************
		\brief
		Set/Get the selected choice using function from DialogManager.

		\param int _id
		The id of the current dialog.

		\param int _selectedChoice
		The selected choice.
		*******************************************************************************/
		static void SetSelectedChoice(int _selectedChoice);
		static int GetSelectedChoice(int _id);
		static int GetCurrentSelectedChoice();

		/*!*****************************************************************************
		\brief
		Get/Set current dialogue id using function from DialogManager.

		\param int _id
		The id of the current dialog.
		*******************************************************************************/
		static bool SetCurrentId(int _id);
		static int GetCurrentId();

		/*!*****************************************************************************
		\brief
		Returns true if speaker is player using function from DialogManager.

		\param int _id
		The id of the current dialog.
		*******************************************************************************/
		static bool IsPlayerSpeaker(int _id);
	};

	struct iGameState {
		/*!*****************************************************************************
		\brief
		This function is called by user, to change the next game state
		*******************************************************************************/
		static void ChangeGameState(std::string const& _name);

		/*!*****************************************************************************
		\brief
		Get current game state
		*******************************************************************************/
		static GameState& CurrentGameState();
		static std::string GetCurrentGameStateName();

		/*!*****************************************************************************
		\brief
		Exits the game.
		*******************************************************************************/
		static void GameStateExit();
	};

	struct iEntity {
		/*!*****************************************************************************
		\brief
		Gets entity id.
		*******************************************************************************/
		static int GetId(const Entity& _e);
		static int GetId(std::string _entityName, std::string _sceneName);

		/*!*****************************************************************************
		\brief
		Gets an entity.
		*******************************************************************************/
		static Entity GetEntity(const int _eId);
		static Entity GetEntity(std::string const& _entityName, std::string const& _sceneName);

		/*!*****************************************************************************
		\brief
		Checks if/Set an entity is active.
		*******************************************************************************/
		static bool IsActive(const Entity& _e);
		static bool IsActive(const int _eId);
		static bool IsActive(std::string const& _entityName, std::string const& _sceneName);

		static void SetIsActive(const Entity& _e, bool _active);
		static void SetIsActive(const int _eId, bool _active);
		static void SetIsActive(std::string const& _entityName, std::string const& _sceneName, bool _active);

		/*!*****************************************************************************
		\brief
		Activate/Deactivate an entity.
		*******************************************************************************/
		static void Activate(const Entity& _e);
		static void Deactivate(const Entity& _e);
		static void Activate(const int _eId);
		static void Deactivate(const int _eId);
		static void Activate(std::string const& _entityName, std::string const& _sceneName);
		static void Deactivate(std::string const& _entityName, std::string const& _sceneName);

		/*!*****************************************************************************
		\brief
		Gets parent name/id of an entity.
		*******************************************************************************/
		static std::string GetParent(const Entity& _e);
		static std::string GetParent(const int _eId);
		static std::string GetParent(std::string const& _entityName, std::string const& _sceneName);
		
		static int GetParentId(const Entity& _e);
		static int GetParentId(const int _eId);
		static int GetParentId(std::string const& _entityName, std::string const& _sceneName);
	};

	struct iScene {
		/*!*****************************************************************************
		\brief
		Gets scene to pause or unpause the scene.
		*******************************************************************************/
		static Scene& Select(std::string const& _name);
		static void Pause(std::string const& _name);
		static void Play(std::string const& _name);
		static bool IsPaused(std::string const& _name);
		static void SetForceRender(std::string const& _name, bool _render = true);
		static bool GetForceRender(std::string const& _name);
	};

	struct iCamera {
		/*!*****************************************************************************
		\brief
		Get current game state's camera
		*******************************************************************************/
		static Transform& CurrentCamera();
		static float GetScaleX();
		static float GetScaleY();
		static float GetPosX();
		static float GetPosY();
		static void SetScaleX(float _x);
		static void SetScaleY(float _y);
		static void SetPosX(float _x);
		static void SetPosY(float _y);
	};

	struct iWindow {
		/*!*****************************************************************************
		\brief
		Get main window's screen width and height
		*******************************************************************************/
		static int GetScreenWidth();
		static int GetScreenHeight();

		/*!*****************************************************************************
		\brief
		Set the main window to fullscreen according to bool
		*******************************************************************************/
		static bool GetFullScreen();
		static void SetFullScreen(bool _fullscreen = true);
	};

	struct iViewport {
		/*!*****************************************************************************
		\brief
		Viewport stuff
		*******************************************************************************/
		static int GetWidth(const int _eId);
		static void SetWidth(const int _eId, int _width);
		static bool IsUI(const int _eId);
		static void SetIsUI(const int _eId, bool _ui = true);
	};

	struct iMath {
		/*!*****************************************************************************
		\brief
		Math vector functions
		*******************************************************************************/
		static float SqMagnitude(float _x, float _y);
		static float Magnitude(float _x, float _y);
		static float NormalizeX(float _x, float _y);
		static float NormalizeY(float _x, float _y);
		static float ArcTangent(float _x, float _y);
		static float Negate(float _value);
		static float Epsilon();
		static double Pi();
		static float ConvertRadToDeg(float _value);
		static float ConvertDegToRad(float _value);
	};

	struct iAnimation {
		struct iSpriteSheet {
			/*!*****************************************************************************
			\brief
			Set/Get current animation image speed
			*******************************************************************************/
			static void SetSpeed(const Entity& _e, float _speed);
			static float GetSpeed(const Entity& _e);
			static void SetSpeed(const int _eId, float _speed);
			static float GetSpeed(const int _eId);
			static void SetSpeed(std::string const& _entityName, std::string const& _sceneName, float _speed);
			static float GetSpeed(std::string const& _entityName, std::string const& _sceneName);

			/*!*****************************************************************************
			\brief
			Set/Get current sprite sheet image index
			*******************************************************************************/
			static void SetSheetIndex(const Entity& _e, int _index);
			static int GetSheetIndex(const Entity& _e);
			static void SetSheetIndex(const int _eId, int _index);
			static int GetSheetIndex(const int _eId);
			static void SetSheetIndex(std::string const& _entityName, std::string const& _sceneName, int _index);
			static int GetSheetIndex(std::string const& _entityName, std::string const& _sceneName);

			/*!*****************************************************************************
			\brief
			Set/Get current animation image frame index
			*******************************************************************************/
			static void SetCurrentFrame(const Entity& _e, int _index);
			static int GetCurrentFrame(const Entity& _e);
			static void SetCurrentFrame(const int _eId, int _index);
			static int GetCurrentFrame(const int _eId);
			static void SetCurrentFrame(std::string const& _entityName, std::string const& _sceneName, int _index);
			static int GetCurrentFrame(std::string const& _entityName, std::string const& _sceneName);

			/*!*****************************************************************************
			\brief
			Get current animation total frame count
			*******************************************************************************/
			static void SetFrameCount(const Entity& _e, int _count);
			static int GetFrameCount(const Entity& _e);
			static void SetFrameCount(const int _eId, int _count);
			static int GetFrameCount(const int _eId);
			static void SetFrameCount(std::string const& _entityName, std::string const& _sceneName, int _count);
			static int GetFrameCount(std::string const& _entityName, std::string const& _sceneName);
		};
	};

	struct iMovementAI {
		/*!*****************************************************************************
		\brief
		Movement AI
		*******************************************************************************/
		static void Start(const int _eId);
		static void ForceStop(const int _eId);
		static bool SetNext(const int _eId, int _i);
		static void GoToNext(const int _eId);
		static void Stop(const int _eId, bool _next = true);
		static void StopAfterEndLoop(const int _eId, bool _loop = true);
		static void ReverseOrder(const int _eId, bool _reverse = true);
		static void SetLoopCycle(const int _eId, bool _cycle = true);
		static int GetCurrentIndex(const int _eId);
		static void Remove(const int _eId, int _index);
		static void EditTiming(const int _eId, float _time);
		static void EditCurrentTiming(const int _eId, float _time);
		static void AddTransform(const int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1);
		static void EditCurrent(const int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1);
		static void CurrentScaleX(const int _eId, float _scaleX);
		static void CurrentScaleY(const int _eId, float _scaleY);
		static void CurrentRotate(const int _eId, float _rot);
		static void CurrentPosX(const int _eId, float _posX);
		static void CurrentPosY(const int _eId, float _posY);
		static float GetTiming(const int _eId);
		static float GetCurrentTiming(const int _eId);
		static float GetCurrentScaleX(const int _eId);
		static float GetCurrentScaleY(const int _eId);
		static float GetCurrentRotate(const int _eId);
		static float GetCurrentPosX(const int _eId);
		static float GetCurrentPosY(const int _eId);
		static void AddTransformAt(const int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1.f, int _index = 0);
		static void TransformScaleAt(const int _eId, float _scaleX, float _scaleY, float _time = 1.f, int _index = 0);
		static void TransformRotateAt(const int _eId, float _rot, float _time = 1.f, int _index = 0);
		static void TransformPosAt(const int _eId, float _posX, float _posY, float _time = 1.f, int _index = 0);
		static void AddTransformDifference(const int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1);
		static void SetCalculatedTimeFromPosition(const int _eId, float _posX, float _posY, int _step = -1);
		static void SetCalculatedTimeFromRotation(const int _eId, float _rot, int _step = -1);
		static void SetCalculatedTimeFromScale(const int _eId, float _scaleX, float _scaleY, int _step = -1);
		static void AddTransformAtCurrent(const int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1.f);
		static void TransformScaleAtCurrent(const int _eId, float _scaleX, float _scaleY, float _time = 1.f);
		static void TransformRotateAtCurrent(const int _eId, float _rot, float _time = 1.f);
		static void TransformPosAtCurrent(const int _eId, float _posX, float _posY, float _time = 1.f);
	};

	struct iColorAI {
		/*!*****************************************************************************
		\brief
		Color AI
		*******************************************************************************/
		static void StartAnimation(const Entity& _e);
		static void AddColor(const Entity& _e, int _r, int _g, int _b, int _a, float _time = 1.f);
		static void AddColorAt(const Entity& _e, int _r, int _g, int _b, int _a, float _time = 1.f, int _index = 0);
		static void RemoveColorAt(const Entity& _e, int _index);
		static bool SetNextStep(const Entity& _e, int _i); // return ture if sucessful (withing 0 to the vector MAX)
		static void StopAfterThisAnimation(const Entity& _e, bool _next = true);
		static void StopAfterEndofAnimationLoop(const Entity& _e, bool _loop = true);
		static void ReverseOrderAfterNextAnimation(const Entity& _e, bool _reverse = true);
		static void SetAnimationLoopToCycle(const Entity& _e, bool _cycle = true);

		static void StartAnimation(const int _eId);
		static void AddColor(const int _eId, int _r, int _g, int _b, int _a, float _time = 1.f);
		static void AddColorAt(const int _eId, int _r, int _g, int _b, int _a, float _time = 1.f, int _index = 0);
		static void RemoveColorAt(const int _eId, int _index);
		static bool SetNextStep(const int _eId, int _i); // return ture if sucessful (withing 0 to the vector MAX)
		static void StopAfterThisAnimation(const int _eId, bool _next = true);
		static void StopAfterEndofAnimationLoop(const int _eId, bool _loop = true);
		static void ReverseOrderAfterNextAnimation(const int _eId, bool _reverse = true);
		static void SetAnimationLoopToCycle(const int _eId, bool _cycle = true);

		static void StartAnimation(std::string const& _entityName, std::string const& _sceneName);
		static void AddColor(std::string const& _entityName, std::string const& _sceneName, int _r, int _g, int _b, int _a, float _time = 1.f);
		static void AddColorAt(std::string const& _entityName, std::string const& _sceneName, int _r, int _g, int _b, int _a, float _time = 1.f, int _index = 0);
		static void RemoveColorAt(std::string const& _entityName, std::string const& _sceneName, int _index);
		static bool SetNextStep(std::string const& _entityName, std::string const& _sceneName, int _i); // return ture if sucessful (withing 0 to the vector MAX)
		static void StopAfterThisAnimation(std::string const& _entityName, std::string const& _sceneName, bool _next = true);
		static void StopAfterEndofAnimationLoop(std::string const& _entityName, std::string const& _sceneName, bool _loop = true);
		static void ReverseOrderAfterNextAnimation(std::string const& _entityName, std::string const& _sceneName, bool _reverse = true);
		static void SetAnimationLoopToCycle(std::string const& _entityName, std::string const& _sceneName, bool _cycle = true);
	};

	struct iParticleSystem {
		/*!*****************************************************************************
		\brief
		Particle system
		*******************************************************************************/
		static int GetDensity(const int _eId);
		static void SetDensity(const int _eId, int _density);

		static float GetPosX(const int _eId);
		static void SetPosX(const int _eId, float _posX);
		static float GetPosY(const int _eId);
		static void SetPosY(const int _eId, float _posY);

		static float GetWidth(const int _eId);
		static void SetWidth(const int _eId, float _width);

		static float GetDir(const int _eId);
		static void SetDir(const int _eId, float _dir);

		static float GetSpread(const int _eId);
		static void SetSpread(const int _eId, float _spread);

		static float GetDuration(const int _eId);
		static void SetDuration(const int _eId, float _dur);

		static bool IsActive(const int _eId);
		static void SetIsActive(const int _eId, bool _active = true);

		static float GetSlow(const int _eId);
		static void SetSlow(const int _eId, float _slow);

		// Generate one frame of particles
		static void GenerateOnce(const int _eId);

		// Generate loop of time frame
		static void GenerateLoop(const int _eId, float _duration);


		/*!*****************************************************************************
		\brief
		Individual particle
		*******************************************************************************/
		struct iParticleInfo {
			static float GetScale(const int _eId);
			static float GetDirection(const int _eId);
			static float GetLifeSpan(const int _eId);
			static float GetRotation(const int _eId);
			static float GetSpeed(const int _eId);
			static bool GetFading(const int _eId);

			static int GetSpriteType(const int _eId);
			static int GetLayer(const int _eId);

			static void SetScale(const int _eId, float _scale);
			static void SetDirection(const int _eId, float _dir);
			static void SetLifeSpan(const int _eId, float _lifespan);
			static void SetRotation(const int _eId, float _rot);
			static void SetSpeed(const int _eId, float _speed);
			static void SetFading(const int _eId, bool _fade = true);

			static void SetColour(const int _eId, int _r, int _g, int _b, int _a);
			static void SetSpriteType(const int _eId, int _type);
			static void SetTexture(const int _eId, std::string _texture);
			static void SetLayer(const int _eId, int _layer);

			/*
			Color color = Color{ 0,255,0,255 };	//save, edit, see
			SPRITE sprite;						//save, edit, see
			GLuint texture = 0;					//save, edit, see
			int layer = 0;						//save, edit, see
			*/
		};
	};

	struct iSprite {
		/*!*****************************************************************************
		\brief
		Set/Get the texture of an entity.
		*******************************************************************************/
		static void SetTexture(const Entity& _e, const std::string& _path);
		static std::string GetTexture(const Entity& _e);
		static void SetTexture(const int _eId, const std::string& _path);
		static std::string GetTexture(const int _eId);
		static void SetTexture(std::string const& _entityName, std::string const& _sceneName, const std::string& _path);
		static std::string GetTexture(std::string const& _entityName, std::string const& _sceneName);

		/*!*****************************************************************************
		\brief
		Get/Set sprite type
		*******************************************************************************/
		static int GetType(const Entity& _e);
		static int GetType(const int _eId);
		static int GetType(std::string const& _entityName, std::string const& _sceneName);
		static void SetType(const Entity& _e, int _type);
		static void SetType(const int _eId, int _type);
		static void SetType(std::string const& _entityName, std::string const& _sceneName, int _type);

		/*!*****************************************************************************
		\brief
		Set/Get layer of an entity.
		*******************************************************************************/
		static int GetLayer(const Entity& _e);
		static void SetLayer(const Entity& _e, int layer);
		static int GetLayer(const int _eId);
		static void SetLayer(const int _eId, int layer);
		static int GetLayer(std::string const& _entityName, std::string const& _sceneName);
		static void SetLayer(std::string const& _entityName, std::string const& _sceneName, int layer);

		/*!*****************************************************************************
		\brief
		Get/Set sprite colour of an entity.
		*******************************************************************************/
		static void SetAlpha(const Entity& _e, int _a);
		static void SetAlpha(const int _eId, int _a);
		static void SetAlpha(std::string const& _entityName, std::string const& _sceneName, int _a);
		static void SetColor(const Entity& _e, int _r, int _g, int _b, int _a);
		static void SetColor(const int _eId, int _r, int _g, int _b, int _a);
		static void SetColor(std::string const& _entityName, std::string const& _sceneName, int _r, int _g, int _b, int _a);
		static int GetColor(const Entity& _e, int _rgba); // _rgba: 0 = red, 1 = green, 2 = blue, 3 = alpha
		static int GetColor(const int _eId, int _rgba);
		static int GetColor(std::string const& _entityName, std::string const& _sceneName, int _rgba);
	};

	struct iAudio {
		/*!*****************************************************************************
		\brief
			Plays sound
		*******************************************************************************/
		static void Play(const Entity& _e);
		static void Play(const int _eId);
		static void Play(std::string const& _entityName, std::string const& _sceneName);

		static void PlayNow(const Entity& _e);
		static void PlayNow(const int _eId);
		static void PlayNow(std::string const& _entityName, std::string const& _sceneName);

		/*!*****************************************************************************
		\brief
			Plays a sound on loop
		*******************************************************************************/
		static void PlayOnLoop(const Entity& _e);
		static void PlayOnLoop(const int _eId);
		static void PlayOnLoop(std::string const& _entityName, std::string const& _sceneName);

		/*!*****************************************************************************
		\brief
			Loop functions
		*******************************************************************************/
		static void SetLoop(const Entity& _e, bool _loop);
		static void SetLoop(const int _eId, bool _loop);
		static void SetLoop(std::string const& _entityName, std::string const& _sceneName, bool _loop);

		/*!*****************************************************************************
		\brief
			Stop a sound
		*******************************************************************************/
		static void Stop(const Entity& _e);
		static void Stop(const int _eId);
		static void Stop(std::string const& _entityName, std::string const& _sceneName);

		static void StopAll();
		static void StopBGM();
		static void StopSFX();

		/*!*****************************************************************************
		\brief
			BGM & force play
		*******************************************************************************/
		static void PlayBGM(std::string const& _soundName);
		static bool IsBgmPlaying();
		/*static void ForcePlay(std::string const& _soundName);*/

		/*!*****************************************************************************
		\brief
			Set and get volume
		*******************************************************************************/
		static void SetAllVolume(float _vol);
		static void SetBGMVolume(float _vol);
		static void SetSFXVolume(float _vol);
		static float GetBgmVol();
		static float GetSfxVol();
	};

	struct iTransform {
		static Transform& GetTransform(const Entity& _e);
		static float GetRotation(const Entity& _e);
		static Math::Vec2 GetScale(const Entity& _e);
		static Math::Vec2 GetTranslate(const Entity& _e);

		/*!*****************************************************************************
		\brief
		Get/Set pos of an entity.
		*******************************************************************************/
		static float GetPosX(const Entity& _e);
		static float GetPosY(const Entity& _e);
		static void SetPosX(const Entity& _e, float _posX);
		static void SetPosY(const Entity& _e, float _posY);

		static float GetPosX(const int _eId);
		static float GetPosY(const int _eId);
		static void SetPosX(const int _eId, float _posX);
		static void SetPosY(const int _eId, float _posY);

		static float GetPosX(std::string const& _entityName, std::string const& _sceneName);
		static float GetPosY(std::string const& _entityName, std::string const& _sceneName);
		static void SetPosX(std::string const& _entityName, std::string const& _sceneName, float _posX);
		static void SetPosY(std::string const& _entityName, std::string const& _sceneName, float _posY);

		/*!*****************************************************************************
		\brief
		Get/Set scale of an entity.
		*******************************************************************************/
		static float GetScaleX(const Entity& _e);
		static float GetScaleY(const Entity& _e);
		static void SetScaleX(const Entity& _e, float _scaleX);
		static void SetScaleY(const Entity& _e, float _scaleY);

		static float GetScaleX(const int _eId);
		static float GetScaleY(const int _eId);
		static void SetScaleX(const int _eId, float _scaleX);
		static void SetScaleY(const int _eId, float _scaleY);

		static float GetScaleX(std::string const& _entityName, std::string const& _sceneName);
		static float GetScaleY(std::string const& _entityName, std::string const& _sceneName);
		static void SetScaleX(std::string const& _entityName, std::string const& _sceneName, float _scaleX);
		static void SetScaleY(std::string const& _entityName, std::string const& _sceneName, float _scaleY);

		/*!*****************************************************************************
		\brief
		Get/Set rotation of an entity.
		*******************************************************************************/
		static float GetRotate(const Entity& _e);
		static void SetRotate(const Entity& _e, float _rotate);

		static float GetRotate(const int _eId);
		static void SetRotate(const int _eId, float _rotate);

		static float GetRotate(std::string const& _entityName, std::string const& _sceneName);
		static void SetRotate(std::string const& _entityName, std::string const& _sceneName, float _rotate);
	};

	struct iText {
		/*!*****************************************************************************
		\brief
		Changing text component text.
		*******************************************************************************/
		static void UpdateText(const Entity& _e, std::string const& _text);
		static void UpdateText(const int _eId, std::string const& _text);
		static void UpdateText(std::string const& _entityName, std::string const& _sceneName, std::string const& _text);
		
		static void SetOffset(const Entity& _e, float _xoffset, float _yoffset);
		static void SetOffset(const int _eId, float _xoffset, float _yoffset);
		static void SetOffset(std::string const& _entityName, std::string const& _sceneName, float _xoffset, float _yoffset);

		static float GetOffsetX(const Entity& _e);
		static float GetOffsetX(const int _eId);
		static float GetOffsetX(std::string const& _entityName, std::string const& _sceneName);

		static float GetOffsetY(const Entity& _e);
		static float GetOffsetY(const int _eId);
		static float GetOffsetY(std::string const& _entityName, std::string const& _sceneName);

		static int GetLineCount(const Entity& _e);
		static int GetLineCount(const int _eId);
		static int GetLineCount(std::string const& _entityName, std::string const& _sceneName);

		static float GetScale(const Entity& _e);
		static float GetScale(const int _eId);
		static float GetScale(std::string const& _entityName, std::string const& _sceneName);

		static void SetScale(const Entity& _e, float _scale);
		static void SetScale(const int _eId, float _scale);
		static void SetScale(std::string const& _entityName, std::string const& _sceneName, float _scale);

		static void SetColour(const int _eId, int _r, int _g, int _b, int _a);
	};

	struct iWeather {
		/*!*****************************************************************************
		\brief
		Weather functions.
		*******************************************************************************/
		static void WeatherInit(float width = 1600, float height = 900);
		static int GetCurrentWeather(int index, float posX, float posY);
	};

	struct iLightSource {
		/*!*****************************************************************************
		\brief
		Changing lightsource component.
		*******************************************************************************/
		static float GetRadius(const Entity& _e);
		static float GetRadius(const int _eId);
		static float GetRadius(std::string const& _entityName, std::string const& _sceneName);

		static void SetRadius(const Entity& _e, float _radius);
		static void SetRadius(const int _eId, float _radius);
		static void SetRadius(std::string const& _entityName, std::string const& _sceneName, float _radius);
	};
};
