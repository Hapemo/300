/*!*****************************************************************************
\file LevelTutorialPassengerScript.cs
\author Lee Hsien Wei Joachim
\par DP email: l.hsienweijoachim@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
LevelTutorialPassengerScript contains the logic that handles the logic of the animation
calls of the passenger in the tutorial level and setting the level's dialog
manager variable to true when the player collides with its trigger box
*******************************************************************************/

using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class LevelTutorialPassengerScript : PassengerBaseScript {

        //private int level1Transition_Box;

        public override void Alive(int _ENTITY) {
            base.Alive(_ENTITY);    
        }

        public override void Init(int _ENTITY) {
            base.Init(_ENTITY);

            // Find required entities
            playerBoat = VI.Entity.GetId("Boat");
            triggerBox = VI.Entity.GetId("PassengerPickupBox");

            correctDestination_Box = VI.Entity.GetId("PassengerDropOffBox");
            correctDestination_RenderLocation = VI.Entity.GetId("PassengerDummyDropOffBox");

            wrongDestination_Box = VI.Entity.GetId("PassengerDummyDropOffBox");
            wrongDestination_RenderLocation = VI.Entity.GetId("PassengerDummyDropOffBox");

            //level1Transition_Box = VI.Entity.GetId("TutorialEndCollider");
            
            // Store original scale and layer values
            //InitialScaleX = VI.Transform.Scale.GetX(_ENTITY);
            //InitialLayerVal = VI.Sprite.GetLayer(_ENTITY);
        }

        public override void EarlyUpdate(int _ENTITY) {
            base.EarlyUpdate(_ENTITY);
        }

        public override void Update(int _ENTITY) {
            base.Update(_ENTITY);

            // Check if passenger's trigger box is active
            if (VI.Entity.IsActive(triggerBox)) {
                // Check if player is colliding with that box
                if (VI.Physics.IsCollided(triggerBox, playerBoat)) {
                    // Check if dialogue script should be ran
                    if (!LevelTutorialDialogManager.runPassengerDialog) {
                        // Set Dialogue Manager's flag to true to run it
                        LevelTutorialDialogManager.runPassengerDialog = true;
                        
                        // Deactivate the trigger box
                        VI.Entity.Deactivate(triggerBox);

                        // Activate the drop off point boxes
                        VI.Entity.Activate(correctDestination_Box);
                        VI.Entity.Activate(wrongDestination_Box);

                        // Set flag to true
                        ReadyToAttach = true;
                    }
                }
            }

            // Passenger is ready to attach & dialogue has ended
            if (ReadyToAttach && !LevelTutorialDialogManager.runPassengerDialog) {
                // Set flag variables
                fadingOut = true;
                AttachToPlayerAnimation = true;
                PlayerScript.PlayerInOtherAnimation = true;
                ReadyToAttach = false;
            }

            // For transition of passenger location to player's boat, to use particle system to indicate the transfer

            // Play animation of passenger attaching to player
            if (AttachToPlayerAnimation) {
                DefaultAttachToPlayer(_ENTITY);
            }

            // Check if passenger reaches destination
            if (VI.Physics.CheckCollision(correctDestination_Box, _ENTITY, false)) {
                // Move on to detaching animation
                AttachedToPlayer = false;
                DetachFromPlayerAnimation = true;
                PlayerScript.PlayerInOtherAnimation = true;
                
                // Set flag
                correctDestinationDelivery = true;
            }
            else if (VI.Physics.CheckCollision(wrongDestination_Box, _ENTITY, false)) {
                // Move on to detaching animation
                AttachedToPlayer = false;
                DetachFromPlayerAnimation = true;
                PlayerScript.PlayerInOtherAnimation = true;
                LevelTutorialDialogManager.runPassengerEndDialog = true;

                // Set flag
                correctDestinationDelivery = false;
            }

            // Play animation of passenger detaching from player
            if (DetachFromPlayerAnimation) {
                // Update passenger layer to its original value
                //VI.Sprite.SetLayer(_ENTITY, InitialLayerVal);
                DefaultAttachToDestination(_ENTITY);
            }
        
            // Passenger was delivered
            if (DestinationReached) {
                // Return layer value
                VI.Sprite.SetLayer(_ENTITY, InitialLayerVal);

                // Restore passenger's original scale value
                // Returns true when completed
                if (RestorePassengerScale(_ENTITY, InitialScaleX)){
                    // Run logic based on whether destination delievered was correct
                    switch (correctDestinationDelivery) {
                        case true:
                            // Run second script/logic from here
                            break;
                        case false:
                            // Run second script/logic from here
                            break;
                        default:
                            // Do nothing
                            break;
                    }
                    // Deactivate the drop off point boxes
                    VI.Entity.Deactivate(correctDestination_Box);
                    VI.Entity.Deactivate(wrongDestination_Box);

                    // Activate entity to transition to level 1 once player touches it
                    //VI.Entity.Activate(level1Transition_Box);
                    if (VI.Transform.Position.GetX(playerBoat) < VI.Transform.Position.GetX(_ENTITY)) 
                        VI.Animation.SpriteSheet.SheetIndex.Set(_ENTITY, 5);
                    else 
                        VI.Animation.SpriteSheet.SheetIndex.Set(_ENTITY, 4);
                    

                    // Set flag to get out
                    DestinationReached = false;
                }
            }
        }

        public override void FixedUpdate(int _ENTITY) {
            base.FixedUpdate(_ENTITY);
        }

        public override void LateUpdate(int _ENTITY) {
            base.LateUpdate(_ENTITY);

            // Move passenger with player       
            if (AttachedToPlayer)
            {
                // Move passenger with player
                MovePassengerWithPlayer(_ENTITY, playerBoat, InitialScaleX);
            }
        }

        public override void Exit(int _ENTITY) {
            base.Exit(_ENTITY);
        }

        public override void Dead(int _ENTITY) {
            base.Dead(_ENTITY);
        }
    }
}
