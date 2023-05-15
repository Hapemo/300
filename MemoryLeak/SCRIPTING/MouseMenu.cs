/*!*****************************************************************************
\file MouseMenu.cs
\author Lee Hsien Wei Joachim
\par DP email: l.hsienweijoachim@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
Animates mouse entity icon based on player action
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class MouseMenu : BaseScript {
        private float ZoomScaleFactorX;
        private float ZoomScaleFactorY;

        private bool InAnimation = false;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            //playerBoat = VI.Entity.GetId("Boat", "Menu_Main");
        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            //playerBoat = VI.Entity.GetId("Boat", "Menu_Main");
            InAnimation = false;
        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            // To change hardcoded value to get application's width n height when functions are added to internal calls or the compution of mouse world position is corrected to be in the world
            ZoomScaleFactorX = VI.Camera.GetScale.X() / VI.Window.GetScreenWidth();
            ZoomScaleFactorY = VI.Camera.GetScale.Y() / VI.Window.GetScreenHeight();

            VI.Transform.Position.SetX(_ENTITY, VI.Camera.GetPos.X() + ZoomScaleFactorX * VI.Input.Mouse.WorldPosX() + VI.Transform.Scale.GetX(_ENTITY) / 2f);
            VI.Transform.Position.SetY(_ENTITY, VI.Camera.GetPos.Y() + ZoomScaleFactorY * VI.Input.Mouse.WorldPosY() - VI.Transform.Scale.GetY(_ENTITY) / 2f);

            //if (VI.Input.Mouse.Hold()) {
            //    if (!InAnimation) {
            //        float DirX = VI.Input.Mouse.WorldPosX() + VI.Camera.GetPos.X() - VI.Transform.Position.GetX(playerBoat);
            //        float DirY = VI.Input.Mouse.WorldPosY() + VI.Camera.GetPos.Y() - VI.Transform.Position.GetY(playerBoat);
            //        SetMouseMovingIcon(_ENTITY, GetRotation(DirX, DirY));

            //        InAnimation = true;
            //    }
            //}
            if (VI.Input.Mouse.Press())
            {
                if (!InAnimation)
                {
                    VI.Animation.SpriteSheet.SheetIndex.Set(_ENTITY, 1);
                    VI.Animation.SpriteSheet.CurrentFrame.Set(_ENTITY, 0);
                    VI.Animation.SpriteSheet.FrameCount.Set(_ENTITY, 2);
                    InAnimation = true;
                }
            }
            else if (VI.Input.Mouse.Release()) {
                InAnimation = false;
            }
            
            if (!InAnimation) { 
                VI.Animation.SpriteSheet.SheetIndex.Set(_ENTITY, 0);
                VI.Animation.SpriteSheet.CurrentFrame.Set(_ENTITY, 0);
                VI.Animation.SpriteSheet.FrameCount.Set(_ENTITY, 1);
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
