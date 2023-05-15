/*!*****************************************************************************
\file MenuLightScript.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 27-01-2023
\brief
This script contains functions to alter the light in the menu of the game.
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class MenuLightScript : BaseScript
    {
        float radius;
        float speed;
        bool increasing;
        float upper, lower;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY) {
            radius = VI.LightSource.Radius.s_Get("Title", "Menu_Main");
            speed = 500;
            increasing = false;
            upper = 1100;
            lower = 900;
        }

        public void EarlyUpdate(int _ENTITY) { }

        public void Update(int _ENTITY) {
            UpdateRadius();
            VI.LightSource.Radius.s_Set("Title", "Menu_Main", radius);
            Console.Write(THIS.IsActive());
        }

        public void FixedUpdate(int _ENTITY) {

        }

        public void LateUpdate(int _ENTITY) { }

        public void Exit(int _ENTITY) {

        }

        public void Dead(int _ENTITY) {

        }

        void UpdateRadius()
        {
            if (increasing)
            {
                radius += speed * (float)VI.General.DeltaTime();
                if (radius > upper)
                {
                    increasing = false;
                    radius = upper;
                }
            }
            else
            {
                radius -= speed * (float)VI.General.DeltaTime();
                if (radius < lower)
                {
                    increasing = true;
                    radius = lower;
                }
            }
        }
    }
}
