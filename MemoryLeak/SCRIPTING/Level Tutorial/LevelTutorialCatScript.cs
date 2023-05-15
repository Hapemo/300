/*!*****************************************************************************
\file LevelTutorialCatScript.cs
\author Lee Hsien Wei Joachim
\par DP email: l.hsienweijoachim@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
LevelTutorialCatScript contains the logic that sets the level's dialog
manager variable to true when the player collides with its trigger box
*******************************************************************************/

using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class LevelTutorialCatScript : BaseScript {
        private int playerBoat;
        private int triggerBox;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            playerBoat = VI.Entity.GetId("Boat");
            triggerBox = VI.Entity.GetId("CatBox");
        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            // Check if cat's trigger box is active
            if (VI.Entity.IsActive(triggerBox)) { 
                // Check if player is colliding with that box
                if (VI.Physics.IsCollided(triggerBox, playerBoat)) {
                    // Set Dialog Manager's flag to true and run it
                    // LevelTutorialDialogManager
                    LevelTutorialDialogManager.runCatDialog = true;
                    
                    // Deactivate the trigger box
                    VI.Entity.Deactivate(triggerBox);

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
