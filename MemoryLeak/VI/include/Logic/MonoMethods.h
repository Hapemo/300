/*!*****************************************************************************
\file MonoMethods.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 04-01-2023
\brief
This file contains the function declarations of the class MonoMethods.
MonoMethods are where all the mono internal calls functions that scripts need
to use from the engine is being linked.
*******************************************************************************/

#pragma once
#include "Singleton.h"
#include "InternalCalls.h"
#include <mono/jit/jit.h>

#define MONO MonoMethods::GetInstance()
#define MM MonoMethods

class MonoMethods : public Singleton<MonoMethods> {
public:
	/*!*****************************************************************************
	\brief
	Registers the internal calls for mono use.
	*******************************************************************************/
	void RegisterCalls();

	/*!*****************************************************************************
	\brief
	MonoString conversion methods.
	*******************************************************************************/
	std::string ConvertFromMonoString(MonoString* _string);
	MonoString* ConvertToMonoString(std::string _string);

	/*!*****************************************************************************
	\brief
	Logger function.
	*******************************************************************************/
	static void LogGame(MonoString* _log);
	static void LogTest(MonoString* _log);

	/*!*****************************************************************************
	\brief
	Player data.
	*******************************************************************************/
	static void LoadPlayerData(MonoString* _name);
	static MonoString* GetPlayerName();

	/*!*****************************************************************************
	\brief
	Internal Call functions that uses MonoString or needs overloading.
	*******************************************************************************/
	struct iInput {
		struct iButton {
			static bool s_Clicked(MonoString* _entityName, MonoString* _sceneName);
			static bool s_Released(MonoString* _entityName, MonoString* _sceneName);
			static bool s_Hover(MonoString* _entityName, MonoString* _sceneName);

			static bool Clicked(const int _eId);
			static bool Released(const int _eId);
			static bool Hover(const int _eId);
		};
	};
	struct iPhysics {
		static void s_ApplyImpulse(MonoString* _entityName, MonoString* _sceneName, const float _impulseX, const float _impulseY, const float _rotationX, const float _rotationY);
		static bool s_IsCollided(MonoString* _entityName1, MonoString* _entityName2, MonoString* _sceneName);
		static bool s_CheckCollision(MonoString* _entityName1, MonoString* _entityName2, MonoString* _sceneName, bool _dynamicCheck);

		static void ApplyImpulse(const int _eId, const float _impulseX, const float _impulseY, const float _rotationX, const float _rotationY);
		static bool IsCollided(const int _eId1, const int _eId2);
		static bool CheckCollision(const int _eId1, const int _eId2, bool _dynamicCheck);
	};
	struct iDialogue {
		static void LoadScript(MonoString* _filename);
		static MonoString* GetLine(int _id);
		static MonoString* GetCurrentLine();

		static bool HaveChoices();
		static bool GetCurrentChoice1();
		static bool GetCurrentChoice2();
		static bool IsPlayerCurrentSpeaker();
		
		static int GetNextId();
		static MonoString* GetNext();
		static bool NextHaveChoices();
		static bool GetNextChoice1();
		static bool GetNextChoice2();
		static bool IsPlayerNextSpeaker();
	};
	struct iGameState {
		static void Go(MonoString* _name);
		static MonoString* GetCurrentName();
	};
	struct iEntity {
		static int GetId(MonoString* _entityName, MonoString* _sceneName);
		
		static bool IsActive(const int _id);
		static void SetIsActive(const int _id, bool _active);
		static void Activate(const int _id);
		static void Deactivate(const int _id);
		static MonoString* GetParent(const int _id);
		static int GetParentId(const int _id);

		static bool s_IsActive(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetIsActive(MonoString* _entityName, MonoString* _sceneName, bool _active);
		static void s_Activate(MonoString* _entityName, MonoString* _sceneName);
		static void s_Deactivate(MonoString* _entityName, MonoString* _sceneName);
		static MonoString* s_GetParent(MonoString* _entityName, MonoString* _sceneName);
		static int s_GetParentId(MonoString* _entityName, MonoString* _sceneName);
	};
	struct iScene {
		static void Pause(MonoString* _name);
		static void Play(MonoString* _name);
		static bool IsPaused(MonoString* _name);
		static void SetForceRender(MonoString* _name, bool _render);
		static bool GetForceRender(MonoString* _name);
	};
	struct iAnimation {
		static void s_SetSpeed(MonoString* _entityName, MonoString* _sceneName, float _speed);
		static float s_GetSpeed(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetCurrentFrame(MonoString* _entityName, MonoString* _sceneName, int _index);
		static int s_GetCurrentFrame(MonoString* _entityName, MonoString* _sceneName);
		static int s_GetFrameCount(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetFrameCount(MonoString* _entityName, MonoString* _sceneName, int _count);
		static void s_SetSheetIndex(MonoString* _entityName, MonoString* _sceneName, int _index);
		static int s_GetSheetIndex(MonoString* _entityName, MonoString* _sceneName);

		static void SetSpeed(const int _eId, float _speed);
		static float GetSpeed(const int _eId);
		static void SetCurrentFrame(const int _eId, int _index);
		static int GetCurrentFrame(const int _eId);
		static int GetFrameCount(const int _eId);
		static void SetFrameCount(const int _eId, int _count);
		static void SetSheetIndex(const int _eId, int _index);
		static int GetSheetIndex(const int _eId);
	};
	struct iColorAI {
		/*!*****************************************************************************
		\brief
		Color AI
		*******************************************************************************/
		static void StartAnimation(const int _eId);
		static void AddColor(const int _eId, int _r, int _g, int _b, int _a, float _time = 1.f);
		static void AddColorAt(const int _eId, int _r, int _g, int _b, int _a, float _time = 1.f, int _index = 0);
		static void RemoveColorAt(const int _eId, int _index);
		static bool SetNextStep(const int _eId, int _i); // return ture if sucessful (withing 0 to the vector MAX)
		static void StopAfterThisAnimation(const int _eId, bool _next = true);
		static void StopAfterEndofAnimationLoop(const int _eId, bool _loop = true);
		static void ReverseOrderAfterNextAnimation(const int _eId, bool _reverse = true);
		static void SetAnimationLoopToCycle(const int _eId, bool _cycle = true);

		static void s_StartAnimation(MonoString* _entityName, MonoString* _sceneName);
		static void s_AddColor(MonoString* _entityName, MonoString* _sceneName, int _r, int _g, int _b, int _a, float _time = 1.f);
		static void s_AddColorAt(MonoString* _entityName, MonoString* _sceneName, int _r, int _g, int _b, int _a, float _time = 1.f, int _index = 0);
		static void s_RemoveColorAt(MonoString* _entityName, MonoString* _sceneName, int _index);
		static bool s_SetNextStep(MonoString* _entityName, MonoString* _sceneName, int _i); // return ture if sucessful (withing 0 to the vector MAX)
		static void s_StopAfterThisAnimation(MonoString* _entityName, MonoString* _sceneName, bool _next = true);
		static void s_StopAfterEndofAnimationLoop(MonoString* _entityName, MonoString* _sceneName, bool _loop = true);
		static void s_ReverseOrderAfterNextAnimation(MonoString* _entityName, MonoString* _sceneName, bool _reverse = true);
		static void s_SetAnimationLoopToCycle(MonoString* _entityName, MonoString* _sceneName, bool _cycle = true);
	};
	struct iParticleSystem {
		struct iParticleInfo {
			static void SetTexture(const int _eId, MonoString* _texture);
		};
	};
	struct iSprite {
		static void s_SetTexture(MonoString* _entityName, MonoString* _sceneName, MonoString* _path);
		static MonoString* s_GetTexture(MonoString* _entityName, MonoString* _sceneName);
		static void SetTexture(const int _id, MonoString* _path);
		static MonoString* GetTexture(const int _id);

		static int GetType(const int _eId);
		static void SetType(const int _eId, int _type);
		static int s_GetType(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetType(MonoString* _entityName, MonoString* _sceneName, int _type);

		static int GetLayer(const int _eId);
		static void SetLayer(const int _eId, int _layer);
		static int s_GetLayer(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetLayer(MonoString* _entityName, MonoString* _sceneName, int _layer);

		static void s_SetColor(MonoString* _entityName, MonoString* _sceneName, int _r, int _g, int _b, int _a);
		static void SetColor(const int _eId, int _r, int _g, int _b, int _a);
		static void s_SetAlpha(MonoString* _entityName, MonoString* _sceneName, int _a);
		static void SetAlpha(const int _eId, int _a);

		static int s_GetColor(MonoString* _entityName, MonoString* _sceneName, int _rgb);
		static int GetColor(const int _eId, int _rgb);
		static int s_GetAlpha(MonoString* _entityName, MonoString* _sceneName);
		static int GetAlpha(const int _eId);
	};
	struct iAudio {
		static void s_Play(MonoString* _entityName, MonoString* _sceneName);
		static void s_PlayNow(MonoString* _entityName, MonoString* _sceneName);
		static void s_PlayOnLoop(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetLoop(MonoString* _entityName, MonoString* _sceneName, bool _loop);
		static void s_Stop(MonoString* _entityName, MonoString* _sceneName);

		static void Play(const int _eId);
		static void PlayNow(const int _eId);
		static void PlayOnLoop(const int _eId);
		static void SetLoop(const int _eId, bool _loop);
		static void Stop(const int _eId);

		static void PlayBGM(MonoString* _soundName);
		static bool IsBgmPlaying();
		/*static void ForcePlay(MonoString* _soundName);*/
	};
	struct iTransform {
		static float s_GetPosX(MonoString* _entityName, MonoString* _sceneName);
		static float s_GetPosY(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetPosX(MonoString* _entityName, MonoString* _sceneName, float _posX);
		static void s_SetPosY(MonoString* _entityName, MonoString* _sceneName, float _posY);
		static float s_GetScaleX(MonoString* _entityName, MonoString* _sceneName);
		static float s_GetScaleY(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetScaleX(MonoString* _entityName, MonoString* _sceneName, float _scaleX);
		static void s_SetScaleY(MonoString* _entityName, MonoString* _sceneName, float _scaleY);
		static float s_GetRotate(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetRotate(MonoString* _entityName, MonoString* _sceneName, float _rotate);

		static float GetPosX(const int _eId);
		static float GetPosY(const int _eId);
		static void SetPosX(const int _eId, float _posX);
		static void SetPosY(const int _eId, float _posY);
		static float GetScaleX(const int _eId);
		static float GetScaleY(const int _eId);
		static void SetScaleX(const int _eId, float _scaleX);
		static void SetScaleY(const int _eId, float _scaleY);
		static float GetRotate(const int _eId);
		static void SetRotate(const int _eId, float _rotate);
	};
	struct iText {
		static void s_UpdateText(MonoString* _entityName, MonoString* _sceneName, MonoString* _text);
		static void s_SetOffset(MonoString* _entityName, MonoString* _sceneName, float _xoffset, float _yoffset);
		static float s_GetOffsetX(MonoString* _entityName, MonoString* _sceneName);
		static float s_GetOffsetY(MonoString* _entityName, MonoString* _sceneName);
		static int s_GetLineCount(MonoString* _entityName, MonoString* _sceneName);
		static float s_GetScale(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetScale(MonoString* _entityName, MonoString* _sceneName, float _scale);

		static void UpdateText(const int _eId, MonoString* _text);
		static void SetOffset(const int _eId, float _xoffset, float _yoffset);
		static float GetOffsetX(const int _eId);
		static float GetOffsetY(const int _eId);
		static int GetLineCount(const int _eId);
		static float GetScale(const int _eId);
		static void SetScale(const int _eId, float _scale);
	};
	struct iLightSource {
		static float s_GetRadius(MonoString* _entityName, MonoString* _sceneName);
		static void s_SetRadius(MonoString* _entityName, MonoString* _sceneName, float _radius);
		static float GetRadius(const int _eId);
		static void SetRadius(const int _eId, float _radius);
	};
};
