/*!*****************************************************************************
\file GameOver.cs
\author Lee Hsien Wei, Joachim
\par DP email: l.hsienweijoachim@digipen.edu
\par Group: Memory Leak Studios
\date 04-03-2023
\brief
GameOver script that handles the change in game over scene after player dies and is deactivated
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class GameOver : BaseScript
    {
        private bool init = false;
        private int playerBoat = 0;

        private double gameOverDelay = 1.0;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            playerBoat = VI.Entity.GetId("Boat");

            gameOverDelay = 1.0;

            init = true;
        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Update(int _ENTITY) {
            if (init == false)
                Init(_ENTITY);

            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            if (PlayerScript.PlayerInDeathAnimation && VI.Entity.IsActive(playerBoat) == false) {
                // Delay game over scene as requested by wei jhin
                if (gameOverDelay < 0.0) {
                    // Assumes the main game scene level running is the same as the gamestate name
                    if (VI.GameState.GetName() == "LevelTutorial") {
                        VI.Scene.Play("Game Over");
                        VI.Scene.Pause("LevelTutorial");
                        VI.Scene.Pause("Pause");
                        VI.Scene.Pause("How_To_Play");
                        VI.Scene.Pause("Quit Confirmation");
                        VI.Scene.Pause("TutorialDialogue");
                        VI.Scene.Pause("Transition");
                        VI.Scene.Pause("TutorialUI");
                        VI.Scene.Pause("LevelTutorialBackground");
                    }
                    else if (VI.GameState.GetName() == "Level1") {
                        VI.Scene.Play("Game Over");
                        VI.Scene.Pause("Level1");
                        VI.Scene.Pause("Pause");
                        VI.Scene.Pause("How_To_Play");
                        VI.Scene.Pause("Quit Confirmation");
                        VI.Scene.Pause("Dialogue");
                        VI.Scene.Pause("CrystalBalls");
                        VI.Scene.Pause("MiniMap");
                        VI.Scene.Pause("EnemyMap");
                        VI.Scene.Pause("WeatherMap");
                        VI.Scene.Pause("Level1Background");
                        VI.Scene.Pause("Transition");
                        VI.Scene.Pause("TutorialUILvl1");
                        VI.Scene.Pause("GUI Scene");
                        VI.Scene.Pause("WeatherScene");
                        VI.Scene.Pause("GUI Scene Expanded");
                        VI.Scene.Pause("Lv1Firefly");
                    }
                    else if (VI.GameState.GetName() == "Level2") {
                        VI.Scene.Play("Game Over");
                        VI.Scene.Pause("Level2");
                        VI.Scene.Pause("Pause");
                        VI.Scene.Pause("How_To_Play");
                        VI.Scene.Pause("Dialogue2");
                        VI.Scene.Pause("Quit Confirmation");
                        VI.Scene.Pause("CrystalBalls");
                        VI.Scene.Pause("Level2Background");
                        VI.Scene.Pause("MiniMap");
                        VI.Scene.Pause("WeatherMap");
                        VI.Scene.Pause("EnemyMap");
                        VI.Scene.Pause("GUI Scene");
                        VI.Scene.Pause("Transition");
                        VI.Scene.Pause("TutorialUILvl2");
                        VI.Scene.Pause("WeatherScene");
                        VI.Scene.Pause("GUI Scene Expanded");
                        VI.Scene.Pause("Lv2Firefly");
                        // Play audio transition if required
                    }
                    else if (VI.GameState.GetName() == "Level3") {
                        VI.Scene.Play("Game Over");
                        VI.Scene.Pause("Level3");
                        VI.Scene.Pause("Level3Background");
                        VI.Scene.Pause("Dialogue3");
                        VI.Scene.Pause("GUI Scene");
                        VI.Scene.Pause("GUI Scene Expanded");
                        VI.Scene.Pause("Lv3Firefly");
                        VI.Scene.Pause("CrystalBalls");
                        VI.Scene.Pause("MiniMap");
                        VI.Scene.Pause("EnemyMap");
                        VI.Scene.Pause("WeatherMap");
                        VI.Scene.Pause("WeatherScene");
                        VI.Scene.Pause("Pause");
                        VI.Scene.Pause("How_To_Play");
                        VI.Scene.Pause("Quit Confirmation");
                        VI.Scene.Pause("Transition");
                    }
                    else {
                        // Unknown game state the player is in???
                    }
                }
                else {
                    gameOverDelay -= VI.General.DeltaTime();
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

            Init(_ENTITY);
        }

        public void Dead(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }
    }
}
