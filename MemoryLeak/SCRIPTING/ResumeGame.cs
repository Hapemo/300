/*!*****************************************************************************
\file ResumeGame.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 01-03-2023
\brief
This file contains scripting functions for logic system for resuming game.
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class ResumeGame : BaseScript
    {
        private int toggleMap;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            toggleMap = VI.Entity.GetId("toggleMap");

        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            if (THIS.Input.Button.Hover())
                THIS.Sprite.SetColor(0, 0, 0, 100);
            else
                THIS.Sprite.SetColor(0, 0, 0, 0);
            if (THIS.Input.Button.Released())
            {
                string gsname = VI.GameState.GetName();

                if (gsname == "LevelTutorial")
                {
                    VI.Scene.Play(gsname);
                    VI.Scene.Play(gsname + "Background");
                    //VI.Scene.Pause("WeatherScene");
                    VI.Scene.SetForceRender(gsname, false);
                    VI.Scene.SetForceRender(gsname + "Background", false);
                    //VI.Scene.SetForceRender("WeatherScene", true);

                    //VI.Scene.Pause("GUI Scene");
                    VI.Scene.Play("TutorialUI");
                    VI.Scene.Play("TutorialNameFade");

                    //VI.Entity.Deactivate(toggleMap);

                    VI.Scene.Pause("Pause");
                }
                else
                {
                    VI.Scene.Play(gsname);
                    VI.Scene.Play(gsname + "Background");
                    VI.Scene.Play("WeatherScene");
                    VI.Scene.SetForceRender(gsname, false);
                    VI.Scene.SetForceRender(gsname + "Background", false);
                    VI.Scene.SetForceRender("WeatherScene", false);

                    VI.Scene.Play("GUI Scene");
                    VI.Scene.Play("TutorialUILvl" + gsname[gsname.Length - 1]);
                    VI.Scene.Play("L" + gsname[gsname.Length - 1] + "NameFade");

                    VI.Entity.Activate(toggleMap);

                    VI.Scene.Pause("Pause");
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
