/*!*****************************************************************************
\file Fragment1.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 01-03-2023
\brief
This file contains scripting functions for logic system for fragments.
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class Fragment1 : BaseScript
    {
        bool increasingalpha;
        float currclr;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            increasingalpha = false;
            currclr = 0;

        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            increasingalpha = true;
        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            if (increasingalpha)
                IncreaseAlpha();

        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void FixedUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void LateUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Exit(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            increasingalpha = false;
            currclr = 255;
            THIS.Sprite.SetColor((int)currclr, (int)currclr, (int)currclr, 255);

        }

        public void Dead(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        private void IncreaseAlpha()
        {
            currclr += 2;

            if (currclr > 255)
            {
                currclr = 255;
                increasingalpha = false;
            }
            THIS.Sprite.SetColor((int)currclr, (int)currclr, (int)currclr, 255);
        }
    }
}
