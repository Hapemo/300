using System;
using System.Runtime.CompilerServices;

namespace BonVoyage
{
    public class InternalCalls
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool TestArgString(string _thingToPrint);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string TestReturnString();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool CheckKeyPress(int _key);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool CheckKeyHold(int _key);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool CheckKeyRelease(int _key);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool CheckKeyIdle(int _key);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetWorldMousePosX();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetWorldMousePosY();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void ApplyImpulse(string _entityName, string _sceneName, float _impulseX, float _impulseY, float _rotationX, float _rotationY);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void LoadDialogs(string _filename);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string GetDialogue(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetNextDialogueID(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool HasChoice(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetChoice1(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetChoice2(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetNext(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetSelectedChoice(int _id, int _selectedChoice);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool SetCurrentDialogueID(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetCurrentDialogueID();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void ChangeGameState(string _name);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool EntityIsActive(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void EntityActivate(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void EntityDeactivate(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string EntityGetParent(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int EntityGetParentId(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string GetCurrentGameStateName();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void PauseScene(string _name);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void PlayScene(string _name);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void GetCurrentCameraScaleX();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void GetCurrentCameraScaleY();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void GetCurrentCameraPosX();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void GetCurrentCameraPosY();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetCurrentCameraScaleX(float _x);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetCurrentCameraScaleY(float _y);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetCurrentCameraPosX(float _x);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetCurrentCameraPosY(float _y);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetCurrentImageIndex(string _entityName, string _sceneName, int _index);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetCurrentImageIndex(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetTexture(string _entityName, string _sceneName, string _path);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string GetTexture(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool EntitiesCollided(string _entityName1, string _entityName2, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool CheckCollision(string _entityName1, string _entityName2, string _sceneName, bool _dynamicCheck);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void GameStateExit();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static double GetDeltaTime();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void PlayAnySound(string _name, int _channel);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool IsPlaying(int _channel);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void PlayBGSound(string _name, int _channel);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetPosX(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetPosY(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetPosX(string _entityName, string _sceneName, float _posX);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetPosY(string _entityName, string _sceneName, float _posY);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetScaleX(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetScaleY(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetScaleX(string _entityName, string _sceneName, float _scaleX);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetScaleY(string _entityName, string _sceneName, float _scaleY);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetRotate(string _entityName, string _sceneName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetRotate(string _entityName, string _sceneName, float _rotate);
	}
}
