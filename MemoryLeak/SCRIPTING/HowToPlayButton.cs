/*!*****************************************************************************
\file HowToPlayButton.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 01-03-2023
\brief
This file contains scripting functions for logic system for buttons.
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class HowToPlayButton : BaseScript
    {
        string currlevel;
        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY) {

        }

        public void EarlyUpdate(int _ENTITY) {
        
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY);
            if (THIS.Input.Button.Hover())
                THIS.Sprite.SetColor(0, 0, 0, 100);
            else
                THIS.Sprite.SetColor(0, 0, 0, 0);

            if (THIS.Input.Button.Released())
            {
                currlevel = VI.GameState.GetName();
                if (currlevel == "Menu")
                {
                    VI.Scene.Pause("Menu_Main");
                    VI.Scene.Pause("Settings");
                    VI.Scene.Pause("Credits");
                    VI.Scene.Pause("Quit Confirmation");
                    VI.Scene.Pause("TransitionScene");
                    
                    VI.Scene.Play("How_To_Play");
                }
                else
                { 
                    VI.Scene.Pause("Pause");
                    VI.Scene.Play("How_To_Play");
                }
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
