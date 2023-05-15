/*!*****************************************************************************
\file CutSceneScript.cs
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
    public class CutSceneFinalScript : BaseScript
    {
        int currScn;
        int maxScn;
        int currAlpha;
        int skip;
        int speed;
        bool isAlphaDecreasing;
        string entity;
        bool skipping;
        //bool soundPlayed;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY);

        }

        public void Init(int _ENTITY) {

            VI.Audio.PlayBGM("Ending_Cutscene");

            skip = VI.Entity.GetId("Skip");
            currScn = 0;
            maxScn = 16;
            isAlphaDecreasing = false;
            entity = "e" + currScn;
            currAlpha = 255;
            speed = 5;
            //soundPlayed = false;
            skipping = false;
        }

        public void EarlyUpdate(int _ENTITY) { }

        public void Update(int _ENTITY) {
            Console.WriteLine("entityid:" + skip);
            if (VI.Input.Button.Released(skip))
            {
                    TransitionSquare.FadeOut("Menu");
                skipping = true;
                return;
            }
            if (currScn >= maxScn)
            {
                if (!isAlphaDecreasing)
                {
                    if (!skipping)
                    {
                       
                            TransitionSquare.FadeOut("Menu");

                    }
                }
            }
            if (!skipping && !isAlphaDecreasing && VI.Input.Key.Release(349) && currScn < maxScn)
                isAlphaDecreasing = true;
        }

        public void FixedUpdate(int _ENTITY) {
            if (isAlphaDecreasing)
                UpdateAlpha();

        }

        public void UpdateAlpha()
        {
            if (skipping)
            {
                if (currAlpha > 0)
                {
                    currAlpha -= speed;
                    if (currAlpha < 0) currAlpha = 0;
                    VI.Sprite.SetColor(VI.Entity.GetId("e" + currScn), 255, 255, 255, currAlpha);

                    for (int i = currScn + 1; i < maxScn; ++i)
                        VI.Sprite.SetColor(VI.Entity.GetId("e" + currScn), 255, 255, 255, 0);
                }
                if (currAlpha == 0)
                {
                    currAlpha = 255;
                    isAlphaDecreasing = false;
                    currScn = maxScn;
                }
            }
            else
            {
                if (currAlpha > 0)
                {
                    currAlpha -= speed; 
                    if (currAlpha < 0)
                        currAlpha = 0;
                    VI.Sprite.SetColor(VI.Entity.GetId("e" + currScn), 255, 255, 255, currAlpha);
                }
                if (currAlpha == 0)
                {
                    currAlpha = 255;
                    isAlphaDecreasing = false;
                    ++currScn;
                    entity = "e" + currScn;
                }
            }
        }
        public void LateUpdate(int _ENTITY) { }

        public void Exit(int _ENTITY) {
            VI.Audio.StopBGM();
        }

        public void Dead(int _ENTITY) {

        }
    }
}
