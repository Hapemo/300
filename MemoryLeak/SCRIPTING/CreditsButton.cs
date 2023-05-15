/*!*****************************************************************************
\file CreditsButton.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 01-03-2023
\brief
This file contains scripting functions for logic system for button.
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class CreditsButton : BaseScript
    {
        float firstX, firstY;
        float bigX, bigY;
        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            firstX = THIS.Transform.Scale.GetX();
            firstY = THIS.Transform.Scale.GetY();
            bigX = firstX * 1.5f;
            bigY = firstY * 1.5f;
        }

        public void Init(int _ENTITY) {
            
        }

        public void EarlyUpdate(int _ENTITY) {
        
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            if (THIS.Input.Button.Hover())
            {
                THIS.Transform.Scale.SetX(bigX);
                THIS.Transform.Scale.SetY(bigY);
            }
            else
            {
                THIS.Transform.Scale.SetX(firstX);
                THIS.Transform.Scale.SetY(firstY);
            }

            if (THIS.Input.Button.Released())
            {
                VI.Scene.Pause("How_To_Play");
                VI.Scene.Pause("Menu_Main");
                VI.Scene.Pause("Settings");
                VI.Scene.Pause("Quit Confirmation");
                VI.Scene.Pause("TransitionScene");

                VI.Scene.Play("Credits");
            }
        }

        public void FixedUpdate(int _ENTITY) {

        }

        public void LateUpdate(int _ENTITY) {
        
        }

        public void Exit(int _ENTITY) {

        }

        public void Dead(int _ENTITY) {

        }
    }
}
