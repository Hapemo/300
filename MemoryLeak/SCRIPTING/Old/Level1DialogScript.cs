using System;
using System.Runtime.CompilerServices;
using VI;
using static VI.Dialogue;

namespace BonVoyage {
  public class Level1DialogScript : BaseScript {
    static public bool runIntroDialog;
    static public bool runGirlDialog;
    static public bool runPassengerDialog;
    private bool choiceFlag;            // This flag is true during choice selection dialogs
    private bool updateChat;            // This flag is true when dialog changes for anyone
    private bool dialogInit;            // THis flag is true when entering a dialog for the first line

    private int playerID;

    private int UIHPBarID;
    private int UIMemoryFragmentID;
    private int UIMemoryFragmentScreenID;
    private int UIFragment1ObjID;
    private int UICycleMapID;
    private int UIMiniMapID;
    private int UIEnemyMapID;
    private int UIWeatherMapID;
    private int UIWeatherTextID;
    private int UIObjectiveTextID;

    private int P1ID;
    private int PP1ID;
    private int PP2ID;
    private int G1ID;
    public void Alive(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
    }

    public void Init(int _ENTITY) {
      playerID = VI.Entity.GetId("Boat", VI.GameState.GetName());

      UIHPBarID = VI.Entity.GetId("hpbar", "Dialogue");
      UIMemoryFragmentID = VI.Entity.GetId("memoryfragment", "Dialogue");
      UIMemoryFragmentScreenID = VI.Entity.GetId("memoryfragmentscreen", "Dialogue");
      UIFragment1ObjID = VI.Entity.GetId("fragment1obj", "Dialogue");
      UICycleMapID = VI.Entity.GetId("cyclemap", "Dialogue");
      UIMiniMapID = VI.Entity.GetId("minimap", "Dialogue");
      UIEnemyMapID = VI.Entity.GetId("enemymap", "Dialogue");
      UIWeatherMapID = VI.Entity.GetId("weathermap", "Dialogue");
      UIWeatherTextID = VI.Entity.GetId("weathertext", "Dialogue");
      UIObjectiveTextID = VI.Entity.GetId("objectivetext", "Dialogue");

      P1ID = VI.Entity.GetId("P1", "Dialogue");
      PP1ID = VI.Entity.GetId("PP1", "Dialogue");
      PP2ID = VI.Entity.GetId("PP2", "Dialogue");
      G1ID = VI.Entity.GetId("G1", "Dialogue");

      dialogInit = true;
    }

    public void Update(int _ENTITY) {
      // Dialog control
      PlayerScript.PlayerInDialogue = (runIntroDialog || runGirlDialog || runPassengerDialog);
      if (!PlayerScript.PlayerInDialogue) return;

      if (runIntroDialog) {
        runIntroDialog = RunDialog("P1", "G1", "PP1", "PP2", "Dialogue", "Dialogue SceneIntro 1");
        if (!runIntroDialog) EndIntroDialog();
      }

      if (runGirlDialog) {
        VI.Animation.SheetIndex.Set(playerID, 1);
        runGirlDialog = RunDialog("P1", "G1", "PP1", "PP2", "Dialogue", "Dialogue LittleGirl 0");
        if (!runGirlDialog) EndGirlDialog();
      }

      if (runPassengerDialog) {
        VI.Animation.SheetIndex.Set(playerID, 1);
        runPassengerDialog = RunDialog("P1", "G1", "PP1", "PP2", "Dialogue", "Dialogue Passenger 1");
        if (!runPassengerDialog) EndPassengerDialog();
      }

    }

    public void FixedUpdate(int _ENTITY) {

    }
    public void Exit(int _ENTITY) {

    }
    public void Dead(int _ENTITY) {

    }


    #region UIControl
    public void DisableUI() {
      VI.Entity.Deactivate(UIHPBarID);
      VI.Entity.Deactivate(UIMemoryFragmentID);
      VI.Entity.Deactivate(UIMemoryFragmentScreenID);
      VI.Entity.Deactivate(UIFragment1ObjID);
      VI.Entity.Deactivate(UICycleMapID);
      VI.Entity.Deactivate(UIMiniMapID);
      VI.Entity.Deactivate(UIEnemyMapID);
      VI.Entity.Deactivate(UIWeatherMapID);
      VI.Entity.Deactivate(UIWeatherTextID);
      VI.Entity.Deactivate(UIObjectiveTextID);
    }

    public void EnableUI() {
      VI.Entity.Activate(UIHPBarID);
      VI.Entity.Activate(UIMemoryFragmentID);
      VI.Entity.Activate(UICycleMapID);
      VI.Entity.Activate(UIObjectiveTextID);
    }
    #endregion

    // All the functions on the top of DialogHelper region are helper functions to assist RunDialog() at the bottom of the region
    #region DialogHelper
    // int scaleX           - This is the default width of button
    // int posX             - This is the center X coordinate of button (500)
    // int posY             - This is the center Y coordinate of button. If it's choice, it's the middle of both buttons (-25)
    // int spaceing         - Spacing between choice button edge from middle, useless for single button.
    // int perLineScaleY    - This is the increment for one additional line
    // int textXSpacing     - This is the spacing of the text from the left edge of the box 
    // int textYSpacing     - This is the spacing of the text from the top edge of the box 
    public void TextBoxAlign(string entityname, string scenename, float posX, float posY, float scaleX = 500, float perLineScaleY = 51, float textXSpacing = 50, float textYSpacing = 50, int choice = 0, float spacing = 15) {
      int additionalLines = VI.Text.s_GetLineCount(entityname, scenename) - 1;
      //Console.WriteLine("lines: " + additionalLines);
      //float scaleY = perLineScaleY;         // This is the default height of button, will changing with respect to line count
      perLineScaleY *= VI.Text.Scale.s_Get(entityname, scenename);

      VI.Transform.Scale.s_SetX(entityname, scenename, scaleX);
      VI.Transform.Scale.s_SetY(entityname, scenename, perLineScaleY * (1 + additionalLines) + textYSpacing * 2);

      // If it's a choice textbox, posY is the middle of both texts
      if (choice == 1) posY = posY + spacing + perLineScaleY * (additionalLines + 1);
      else if (choice == 2) posY = posY - spacing - perLineScaleY * (additionalLines + 1);

      float finalPosY = posY - (perLineScaleY * additionalLines) / 2;

      VI.Transform.Position.s_SetX(entityname, scenename, posX);
      VI.Transform.Position.s_SetY(entityname, scenename, finalPosY);

      float textY = perLineScaleY * additionalLines / 2 - perLineScaleY / 2;
      float magicNumber = 10.0f * VI.Text.Scale.s_Get(entityname, scenename);
      VI.Text.Offset.s_Set(entityname, scenename, -scaleX / 2 + textXSpacing, textY + magicNumber);
    }
    // Based on the current dialog ID, move to the next one. Can input choice if there is a choice selection, by default it's 1
    public void MoveToNextDialog(int choice = 1) {
      VI.Test.ArgString("Current ID: " + Dialogue.Current.GetId());
      VI.Test.ArgString("Next ID: " + Dialogue.Next.GetId());

      if (choice == 1) VI.Dialogue.Current.SetTo(VI.Dialogue.GetNextId(VI.Dialogue.Current.GetId()));
      else VI.Dialogue.Current.SetTo(VI.Dialogue.Choice.Second(VI.Dialogue.Current.GetId()));
    }

    // Get the texts of the next dialog, able to input 1 or 2 to get the different choices
    public string GetNextDialog(int choice = 1) {
      int ID = 0;
      if (choice == 1) {
        ID = VI.Dialogue.GetNextId(VI.Dialogue.Current.GetId());
        //Console.WriteLine("Choice 1 is: " + ID);
        VI.Test.ArgString("Choice 1 is: " + ID);
      } else {
        ID = VI.Dialogue.Choice.Second(VI.Dialogue.Current.GetId());
        //Console.WriteLine("Choice 2 is: " + ID);
        VI.Test.ArgString("Choice 2 is: " + ID);
        
      }
      //Console.WriteLine("Resultant line is: " + VI.Dialogue.GetLine(ID));
      VI.Test.ArgString("Resultant line is: " + VI.Dialogue.GetLine(ID));
      return VI.Dialogue.GetLine(ID);
    }

    public void DeactivateDialogBox(string player, string notPlayer, string choice1, string choice2, string scene) {
      VI.Entity.s_Deactivate(player, scene);
      VI.Entity.s_Deactivate(notPlayer, scene);
      VI.Entity.s_Deactivate(choice1, scene);
      VI.Entity.s_Deactivate(choice2, scene);
    }



    /* For carrying on the dialog conversation logic. It will automatically zoom in and out, disabling and enabling UI too.
         * player - The chatbox entity of the player
         * notPlayer - The chatbox entity of the entity the player is talking to
         * choice1 - The chatbox entity of the first choice dialog
         * choice2 - The chatbox entity of the second choice dialog
         * scene - The scene of all the chatbox entities (All must be the same scene)
         * dialogFile - Dialog file name
         * return bool - True if dialog is still running. False if dialog has ended.
        */
    public bool RunDialog(string player, string notPlayer, string choice1, string choice2, string scene, string dialogFile) {
      if (dialogInit) {
        DisableUI();
        // Load Little Girl Talking
        VI.Dialogue.LoadScript(dialogFile);
        VI.Dialogue.Current.SetTo(1);

        // Setting default P1, PP1, PP2 positions
        VI.Transform.Position.SetX(PP1ID, 500);
        VI.Transform.Position.s_SetY(choice1, scene, 46);
        VI.Transform.Position.s_SetX(choice2, scene, 500);
        VI.Transform.Position.s_SetY(choice2, scene, -90);
        VI.Transform.Position.s_SetX(player, scene, 450);
        VI.Transform.Position.s_SetY(player, scene, 5);

        // Activate little girl dialogue
        string firstSpeaker;
        if (VI.Dialogue.Speaker.IsPlayer(1)) firstSpeaker = player;
        else firstSpeaker = notPlayer;

        VI.Entity.s_Activate(firstSpeaker, scene);
        VI.Text.s_Update(firstSpeaker, scene, VI.Dialogue.GetLine(VI.Dialogue.Current.GetId()));

        if (VI.Dialogue.Speaker.IsPlayer(1)) TextBoxAlign(player, scene, 450, 5, 500, 51, 20, 20);
        else TextBoxAlign(notPlayer, scene, -387, 330, 740, 51, 20, 20);

        //camZoomingIn = true;
        dialogInit = false;
      }

      // Button click set flags
      if (choiceFlag) {
        if (VI.Input.Button.s_Released(choice1, scene) || VI.Input.Button.s_Released(choice2, scene)) updateChat = true;
      } else if (VI.Input.Button.s_Released(player, scene) || VI.Input.Button.s_Released(notPlayer, scene)) updateChat = true;

      // Logic done using those flags
      if (updateChat) {
        updateChat = false;

        VI.Test.ArgString("CurrentID before check quit: " + VI.Dialogue.Current.GetId());
        VI.Test.ArgString("NextID before check quit: " + VI.Dialogue.GetNextId(VI.Dialogue.Current.GetId()));
        // Finish dialog
        if (VI.Dialogue.GetNextId(VI.Dialogue.Current.GetId()) == 0) {
          DeactivateDialogBox(player, notPlayer, choice1, choice2, scene);
          dialogInit = true;
          //camZoomingOut = true;
          EnableUI();
          //Console.WriteLine("finished dialog");
          VI.Test.ArgString("finished dialog");
          return false;
        }

        //Console.WriteLine("Moving on from: " + VI.Dialogue.Current.GetId());
        VI.Test.ArgString("Moving on from: " + VI.Dialogue.Current.GetId());
        if (choiceFlag) {
          //Console.WriteLine("It's a choice dialog");
          VI.Test.ArgString("It's a choice dialog");
          choiceFlag = false;
          if (VI.Input.Button.s_Released(choice2, scene)) {
            MoveToNextDialog(2);
            //Console.WriteLine("Choice 2 selected, moving to: " + VI.Dialogue.Current.GetId());
            VI.Test.ArgString("Choice 2 selected, moving to: " + VI.Dialogue.Current.GetId());
          } else {
            MoveToNextDialog(1);
            //Console.WriteLine("Choice 1 selected, moving to: " + VI.Dialogue.Current.GetId());
            VI.Test.ArgString("Choice 1 selected, moving to: " + VI.Dialogue.Current.GetId());
          }
          VI.Entity.s_Deactivate(choice1, scene);
          VI.Entity.s_Deactivate(choice2, scene);
        }
        MoveToNextDialog(1);
        //Console.WriteLine("Moving to: " + VI.Dialogue.Current.GetId());
        VI.Test.ArgString("Moving to: " + VI.Dialogue.Current.GetId());

        if (VI.Dialogue.Speaker.IsPlayer(VI.Dialogue.Current.GetId())) {
          VI.Entity.s_Activate(player, scene);
          VI.Entity.s_Deactivate(notPlayer, scene);
          VI.Text.s_Update(player, scene, VI.Dialogue.GetLine(VI.Dialogue.Current.GetId()));
          TextBoxAlign(player, scene, 450, 5, 500, 51, 20, 20);
        } else {
          VI.Entity.s_Activate(notPlayer, scene);
          VI.Entity.s_Deactivate(player, scene);
          VI.Text.s_Update(notPlayer, scene, VI.Dialogue.GetLine(VI.Dialogue.Current.GetId()));
          TextBoxAlign(notPlayer, scene, -387, 330, 740, 51, 20, 20);
        }

        if (VI.Dialogue.Choice.Second(VI.Dialogue.Current.GetId()) != 0) {
          //Console.WriteLine("This dialog is a choice dialog: " + VI.Dialogue.Current.GetId());
          VI.Test.ArgString("This dialog is a choice dialog: " + VI.Dialogue.Current.GetId());
          VI.Entity.s_Activate(choice1, scene);
          VI.Entity.s_Activate(choice2, scene);
          VI.Text.s_Update(choice1, scene, GetNextDialog(1));
          VI.Text.s_Update(choice2, scene, GetNextDialog(2));
          TextBoxAlign(choice1, scene, 500, -25, 500, 51, 40, 25, 1);
          TextBoxAlign(choice2, scene, 500, -25, 500, 51, 40, 25, 2);
          choiceFlag = true;
        }
      }
      return true;
    }

    #endregion


    #region Dialog Endings
    public void EndIntroDialog() {
      runIntroDialog = false;
      PlayerScript.PlayerInDialogue = false;

      VI.Text.Update(UIObjectiveTextID, "Objective: Find the Little Girl");
    }

    public void EndGirlDialog() {
      runGirlDialog = false;
      PlayerScript.PlayerInDialogue = false;

      VI.Entity.s_Deactivate("LittleGirlBox", "Level1"); //Todo change to girl's ID or public static it's vvariable
      ObjectiveTextScript.UpdateText("Finished talking to little girl"); // TODO Christy to update the text needed here
      //EndGirlExistance(); // TODO. after talking to girl, make her dissappear.
    }

    public void EndPassengerDialog() {
      runPassengerDialog = false;
      PlayerScript.PlayerInDialogue = false;

      VI.Transform.Rotate.s_Set("Passenger_1", "Level1", 0.5f);
      ObjectiveTextScript.UpdateText("Finished talking to passenger"); // TODO Christy to update the text needed here
      // AllowAdvance = true; // TODO to update that player has talked to passenger already
      // dialogueOrder = 2;
    }

    #endregion



























































































  }
}
