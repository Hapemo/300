/*!*****************************************************************************
\file SwitchToLevel2.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
SwitchToLevel2 contains the logic that handles the logic of function calling to 
switch the game state to the second level
*******************************************************************************/

using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class SwitchToLevel3 : BaseScript
    {
        private int playerBoat;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            playerBoat = VI.Entity.GetId("Boat");
        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            if (VI.Physics.IsCollided(_ENTITY, playerBoat) && PassengerDeliverUI.DelieveredAllPassengers()) {
                TransitionSquare.FadeOut("CutScene4");
                //VI.GameState.Go("Level1");
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
