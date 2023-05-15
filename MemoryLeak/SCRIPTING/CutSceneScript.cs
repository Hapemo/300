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
    public class CutSceneScript : BaseScript
    {
        int currScn;
        int maxScn;
        int currAlpha;
        int skip;
        int speed;
        bool isAlphaDecreasing;
        string entity;
        const string scene = "CutScene";
        int light;
        int lightning;
        bool skipping;
        int lightningAlpha;
        bool soundPlayed;
        int gs;
        int targetRadius;
        int currRadius;
        int animationCounter;
        int nextSceneTimer;
        //int nextSceneTextTimer;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY);

        }

        public void Init(int _ENTITY) {
            light = VI.Entity.GetId("light");
            gs = VI.GameState.GetName()[VI.GameState.GetName().Length - 1] - '0';
            if (gs != 4)
                VI.Audio.PlayBGM("Opening_Cutscene(Long)");
            else
                VI.Audio.PlayBGM("Stormy_Cutscene");
            if (gs == 3)
                lightning = VI.Entity.GetId("lightning");
            targetRadius = (int)VI.LightSource.Radius.Get(light);
            skip = VI.Entity.GetId("Skip");
            currScn = 0;
            if (gs == 1)
                maxScn = 4;
            else if (gs == 2)
                maxScn = 4;
            else if (gs == 3)
                maxScn = 6;
            else
                maxScn = 5;
            isAlphaDecreasing = false;
            entity = "e" + currScn;
            currAlpha = 255;
            speed = 5;
            lightningAlpha = 255;
            soundPlayed = false;
            currRadius = 2000;
            skipping = false;
            animationCounter = 0;
            nextSceneTimer = 250;
           // nextSceneTextTimer = 350;
        }

        public void EarlyUpdate(int _ENTITY) { }

        public void Update(int _ENTITY) {
            if (VI.Input.Button.Released(skip))
            {
                if (gs == 1)
                    TransitionSquare.FadeOut("LevelTutorial");
                else if (gs == 2)
                    TransitionSquare.FadeOut("Level1");
                else if (gs == 3)
                    TransitionSquare.FadeOut("Level2");
                else
                    TransitionSquare.FadeOut("Level3");
                skipping = true;
                return;
            }
            if (currScn >= maxScn)
            {
                if (!isAlphaDecreasing)
                {
                    if (!skipping)
                    {
                        if (gs == 1)
                            TransitionSquare.FadeOut("LevelTutorial");
                        else if (gs == 2)
                            TransitionSquare.FadeOut("Level1");
                        else if (gs == 3)
                            TransitionSquare.FadeOut("Level2");
                        else
                            TransitionSquare.FadeOut("Level3");

                    }
                }
            }
            if (!skipping && !isAlphaDecreasing && VI.Input.Key.Release(349) && currScn < maxScn)
                isAlphaDecreasing = true;
            //if (!skipping && !isAlphaDecreasing && animationCounter >= nextSceneTextTimer && currScn == INSERT TEXT SCENE ID)
            //    isAlphaDecreasing = true;
            if (!skipping && !isAlphaDecreasing && animationCounter >= nextSceneTimer && currScn < maxScn)
                isAlphaDecreasing = true;
        }

        public void FixedUpdate(int _ENTITY) {
            ++animationCounter;
            if (isAlphaDecreasing)
                UpdateAlpha();
            if (currScn == 4 && gs == 3 && !skipping)
            {
                UpdateLightning();
                if (!soundPlayed)
                {
                    soundPlayed = true;
                    VI.Audio.Play(lightning);
                    VI.Audio.PlayBGM("Stormy_Cutscene");
                }
            }
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
                    animationCounter = 0;
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
                    animationCounter = 0;
                    ++currScn;
                    entity = "e" + currScn;
                }
            }
        }

        public void UpdateLightning()
        {
            if (lightningAlpha != 0)
            {
                lightningAlpha -= speed;
                if (lightningAlpha < 0)
                    lightningAlpha = 0;
                VI.Sprite.SetColor(lightning, 255, 255, 255, lightningAlpha);
            }
            if (currRadius != targetRadius)
            {
                currRadius -= 20;
                if (currRadius < targetRadius)
                    currRadius = targetRadius;
                VI.LightSource.Radius.Set(light,currRadius);
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
