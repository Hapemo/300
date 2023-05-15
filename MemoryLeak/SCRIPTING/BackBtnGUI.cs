/*!*****************************************************************************
\file BackButton.cs
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
    public class BackBtnGUI : BaseScript
    {
        static int[] expandedRelics;
        int toggleMap;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            expandedRelics = new int[]
            {
                VI.Entity.GetId("Relic1Expanded"),
                VI.Entity.GetId("Relic2Expanded"),
                VI.Entity.GetId("Relic3Expanded"),
                VI.Entity.GetId("Relic4Expanded"),
                VI.Entity.GetId("Relic5Expanded"),
                VI.Entity.GetId("Relic6Expanded")
            };
            toggleMap = VI.Entity.GetId("toggleMap");

        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            //Init(_ENTITY);

            if (THIS.Input.Button.Released())
            {
                foreach (int relic in expandedRelics)
                    VI.Entity.Deactivate(relic);

                string gsname = VI.GameState.GetName();

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

                VI.Scene.Pause("GUI Scene Expanded");

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
