/*!*****************************************************************************
\file Level1DialogManager.cs
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
Level1DialogManager contains the logic that handles the conversation between
the player and/or the different passengers in level 1 as well as the camera
transition
*******************************************************************************/

using System;
using System.Dynamic;
using System.Runtime.CompilerServices;
using VI;

// INFO
// Intro Dialog zoom is x = 1200
// Static Dialog zoom is x = 960
// Normal zoom is x = 1600

namespace BonVoyage {
  public class Level1DialogManager : DialogManager {

    public const string P1String = "Passenger1";
    public const string P1BoxString = "Passenger1Box";
    public const string P1CorrectBoxString = "DoubleStoryHouseDropOffPoint";
    public const string P1WrongBoxString = "LighthouseDropOffPoint";
    public const string P1CorrectRenderString = "DoubleStoryHouseDestRender";
    public const string P1WrongRenderString = "LighthouseDestRender";


    public const string P2String = "Passenger2";
    public const string P2BoxString = "Passenger2Box";
    public const string P2CorrectBoxString = "PortHouseDropOffPoint";
    public const string P2WrongBoxString = "FountainDropOffPoint";
    public const string P2CorrectRenderString = "PortHouseDest2Render";
    public const string P2WrongRenderString = "FountainDestRender";
    //private bool progressUpdate = false;
    //static public bool runGirlDialog;

    private int correctDestination_RenderLocation;
    private int wrongDestination_RenderLocation;

    static public bool runPassengerDialog;
    private int P1ColliderBox;
    
    static public bool runPassenger2Dialog;
    private int P2ColliderBox;


    public override void Alive(int _ENTITY) {
        base.Alive(_ENTITY);
    }

    public override void Init(int _ENTITY) {
        base.Init(_ENTITY);

      P1ColliderBox = VI.Entity.GetId(P1BoxString);
      P2ColliderBox = VI.Entity.GetId(P2BoxString);
      UIPassengersLeftID = VI.Entity.GetId("PassengersLeft");
      UIPassengerIcon1ID = VI.Entity.GetId("PassengerIcon1");
      UIPassengerIcon2ID = VI.Entity.GetId("PassengerIcon2");
      dialogInit = true;
      normalZoom = false;

      VI.Text.Update(UIObjectiveTextID, "Objective: Continue finding the lost souls");
      VI.Entity.s_Deactivate(P2String);
      VI.Entity.Deactivate(P2ColliderBox);
    }

    public override void EarlyUpdate(int _ENTITY) {
        base.EarlyUpdate(_ENTITY);
    }

    public override void Update(int _ENTITY) {
        base.Update(_ENTITY);

      if (!normalZoom)
        normalZoom =  ChangeZoom(1600, 300);


      if (runPassengerDialog) {
        GeneralDialogStart(1);
        MoveCameraRightToDialog();
        MovePlayer(playerID, VI.Transform.Position.GetX(P1ColliderBox), 
                                                 VI.Transform.Position.GetY(P1ColliderBox)); // Move him to better location
        runPassengerDialog = RunDialog(P1ID, G1ID, PP1ID, PP2ID, "Dialog Minerva"); // Run the dialog
        if (!runPassengerDialog)
          EndPassengerDialog();
      }

      if (runPassenger2Dialog) {
        GeneralDialogStart(1);
        MoveCameraRightToDialog();
        MovePlayer(playerID, VI.Transform.Position.GetX(P2ColliderBox), 
                                                 VI.Transform.Position.GetY(P2ColliderBox));
        runPassenger2Dialog = RunDialog(P1ID, G1ID, PP1ID, PP2ID, "Dialog Argus");
        if (!runPassenger2Dialog)
          EndPassenger2Dialog();
      }

      bool dialogEnded = false;

      switch (passengerDialogProgress) {
      case 10: // Passenger 1 wrong
        GeneralDialogStart(7);
        MoveCameraRightToDialog();
        MovePlayer(playerID, VI.Transform.Position.GetX(wrongDestination_RenderLocation),
                                                 VI.Transform.Position.GetY(wrongDestination_RenderLocation));
        VI.Entity.Deactivate(wrongDestination_RenderLocation);
        if (!VI.Entity.IsActive(P2ColliderBox)) {
          VI.Entity.Activate(VI.Entity.GetId(P2String));
          VI.Entity.Activate(P2ColliderBox);
        }
        dialogEnded = RunDialog(P1ID, G1ID, PP1ID, PP2ID, "Dialog Minerva (Lighthouse)");
        if (!dialogEnded) EndDropoffDialog(0, false);
        break;
      case 11: // Passenger 1 correct
        GeneralDialogStart(5);
        MoveCameraRightToDialog();
        MovePlayer(playerID, VI.Transform.Position.GetX(correctDestination_RenderLocation),
                                                 VI.Transform.Position.GetY(correctDestination_RenderLocation));
        VI.Entity.Deactivate(correctDestination_RenderLocation);
        if (!VI.Entity.IsActive(P2ColliderBox)) {
          VI.Entity.Activate(VI.Entity.GetId(P2String));
          VI.Entity.Activate(P2ColliderBox);
        }
        dialogEnded = RunDialog(P1ID, G1ID, PP1ID, PP2ID, "Dialog Minerva (Multistory House)");
        MemoryFragmentExpand.ActivateFragment(0);
        if (!dialogEnded) EndDropoffDialog(0, true);
        break;
      case 20: // Passenger 2 wrong
        GeneralDialogStart(PlayerDirection.SE);
        MoveCameraLeftToDialog();
        MovePlayer(playerID, VI.Transform.Position.GetX(wrongDestination_RenderLocation),
                                                 VI.Transform.Position.GetY(wrongDestination_RenderLocation));
        VI.Entity.Deactivate(wrongDestination_RenderLocation);
        allPassengerDelivered = true;
        dialogEnded = RunDialog(P1ID, G1ID, PP1ID, PP2ID, "Dialog Argus (Water Fountain)");
        if (!dialogEnded) EndDropoffDialog(1, false);
        break;
      case 21: // Passenger 2 correct
        GeneralDialogStart(PlayerDirection.SW);
        MoveCameraRightToDialog();
        MovePlayer(playerID, VI.Transform.Position.GetX(correctDestination_RenderLocation),
                                                 VI.Transform.Position.GetY(correctDestination_RenderLocation));
        VI.Entity.Deactivate(correctDestination_RenderLocation);
        dialogEnded = RunDialog(P1ID, G1ID, PP1ID, PP2ID, "Dialog Argus (Brown House)");
        MemoryFragmentExpand.ActivateFragment(1);
        if (!dialogEnded) EndDropoffDialog(1, true);
        break;
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

      runPassengerDialog = false;
      runPassenger2Dialog = false;
    }

    public override void Dead(int _ENTITY) {
        base.Dead(_ENTITY);
    }

    #region Dialog Endings

    public void EndPassengerDialog() {
      GeneralEndDialog();

      correctDestination_RenderLocation = VI.Entity.GetId(P1CorrectBoxString);
      wrongDestination_RenderLocation = VI.Entity.GetId(P1WrongBoxString);

      // AllowAdvance = true; // TODO to update that player has talked to passenger already
      // dialogueOrder = 2;
      
      // Activate drop off point entity based on the dialog chosen
      if (base.latestChoiceChosen == 1) VI.Entity.Activate(correctDestination_RenderLocation);
      else if (base.latestChoiceChosen == 2) VI.Entity.Activate(wrongDestination_RenderLocation);

      UpdateObjective("Dialog Objective Passenger1 (Minerva)");
    }

    public void EndPassenger2Dialog() {
      GeneralEndDialog();

      correctDestination_RenderLocation = VI.Entity.GetId(P2CorrectBoxString);
      wrongDestination_RenderLocation = VI.Entity.GetId(P2WrongBoxString);
      // AllowAdvance = true; // TODO to update that player has talked to passenger already
      // dialogueOrder = 2;

      // Activate drop off point entity based on the dialog chosen
      if (base.latestChoiceChosen == 1) VI.Entity.Activate(correctDestination_RenderLocation);
      else if (base.latestChoiceChosen == 2) VI.Entity.Activate(wrongDestination_RenderLocation);

      UpdateObjective("Dialog Objective Passenger2 (Argus)");
    }

    #endregion

  }
}
