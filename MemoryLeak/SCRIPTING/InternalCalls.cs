/*!*****************************************************************************
\file InternalCalls.cs
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 02-02-2023
\brief
This file contains the function declarations of the class InternalCalls in C#.
InternalCalls are where all functions that scripts need to use from the engine
is being stored.
*******************************************************************************/

using System;
using System.Runtime.CompilerServices;

public class LOG {
	[MethodImpl(MethodImplOptions.InternalCall)]
	internal extern static void WRITE(string _log);

	[MethodImpl(MethodImplOptions.InternalCall)]
	internal extern static void TEST(string _log);
}

namespace BonVoyage {
	public class THIS 	{
		static int ENTITY = 0;
		internal static bool StoreId(int _ENTITY) {
			ENTITY = _ENTITY;
			return true;
		}

		public class Parent {
			internal static string Name() { return VI.Entity.Parent.Name(ENTITY); }
			internal static int Id() { return VI.Entity.Parent.Id(ENTITY); }
		}
		internal static int GetId() { return ENTITY; }
		internal static bool IsActive() { return VI.Entity.IsActive(ENTITY); }
		internal static void SetActive(bool _active = true) { VI.Entity.SetActive(ENTITY, _active); }
		internal static void Activate() { VI.Entity.Activate(ENTITY); }
		internal static void Deactivate() { VI.Entity.Deactivate(ENTITY); }
		public class Input {
			public class Button {
				internal static bool Clicked() { return VI.Input.Button.Clicked(ENTITY); }
				internal static bool Released() { return VI.Input.Button.Released(ENTITY); }
				internal static bool Hover() { return VI.Input.Button.Hover(ENTITY); }
			}
		}

		public class Animation {
			public class SpriteSheet {
				public class SheetIndex {
					internal static void Set(int _index) { VI.Animation.SpriteSheet.SheetIndex.Set(ENTITY, _index); }
					internal static int Get() { return VI.Animation.SpriteSheet.SheetIndex.Get(ENTITY); }
				}
				public class Speed {
					internal static void Set(float _speed) { VI.Animation.SpriteSheet.Speed.Set(ENTITY, _speed); }
					internal static float Get() { return VI.Animation.SpriteSheet.Speed.Get(ENTITY); }
				}
				public class CurrentFrame {
					internal static void Set(int _index) { VI.Animation.SpriteSheet.CurrentFrame.Set(ENTITY, _index); }
					internal static int Get() { return VI.Animation.SpriteSheet.CurrentFrame.Get(ENTITY); }
				}
				public class FrameCount {
					internal static void Set(int _count) { VI.Animation.SpriteSheet.FrameCount.Set(ENTITY, _count); }
					internal static int Get() { return VI.Animation.SpriteSheet.FrameCount.Get(ENTITY); }
				}
			}
		}
		public class MovementAI {
			internal static void Run() { VI.MovementAI.Run(ENTITY); }
            internal static void ForceStop() { VI.MovementAI.ForceStop(ENTITY); }
            internal static bool SetNext(int _i) { return VI.MovementAI.SetNext(ENTITY, _i); }
			internal static void GoToNext() { VI.MovementAI.GoToNext(ENTITY); }
			internal static void Stop(bool _next = true) { VI.MovementAI.Stop(ENTITY, _next); }
			internal static void StopAfterEndLoop(bool _loop = true) { VI.MovementAI.StopAfterEndLoop(ENTITY, _loop); }
			internal static void ReverseOrder(bool _reverse = true) { VI.MovementAI.ReverseOrder(ENTITY, _reverse); }
			internal static void SetLoopCycle(bool _cycle = true) { VI.MovementAI.SetLoopCycle(ENTITY, _cycle); }
			internal static int GetCurrentIndex() { return VI.MovementAI.GetCurrentIndex(ENTITY); }
			internal static void Remove(int _i) { VI.MovementAI.Remove(ENTITY, _i); }

			public struct Edit {
				internal static void Timing(float _time = 1) { VI.MovementAI.Edit.Timing(ENTITY, _time); }
				internal static void CurrentTiming(float _time = 0) { VI.MovementAI.Edit.CurrentTiming(ENTITY, _time); }
				internal static void Current(float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1) { VI.MovementAI.Edit.Current(ENTITY, _scaleX, _scaleY, _rot, _posX, _posY, _time); }
				internal static void CurrentScaleX(float _scaleX) { VI.MovementAI.Edit.CurrentScaleX(ENTITY, _scaleX); }
				internal static void CurrentScaleY(float _scaleY) { VI.MovementAI.Edit.CurrentScaleY(ENTITY, _scaleY); }
				internal static void CurrentRotate(float _rot) { VI.MovementAI.Edit.CurrentRotate(ENTITY, _rot); }
				internal static void CurrentPosX(float _posX) { VI.MovementAI.Edit.CurrentPosX(ENTITY, _posX); }
				internal static void CurrentPosY(float _posY) { VI.MovementAI.Edit.CurrentPosY(ENTITY, _posY); }
			}
			public struct Get {
				internal static float Timing() { return VI.MovementAI.Get.Timing(ENTITY); }
				internal static float CurrentTiming() { return VI.MovementAI.Get.CurrentTiming(ENTITY); }
				internal static float CurrentScaleX() { return VI.MovementAI.Get.CurrentScaleX(ENTITY); }
				internal static float CurrentScaleY() { return VI.MovementAI.Get.CurrentScaleY(ENTITY); }
				internal static float CurrentRotate() { return VI.MovementAI.Get.CurrentRotate(ENTITY); }
				internal static float CurrentPosX() { return VI.MovementAI.Get.CurrentPosX(ENTITY); }
				internal static float CurrentPosY() { return VI.MovementAI.Get.CurrentPosY(ENTITY); }
			}
			public struct Add {
				internal static void Transform(float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1) { VI.MovementAI.Add.Transform(ENTITY, _scaleX, _scaleY, _rot, _posX, _posY, _time); }
				internal static void TransformAt(float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1, int _index = 0) { VI.MovementAI.Add.TransformAt(ENTITY, _scaleX, _scaleY, _rot, _posX, _posY, _time, _index); }
				internal static void TransformScaleAt(float _scaleX, float _scaleY, float _time = 1, int _index = 0) { VI.MovementAI.Add.TransformScaleAt(ENTITY, _scaleX, _scaleY, _time, _index); }
				internal static void TransformRotateAt(float _rot, float _time = 1, int _index = 0) { VI.MovementAI.Add.TransformRotateAt(ENTITY, _rot, _time, _index); }
				internal static void TransformPosAt(float _posX, float _posY, float _time = 1, int _index = 0) { VI.MovementAI.Add.TransformPosAt(ENTITY, _posX, _posY, _time, _index); }
				internal static void TransformDifference(float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1) { VI.MovementAI.Add.TransformDifference(ENTITY, _scaleX, _scaleY, _rot, _posX, _posY, _time); }
			}
			public struct AddAtCurrent {
				internal static void Transform(float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1) { VI.MovementAI.AddAtCurrent.Transform(ENTITY, _scaleX, _scaleY, _rot, _posX, _posY, _time); }
				internal static void TransformScale(float _scaleX, float _scaleY, float _time = 1) { VI.MovementAI.AddAtCurrent.TransformScale(ENTITY, _scaleX, _scaleY, _time); }
				internal static void TransformRotate(float _rot, float _time = 1) { VI.MovementAI.AddAtCurrent.TransformRotate(ENTITY, _rot, _time); }
				internal static void TransformPos(float _posX, float _posY, float _time = 1) { VI.MovementAI.AddAtCurrent.TransformPos(ENTITY, _posX, _posY, _time); }
			}
			public struct SetCalculatedTime {
				internal static void FromPosition(float _posX, float _posY, int _step = -1) { VI.MovementAI.SetCalculatedTime.FromPosition(ENTITY, _posX, _posY, _step); }
				internal static void FromRotation(float _rot, int _step = -1) { VI.MovementAI.SetCalculatedTime.FromRotation(ENTITY, _rot, _step); }
				internal static void FromScale(float _scaleX, float _scaleY, int _step = -1) { VI.MovementAI.SetCalculatedTime.FromScale(ENTITY, _scaleX, _scaleY, _step); }
			}
		}
		public class Sprite {
			internal static int GetType(int _eId) { return VI.Sprite.GetType(ENTITY); }
			internal static void SetType(int _eId, int _type) { VI.Sprite.SetType(ENTITY, _type); }
			internal static void SetTexture(string _path) { VI.Sprite.SetTexture(ENTITY, _path); }
			internal static string GetTexture() { return VI.Sprite.GetTexture(ENTITY); }
			internal static void SetColor(int _r, int _g, int _b, int _a) { VI.Sprite.SetColor(ENTITY, _r, _g, _b, _a); }
			internal static int GetColor(int _rgba) { return VI.Sprite.GetColor(ENTITY, _rgba); } // _rgba: 0 = red, 1 = green, 2 = blue, 3 = alpha
			internal static void SetAlpha(int _a) { VI.Sprite.SetAlpha(ENTITY, _a); }
			internal static int GetAlpha() { return VI.Sprite.GetAlpha(ENTITY); }
		}
		public class Audio {
			internal static void Play() { VI.Audio.Play(ENTITY); }
            internal static void PlayNow() { VI.Audio.PlayNow(ENTITY); }
            internal static void PlayOnLoop() { VI.Audio.PlayOnLoop(ENTITY); }
			internal static void Stop() { VI.Audio.Stop(ENTITY); }
			internal static void SetLoop(bool _loop = true) { VI.Audio.SetLoop(ENTITY, _loop); }
		}
		public class Transform {
			public class Position {
				internal static float GetX() { return VI.Transform.Position.GetX(ENTITY); }
				internal static float GetY() { return VI.Transform.Position.GetY(ENTITY); }
				internal static void SetX(float _posX) { VI.Transform.Position.SetX(ENTITY, _posX); }
				internal static void SetY(float _posY) { VI.Transform.Position.SetY(ENTITY, _posY); }
			}
			public class Scale {
				internal static float GetX() { return VI.Transform.Scale.GetX(ENTITY); }
				internal static float GetY() { return VI.Transform.Scale.GetY(ENTITY); }
				internal static void SetX(float _scaleX) { VI.Transform.Scale.SetX(ENTITY, _scaleX); }
				internal static void SetY(float _scaleY) { VI.Transform.Scale.SetY(ENTITY, _scaleY); }
			}
			public class Rotate {
				internal static float Get() { return VI.Transform.Rotate.Get(ENTITY); }
				internal static void Set(float _rotate) { VI.Transform.Rotate.Set(ENTITY, _rotate); }
			}
		}
		public class Text {
			internal static void Update(string _text) { VI.Text.Update(ENTITY, _text); }
			internal static int GetLineCount() { return VI.Text.GetLineCount(ENTITY); }
			public class Offset {
				internal static void Set(float _xoffset, float _yoffset) { VI.Text.Offset.Set(ENTITY, _xoffset, _yoffset); }
				internal static float GetX() { return VI.Text.Offset.GetX(ENTITY); }
				internal static float GetY() { return VI.Text.Offset.GetY(ENTITY); }
			}
			public class Scale {
				internal static float Get() { return VI.Text.Scale.Get(ENTITY); }
				internal static void Set(float _scale) { VI.Text.Scale.Set(ENTITY, _scale); }
			}
		}
		public class LightSource {
			public class Radius {
				internal static float Get() { return VI.LightSource.Radius.Get(ENTITY); }
				internal static void Set(float _radius) { VI.LightSource.Radius.Set(ENTITY, _radius); }
			}
		}
	}
}

namespace VI {
	public class Test
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool ArgString(string _thingToPrint);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string ReturnString();
	}

	public class Data
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Load(string _name);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Save();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string GetPlayerName();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetLevelAt();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetLevelAt(int _level);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetData1(int _index);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetData1(int _index, int _value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetData2(int _index);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetData2(int _index, int _value);
	}

	public class General
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static double DeltaTime();
	}
	public class Input
	{
		public class Key
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Press(int _key);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Hold(int _key);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Release(int _key);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Idle(int _key);
		}
		public class Button
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool s_Clicked(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool s_Released(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool s_Hover(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Clicked(int _id);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Released(int _id);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Hover(int _id);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool AllNotHover();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool AllNotClick();
		}
		public class Mouse
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float WorldPosX();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float WorldPosY();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Press(int _key = 349); // right mouse click

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Hold(int _key = 349); // right mouse click

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Release(int _key = 349); // right mouse click

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool Idle(int _key = 349); // right mouse click
		}
	}
	public class Physics
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_ApplyImpulse(string _entityName, string _sceneName = "", float _impulseX = 0, float _impulseY = 0, float _rotationX = 0, float _rotationY = 0);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool s_IsCollided(string _entityName1, string _entityName2, string _sceneName = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool s_CheckCollision(string _entityName1, string _entityName2, string _sceneName = "", bool _dynamicCheck = true);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void ApplyImpulse(int _eId, float _impulseX, float _impulseY, float _rotationX, float _rotationY);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool IsCollided(int _eId1, int _eId2);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool CheckCollision(int _eId1, int _eId2, bool _dynamicCheck);
	}
	public class Dialogue
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void LoadScript(string _filename);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string GetLine(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool HaveChoices(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void GetSelectedChoice(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string GetNext(int _id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetNextId(int _id);
		public class Current
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static string GetLine();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int GetId();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool HaveChoices();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool SetTo(int _id);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int GetFirstChoice();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int GetSecondChoice();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetSelectedChoice(int _selectedChoice);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void GetSelectedChoice();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool PlayerSpeaking();
		}
		public class Next
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static string GetLine();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int GetId();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool HaveChoices();
			
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int GetFirstChoice();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int GetSecondChoice();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool PlayerSpeaking();
		}
		public class Choice
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int First(int _id);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int Second(int _id);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Selected(int _id, int _selectedChoice);
		}
		public class Speaker
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool IsPlayer(int _id);
		}
	}
	public class GameState
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Go(string _name);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string GetName();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Exit();
	}
	public class Entity
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetId(string _entityName, string _sceneName = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool s_IsActive(string _entityName, string _sceneName = "");

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void s_SetActive(string _entityName, string _sceneName = "", bool _active = true);

        [MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_Activate(string _entityName, string _sceneName = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_Deactivate(string _entityName, string _sceneName = "");
		public class Parent
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static string s_Name(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static string Name(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int s_Id(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int Id(int _eId);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool IsActive(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetActive(int _eId, bool _active = true);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Activate(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Deactivate(int _eId);
	}
	public class Scene
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Pause(string _name);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Play(string _name);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool IsPaused(string _name);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetForceRender(string _name, bool _render = true);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool GetForceRender(string _name);
	}
	public class Camera
	{
		public class GetScale
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float X();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float Y();
		}

		public class SetScale
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void X(float _x);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Y(float _y);
		}
		public class GetPos
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float X();

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float Y();
		}

		public class SetPos
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void X(float _x);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Y(float _y);
		}
	}

	public class Window
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetScreenWidth();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetScreenHeight();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool GetFullScreen();

        [MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetFullScreen(bool _fullscreen = true);
	}

	public class Viewport
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetWidth(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetWidth(int _eId, int _width);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool IsUI(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetIsUI(int _eId, bool _ui = true);
	}
	public class Math
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float SqMagnitude(float _x, float _y);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float Magnitude(float _x, float _y);

		public class Normalize
        {
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float X(float _x, float _y);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float Y(float _x, float _y);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float ArcTangent(float _x, float _y);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float Negate(float _value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float Epsilon();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static double Pi();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float ConvertRadToDeg(float _value);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float ConvertDegToRad(float _value);
	}
	public class Animation {
		public class SpriteSheet {
			public struct SheetIndex {
				internal static void s_Set(string _entityName, string _sceneName = "", int _index = 0) { VI.Animation.SheetIndex.s_Set(_entityName, _sceneName, _index); }
				internal static int s_Get(string _entityName, string _sceneName = "") { return VI.Animation.SheetIndex.s_Get(_entityName, _sceneName); }
				internal static void Set(int _eId, int _index) { VI.Animation.SheetIndex.Set(_eId, _index); }
				internal static int Get(int _eId) { return VI.Animation.SheetIndex.Get(_eId); }
			}
			public struct Speed {
				internal static void s_Set(string _entityName, string _sceneName = "", float _speed = 1) { VI.Animation.Speed.s_Set(_entityName, _sceneName, _speed); }
				internal static float s_Get(string _entityName, string _sceneName = "") { return VI.Animation.Speed.s_Get(_entityName, _sceneName); }
				internal static void Set(int _eId, float _speed) { VI.Animation.Speed.Set(_eId, _speed); }
				internal static float Get(int _eId) { return VI.Animation.Speed.Get(_eId); }
			}
			public struct CurrentFrame {
				internal static void s_Set(string _entityName, string _sceneName = "", int _index = 0) { VI.Animation.CurrentFrame.s_Set(_entityName, _sceneName, _index); }
				internal static int s_Get(string _entityName, string _sceneName = "") { return VI.Animation.CurrentFrame.s_Get(_entityName, _sceneName); }
				internal static void Set(int _eId, int _index) { VI.Animation.CurrentFrame.Set(_eId, _index); }
				internal static int Get(int _eId) { return VI.Animation.CurrentFrame.Get(_eId); }
			}
			public struct FrameCount {
				internal static void s_Set(string _entityName, string _sceneName = "", int _count = 0) { VI.Animation.FrameCount.s_Set(_entityName, _sceneName, _count); }
				internal static int s_Get(string _entityName, string _sceneName = "") { return VI.Animation.FrameCount.s_Get(_entityName, _sceneName); }
				internal static void Set(int _eId, int _count) { VI.Animation.FrameCount.Set(_eId, _count); }
				internal static int Get(int _eId) { return VI.Animation.FrameCount.Get(_eId); }
			}
		}
		private class SheetIndex
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_Set(string _entityName, string _sceneName = "", int _index = 0);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int s_Get(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Set(int _eId, int _index);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int Get(int _eId);
		}
		private class Speed
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_Set(string _entityName, string _sceneName = "", float _speed = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float s_Get(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Set(int _eId, float _speed);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float Get(int _eId);
		}
		private class CurrentFrame
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_Set(string _entityName, string _sceneName = "", int _index = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int s_Get(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Set(int _eId, int _index);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int Get(int _eId);
		}
		private class FrameCount
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_Set(string _entityName, string _sceneName = "", int _count = 8);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int s_Get(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Set(int _eId, int _count);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int Get(int _eId);
		}
	}
	public class MovementAI
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Run(int _eId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void ForceStop(int _eId);

        [MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool SetNext(int _eId, int _i); // return true if successful (withing 0 to the vector MAX)

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void GoToNext(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Stop(int _eId, bool _next = true);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void StopAfterEndLoop(int _eId, bool _loop = true);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void ReverseOrder(int _eId, bool _reverse = true);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetLoopCycle(int _eId, bool _cycle = true);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetCurrentIndex(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Remove(int _eId, int _i);

		public class Edit
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Timing(int _eId, float _time = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void CurrentTiming(int _eId, float _time = 0);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Current(int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void CurrentScaleX(int _eId, float _scaleX);
			
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void CurrentScaleY(int _eId, float _scaleY);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void CurrentRotate(int _eId, float _rot);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void CurrentPosX(int _eId, float _posX);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void CurrentPosY(int _eId, float _posY);
		}
		public class Get
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float Timing(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float CurrentTiming(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float CurrentScaleX(int _eId);
			
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float CurrentScaleY(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float CurrentRotate(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float CurrentPosX(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float CurrentPosY(int _eId);
		}
		public class Add {
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Transform(int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void TransformAt(int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1, int _index = 0);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void TransformScaleAt(int _eId, float _scaleX, float _scaleY, float _time = 1, int _index = 0);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void TransformRotateAt(int _eId, float _rot, float _time = 1, int _index = 0);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void TransformPosAt(int _eId, float _posX, float _posY, float _time = 1, int _index = 0);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void TransformDifference(int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1);
		}
		public struct AddAtCurrent
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Transform(int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void TransformScale(int _eId, float _scaleX, float _scaleY, float _time = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void TransformRotate(int _eId, float _rot, float _time = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void TransformPos(int _eId, float _posX, float _posY, float _time = 1);
		}
		public class SetCalculatedTime {
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void FromPosition(int _eId, float _posX, float _posY, int _step = -1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void FromRotation(int _eId, float _rot, int _step = -1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void FromScale(int _eId, float _scaleX, float _scaleY, int _step = -1);
		}
	}

	public class ColorAI {
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void StartAnimation(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void AddColor(int _eId, int _r, int _g, int _b, int _a, float _time = 1f);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void AddColorAt(int _eId, int _r, int _g, int _b, int _a, float _time = 1f, int _index = 0);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void RemoveColorAt(int _eId, int _index);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool SetNextStep(int _eId, int _i); // return ture if sucessful (withing 0 to the vector MAX)
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void StopAfterThisAnimation(int _eId, bool _next = true);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void StopAfterEndofAnimationLoop(int _eId, bool _loop = true);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void ReverseOrderAfterNextAnimation(int _eId, bool _reverse = true);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetAnimationLoopToCycle(int _eId, bool _cycle = true);

		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_StartAnimation(string _entityName, string _sceneName);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_AddColor(string _entityName, string _sceneName, int _r, int _g, int _b, int _a, float _time = 1f);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_AddColorAt(string _entityName, string _sceneName, int _r, int _g, int _b, int _a, float _time = 1f, int _index = 0);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_RemoveColorAt(string _entityName, string _sceneName, int _index);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool s_SetNextStep(string _entityName, string _sceneName, int _i); // return ture if sucessful (withing 0 to the vector MAX)
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_StopAfterThisAnimation(string _entityName, string _sceneName, bool _next = true);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_StopAfterEndofAnimationLoop(string _entityName, string _sceneName, bool _loop = true);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_ReverseOrderAfterNextAnimation(string _entityName, string _sceneName, bool _reverse = true);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_SetAnimationLoopToCycle(string _entityName, string _sceneName, bool _cycle = true);
	}

	public class ParticleSystem
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetDensity(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetDensity(int _eId, int _density);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetPosX(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetPosX(int _eId, float _posX);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetPosY(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetPosY(int _eId, float _posY);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetWidth(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetWidth(int _eId, float _width);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetDir(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetDir(int _eId, float _dir);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetSpread(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetSpread(int _eId, float _spread);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static float GetDuration(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetDuration(int _eId, float _dur);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool IsActive(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetIsActive(int _eId, bool _active = true);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetSlow(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetSlow(int _eId, int _slow);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GenerateOnce(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void GenerateLoop(int _eId, float _slow);

		public class ParticleInfo
        {
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetScale(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetDirection(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetLifeSpan(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetRotation(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetSpeed(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static bool GetFading(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int GetSpriteType(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static int GetLayer(int _eId);


			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetScale(int _eId, float _scale);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetDirection(int _eId, float _dir);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetLifeSpan(int _eId, float _lifespan);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetRotation(int _eId, float _rot);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetSpeed(int _eId, float _speed);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetFading(int _eId, bool _fade = true);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetColour(int _eId, int _r, int _g, int _b, int _a);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetSpriteType(int _eId, int _type);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetTexture(int _eId, string _texture);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetLayer(int _eId, int _type);
		}
	}
	public class Sprite
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_SetTexture(string _entityName, string _sceneName = "", string _path = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string s_GetTexture(string _entityName, string _sceneName = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetTexture(int _eId, string _path);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static string GetTexture(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetType(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetType(int _eId, int _type);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int s_GetType(string _entityName, string _sceneName = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_SetType(string _entityName, string _sceneName = "", int _type = 0);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetLayer(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetLayer(int _eId, int _layer);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int s_GetLayer(string _entityName, string _sceneName = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_SetLayer(string _entityName, string _sceneName = "", int layer = 63);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_SetColor(string _entityName, string _sceneName = "", int _r = 1, int _g = 1, int _b = 1, int _a = 1);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetColor(int _eId, int _r, int _g, int _b, int _a);

		[MethodImpl(MethodImplOptions.InternalCall)] // _rgba: 0 = red, 1 = green, 2 = blue, 3 = alpha
		internal extern static int s_GetColor(string _entityName, string _sceneName = "", int _rgba = 0);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetColor(int _eId, int _rgba = 0); // _rgba: 0 = red, 1 = green, 2 = blue, 3 = alpha

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_SetAlpha(string _entityName, string _sceneName = "", int _a = 1);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetAlpha(int _eId, int _a);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int s_GetAlpha(string _entityName, string _sceneName = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetAlpha(int _eId);
	}
	public class Audio
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_Play(string _entityName, string _sceneName = "");
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void s_PlayNow(string _entityName, string _sceneName = "");

        [MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_PlayOnLoop(string _entityName, string _sceneName = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_SetLoop(string _entityName, string _sceneName = "", bool _loop = true);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_Stop(string _entityName, string _sceneName = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Play(int _eId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void PlayNow(int _eId);

        [MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void PlayOnLoop(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void PlayBGM(string _soundName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static bool IsBgmPlaying();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Stop(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void StopAll();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void StopBGM();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void StopSFX();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void SetLoop(int _eId, bool _loop = true);
		public class Volume
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetVolume(float _vol);

            [MethodImpl(MethodImplOptions.InternalCall)]
            internal extern static void SetBGMVolume(float _vol);

            [MethodImpl(MethodImplOptions.InternalCall)]
            internal extern static void SetSFXVolume(float _vol);

            [MethodImpl(MethodImplOptions.InternalCall)]
            internal extern static float GetBgmVol();

            [MethodImpl(MethodImplOptions.InternalCall)]
            internal extern static float GetSfxVol();
        }
	}
	public class Transform
	{
		public class Position
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float s_GetX(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float s_GetY(string _entityName, string _sceneName = "");
			
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_SetX(string _entityName, string _sceneName = "", float _posX = 0);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_SetY(string _entityName, string _sceneName = "", float _posY = 0);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetX(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetY(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetX(int _eId, float _posX);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetY(int _eId, float _posY);
		}
		public class Scale
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float s_GetX(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float s_GetY(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_SetX(string _entityName, string _sceneName = "", float _scaleX = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_SetY(string _entityName, string _sceneName = "", float _scaleY = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetX(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetY(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetX(int _eId, float _scaleX);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void SetY(int _eId, float _scaleY);
		}
		public class Rotate
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float s_Get(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_Set(string _entityName, string _sceneName = "", float _rotate = 0);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float Get(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Set(int _eId, float _rotate);
		}
	}
	public class Text
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void s_Update(string _entityName, string _sceneName = "", string _text = "");
	
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int s_GetLineCount(string _entityName, string _sceneName = "");

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Update(int _eId, string _text);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetLineCount(int _eId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int SetColour(int _eId, int _r, int _g, int _b, int _a);
		public class Offset
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_Set(string _entityName, string _sceneName = "", float _xoffset = 0, float _yoffset = 0);
			
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float s_GetX(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float s_GetY(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Set(int _eId, float _xoffset, float _yoffset);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetX(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float GetY(int _eId);
		}
		public class Scale
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float s_Get(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_Set(string _entityName, string _sceneName = "", float _scale = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float Get(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Set(int _eId, float _scale);
		}
	}
	public class Weather
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void Init(float _width, float _height);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static int GetCurrent(int _index, float _posX, float _posY);
	}
	public class LightSource
	{
		public class Radius
		{
			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float s_Get(string _entityName, string _sceneName = "");

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void s_Set(string _entityName, string _sceneName = "", float _radius = 1);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static float Get(int _eId);

			[MethodImpl(MethodImplOptions.InternalCall)]
			internal extern static void Set(int _eId, float _radius);
		}
	}
}