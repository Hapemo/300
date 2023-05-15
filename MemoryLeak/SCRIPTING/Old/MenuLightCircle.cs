/*!*****************************************************************************
\file MenuLightCircle.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 27-01-2023
\brief
This script contains functions to alter the light circle in the menu of the
game.
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class MenuLightCircle
    {
        float size;
        float alpha;
        float stepSize, stepAlpha;
        float upperSize, lowerSize;
        float upperAlpha, lowerAlpha;
        bool increasing;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY) {
            size = 110;
            lowerSize = 110;
            upperSize = 120;

            alpha = 150;
            lowerAlpha = 150;
            upperAlpha = 255;

            stepSize = (upperSize - lowerSize) / 0.4f;
            stepAlpha = (upperAlpha - lowerAlpha) / 0.4f;

            increasing = true;
        }

        public void EarlyUpdate(int _ENTITY) { }

        public void Update(int _ENTITY) {
            UpdateSizeAlpha();
            VI.Transform.Scale.s_SetX("LightCircle", "Menu_Main", size);
            VI.Transform.Scale.s_SetY("LightCircle", "Menu_Main", size);
            VI.LightSource.SpriteColor.s_Set("LightCircle", "Menu_Main", 255, 255, 255, (int)alpha);
        }

        void UpdateSizeAlpha()
        {
            if (increasing)
            {
                size += stepSize * (float)VI.General.DeltaTime();
                alpha += stepAlpha * (float)VI.General.DeltaTime();
                if (size > upperSize)
                {
                    size = upperSize;
                    alpha = upperAlpha;
                    increasing = false;
                }
            }
            else
            {
                size -= stepSize * (float)VI.General.DeltaTime();
                alpha -= stepAlpha * (float)VI.General.DeltaTime();
                if (size < lowerSize)
                {
                    size = lowerSize;
                    alpha = lowerAlpha;
                    increasing = true;
                }
            }
        }

        public void FixedUpdate(int _ENTITY) {

        }

        public void LateUpdate(int _ENTITY) { }

        public void Exit(int _ENTITY) {
            
        }
        public void Dead(int _ENTITY) {

        }
    }
}
