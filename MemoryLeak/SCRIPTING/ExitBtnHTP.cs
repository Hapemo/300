/*!*****************************************************************************
\file ExitBtnHTP.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
Controls the exit button
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class ExitBtnHTP : BaseScript
    {
        string currlevel;
        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            currlevel = VI.GameState.GetName();
        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            
            if (THIS.Input.Button.Released())
            {
                currlevel = VI.GameState.GetName();
                if (currlevel == "Menu")
                {
                    VI.Scene.Pause("How_To_Play");
                    VI.Scene.Pause("Settings");
                    VI.Scene.Pause("Credits");
                    VI.Scene.Pause("Quit Confirmation");
                    VI.Scene.Play("Menu_Main");
                    VI.Scene.Play("TransitionScene");
                    VI.Scene.Play("Pause");
                }
                else 
                {
                    VI.Scene.Pause("How_To_Play");
                    VI.Scene.Play("Pause");
                }
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
