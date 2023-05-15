/*!*****************************************************************************
\file SFXcontroller.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 01-03-2023
\brief
This file contains scripting functions for setting scene's volume control
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class SFXcontroller : BaseScript
    {
        bool isIn;
        bool inited = false;
        public void Alive(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void Init(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            isIn = false;
            inited = true;
            float currvol = VI.Audio.Volume.GetSfxVol();
            VI.Transform.Scale.s_SetX("SFXvolume", "Settings", THIS.Transform.Scale.GetX() * currvol);
            VI.Transform.Position.s_SetX("SFXvolume", "Settings", THIS.Transform.Position.GetX() - 
                THIS.Transform.Scale.GetX() * 0.5f + VI.Transform.Scale.s_GetX("SFXvolume","Settings") * 0.5f);
        }

        public void EarlyUpdate(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void Update(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            if (!inited)
                Init(_ENTITY);
            if (THIS.Input.Button.Clicked())
                isIn = true;
            if (isIn && VI.Input.Mouse.Release())
                isIn = false;
            if (isIn)
            {
                float ZoomScaleFactorX = VI.Camera.GetScale.X() / VI.Window.GetScreenWidth();
                float mouseX = VI.Camera.GetPos.X() + ZoomScaleFactorX * VI.Input.Mouse.WorldPosX();
                if (mouseX > THIS.Transform.Position.GetX() + 0.5 * THIS.Transform.Scale.GetX() ||
                    mouseX < THIS.Transform.Position.GetX() - 0.5 * THIS.Transform.Scale.GetX())
                    return;
                VI.Transform.Scale.s_SetX("SFXvolume", "Settings", 0.5f * THIS.Transform.Scale.GetX() - (THIS.Transform.Position.GetX() - mouseX));
                VI.Transform.Position.s_SetX("SFXvolume", "Settings", THIS.Transform.Position.GetX() - 0.5f * THIS.Transform.Scale.GetX() +VI.Transform.Scale.s_GetX("SFXvolume", "Settings") * 0.5f);
                VI.Audio.Volume.SetSFXVolume(VI.Transform.Scale.s_GetX("SFXvolume", "Settings") / THIS.Transform.Scale.GetX());
            }
        }


        public void FixedUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void LateUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Exit(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void Dead(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }
    }
}
