/*!*****************************************************************************
\file Level2Passenger1Script.cs
\author Lee Hsien Wei Joachim
\par DP email: l.hsienweijoachim@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
Level2Passenger1Script contains the logic that handles the logic of the animation
calls of the first passenger in the second level and setting the level's dialog
manager variable to true when the player collides with its trigger box
*******************************************************************************/

using System;
using System.Runtime.CompilerServices;
using VI;

namespace BonVoyage {

  public class Level2LittleGirlScript : PassengerBaseScript {

    private int firstPassengerID;
    private int firstPassengerBox;
    public override void Alive(int _ENTITY) {
      base.Alive(_ENTITY);
    }

    public override void Init(int _ENTITY) {
      base.Init(_ENTITY);

      // Get required entities
      playerBoat = VI.Entity.GetId("Boat");
      triggerBox = VI.Entity.GetId(Level2DialogManager.LittleGirlBoxString);

      firstPassengerID = VI.Entity.GetId(Level2DialogManager.P1String);
      firstPassengerBox = VI.Entity.GetId(Level2DialogManager.P1BoxString);
    }

    public override void EarlyUpdate(int _ENTITY) {
      base.EarlyUpdate(_ENTITY);
    }

    public override void Update(int _ENTITY) {
      base.Update(_ENTITY);

      // Check if passenger's trigger box is active
      if (VI.Entity.IsActive(triggerBox) ) {
        if (VI.Physics.IsCollided(triggerBox, playerBoat))
        if (!Level2DialogManager.runLittleGirlDialog) {
          Level2DialogManager.runLittleGirlDialog = true;
          VI.Entity.Deactivate(triggerBox);

          VI.Entity.Activate(firstPassengerID);
          VI.Entity.Activate(firstPassengerBox);
        }
      }

    }
    public override void FixedUpdate(int _ENTITY) {
      base.FixedUpdate(_ENTITY);
    }

    public override void LateUpdate(int _ENTITY) {
      base.LateUpdate(_ENTITY);
    }

    public override void Exit(int _ENTITY) {
      base.Exit(_ENTITY);
    }

    public override void Dead(int _ENTITY) {
      base.Dead(_ENTITY);
    }
  }
}