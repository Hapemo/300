/*!*****************************************************************************
\file DialogManager.cs
\author Jazz Teoh Yu Jue
\par DP email: j.teoh@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
The base script for dialog manager in any gamestate
*******************************************************************************/
using System;
using System.Dynamic;
using System.Runtime.CompilerServices;
using VI;
using static VI.Dialogue;

// INFO
// Intro Dialog zoom is x = 1200
// Static Dialog zoom is x = 960
// Normal zoom is x = 1600

namespace BonVoyage {
  public class DialogManager : BaseScript {
    protected const float textHeight = 50.0f;
    protected const float dialogBoxWidth = 549.0f;
    protected const float choiceWidth = 564.6f;
    protected const float smallDialogBoxHeight = 109.8f;
    protected const float midDialogBoxHeight = 178.8f;
    protected const float bigDialogBoxHeight = 228.6f;
    protected const float verybigDialogBoxHeight = 300.6f;
    protected const float smallChoiceHeight = 61.8f;
    protected const float midChoiceHeight = 87.6f;

    protected bool choiceFlag;            // This flag is true during choice selection dialogs
    protected bool updateChat;            // This flag is true when dialog changes for anyone
    protected bool dialogInit;            // THis flag is true when entering a dialog for the first line
    protected bool normalZoom = true;
    protected int latestChoiceChosen = 0; // 0 if no choice chosen, 1 if choice 1 chosen, 2 if choice 2 chosen. Resets every frame
    static public int passengerDialogProgress = 0; // Consists of 2 numbers in this format <passenger number><right or wrong>. 1 for right, 0 for wrong. eg passenger 1 right answer destination reached, it will be 11.
    static public bool allPassengerDelivered = false;

    protected int playerID;
    protected int UIObjectiveTextID;

    protected int UIHPBarID;
    protected int UIMemoryFragmentID;
    protected int UIMemoryFragmentScreenID;

    protected int UIPassengersLeftID;
    protected int UIPassengerIcon1ID;
    protected int UIPassengerIcon2ID;


    protected int P1ID;
    protected int PP1ID;
    protected int PP2ID;
    protected int G1ID;

        protected int dpopup;
        protected int dadvance;
        protected int dchoice;
    public virtual void Alive(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
    }

    public virtual void Init(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
      UIObjectiveTextID = VI.Entity.GetId("objectivetext");
      P1ID = VI.Entity.GetId("P1");
      PP1ID = VI.Entity.GetId("PP1");
      PP2ID = VI.Entity.GetId("PP2");
      G1ID = VI.Entity.GetId("G1");

      AlignNonPlayerText(G1ID);
      AlignPlayerText(P1ID);
      TextBoxAlign(PP1ID, 400, -280, 80, 15, 1);
      TextBoxAlign(PP2ID, 400, -280, 80, 15, 2);

      playerID = VI.Entity.GetId("Boat", VI.GameState.GetName());

      UIHPBarID = VI.Entity.GetId("hpbar");
      UIMemoryFragmentID = VI.Entity.GetId("memoryfragment");
      UIMemoryFragmentScreenID = VI.Entity.GetId("memoryfragmentscreen");
      allPassengerDelivered = false;
            dpopup = VI.Entity.GetId("Dpopup");
            dadvance = VI.Entity.GetId("Dadvance");
            dchoice = VI.Entity.GetId("Dchoise");
        }

    public virtual void EarlyUpdate(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
    }

    public virtual void Update(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
      // Things that should happen when passenger is delivered
    }

    public virtual void FixedUpdate(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
    }

    public virtual void LateUpdate(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
    }

    public virtual void Exit(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
      passengerDialogProgress = 0;
    }

    public virtual void Dead(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
    }


    #region UIControl
    public void DisableUI() {
      VI.Entity.Deactivate(UIHPBarID);
      //VI.Entity.Deactivate(UIMiniMapID);
      //VI.Entity.Deactivate(UIEnemyMapID);
      //VI.Entity.Deactivate(UIWeatherMapID);

      VI.Scene.Pause("GUI Scene");
            VI.Scene.Pause("TutorialUI");
            VI.Scene.Pause("TutorialUILvl1");
            VI.Scene.Pause("TutorialUILvl2");
            VI.Scene.Pause("TutorialNameFade");
            VI.Scene.Pause("L1NameFade");
            VI.Scene.Pause("L2NameFade");
            VI.Scene.Pause("L3NameFade");



            VI.Entity.s_Deactivate("toggleMap");
      //VI.Scene.Pause("CrystalBalls");
      //VI.Scene.Pause("MiniMap");
      //VI.Scene.Pause("EnemyMap");
      //VI.Scene.Pause("WeatherMap");
    }

    public void EnableUI() {  
      if (UIHPBarID != 0)
        VI.Entity.Activate(UIHPBarID);

      VI.Scene.Play("GUI Scene");
            VI.Scene.Play("TutorialUI");
            VI.Scene.Play("TutorialUILvl1");
            VI.Scene.Play("TutorialUILvl2");
            VI.Scene.Play("TutorialNameFade");
            VI.Scene.Play("L1NameFade");
            VI.Scene.Play("L2NameFade");
            VI.Scene.Play("L3NameFade");
            VI.Entity.s_Activate("toggleMap");
      //VI.Scene.Play("CrystalBalls");
      //VI.Scene.Play("MiniMap");
      //VI.Scene.Play("EnemyMap");
      //VI.Scene.Play("WeatherMap");
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
    public void TextBoxAlign(int entityname, float posX, float posY, float textXSpacing = 50, float textYSpacing = 50, int choice = 0, float spacing = 15) {
      int additionalLines = VI.Text.GetLineCount(entityname) - 1;

      // Selecting the dialog box texture and putting it into correct position
      if (choice == 0) {
        if (additionalLines < 2) {
          VI.Transform.Scale.SetY(entityname, smallDialogBoxHeight);
          VI.Transform.Scale.SetX(entityname, dialogBoxWidth);
          VI.Sprite.SetTexture(entityname, "Textures\\Icons\\dialogue\\UI_DialogueBox1.png");
        } else if (additionalLines < 4) {
          posY += (midDialogBoxHeight - smallDialogBoxHeight) / 2;
          VI.Transform.Scale.SetY(entityname, midDialogBoxHeight);
          VI.Transform.Scale.SetX(entityname, dialogBoxWidth);
          VI.Sprite.SetTexture(entityname, "Textures\\Icons\\dialogue\\UI_DialogueBox2.png");
        } else if (additionalLines < 6) {
          posY += (bigDialogBoxHeight - smallDialogBoxHeight) / 2;
          VI.Transform.Scale.SetY(entityname, bigDialogBoxHeight);
          VI.Transform.Scale.SetX(entityname, dialogBoxWidth);
          VI.Sprite.SetTexture(entityname, "Textures\\Icons\\dialogue\\UI_DialogueBox3.png");
        } else {
          posY += (verybigDialogBoxHeight - smallDialogBoxHeight) / 2;
          VI.Transform.Scale.SetY(entityname, verybigDialogBoxHeight);
          VI.Transform.Scale.SetX(entityname, dialogBoxWidth);
          VI.Sprite.SetTexture(entityname, "Textures\\Icons\\dialogue\\UI_DialogueBox3.png");
        }
      } else {
        if (additionalLines == 0) {
          textYSpacing = 10;
          VI.Transform.Scale.SetY(entityname, smallChoiceHeight);
          VI.Transform.Scale.SetX(entityname, choiceWidth);
          VI.Sprite.SetTexture(entityname, "Textures\\Icons\\dialogue\\UI_DialogueOption1.png");
        } else {
          VI.Transform.Scale.SetY(entityname, midChoiceHeight);
          VI.Transform.Scale.SetX(entityname, choiceWidth);
          VI.Sprite.SetTexture(entityname, "Textures\\Icons\\dialogue\\UI_DialogueOption2.png");
        }
      }

      // If it's a choice textbox, posY is the middle of both texts
      if (choice == 1)
        posY = posY + spacing + VI.Transform.Scale.GetY(entityname) / 2;
      else if (choice == 2)
        posY = posY - spacing - VI.Transform.Scale.GetY(entityname) / 2;

      VI.Transform.Position.SetX(entityname, posX);
      VI.Transform.Position.SetY(entityname, posY);

      float magicNumber = 10.0f * VI.Text.Scale.Get(entityname);
      VI.Text.Offset.Set(entityname, -VI.Transform.Scale.GetX(entityname) / 2 + textXSpacing, VI.Transform.Scale.GetY(entityname) / 2 - textHeight + textYSpacing);
    }

    // Based on the current dialog ID, move to the next one. Can input choice if there is a choice selection, by default it's 1
    public void MoveToNextDialog(int choice = 1) {
      //VI.Test.ArgString("Current ID: " + Dialogue.Current.GetId());
      //VI.Test.ArgString("Next ID: " + Dialogue.Next.GetId());

      if (choice == 1)
        VI.Dialogue.Current.SetTo(VI.Dialogue.GetNextId(VI.Dialogue.Current.GetId()));
      else
        VI.Dialogue.Current.SetTo(VI.Dialogue.Choice.Second(VI.Dialogue.Current.GetId()));
    }

    // Get the texts of the next dialog, able to input 1 or 2 to get the different choices
    public string GetNextDialog(int choice = 1) {
      int ID = 0;
      if (choice == 1) {
        ID = VI.Dialogue.GetNextId(VI.Dialogue.Current.GetId());
        //Console.WriteLine("Choice 1 is: " + ID);
        //VI.Test.ArgString("Choice 1 is: " + ID);
      } else {
        ID = VI.Dialogue.Choice.Second(VI.Dialogue.Current.GetId());
        //Console.WriteLine("Choice 2 is: " + ID);
        //VI.Test.ArgString("Choice 2 is: " + ID);

      }
      //Console.WriteLine("Resultant line is: " + VI.Dialogue.GetLine(ID));
      //VI.Test.ArgString("Resultant line is: " + VI.Dialogue.GetLine(ID));
      return VI.Dialogue.GetLine(ID);
    }

    public void DeactivateDialogBox(int player, int notPlayer, int choice1, int choice2) {
      VI.Entity.Deactivate(player);
      VI.Entity.Deactivate(notPlayer);
      VI.Entity.Deactivate(choice1);
      VI.Entity.Deactivate(choice2);
    }

    public bool MouseClick(int entity) {
      return (VI.Input.Mouse.Press() && VI.Input.Button.Hover(entity));
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
    public bool RunDialog(int player, int notPlayer, int choice1, int choice2, string dialogFile) {
      if (dialogInit) {
        DisableUI();
        VI.Audio.PlayNow(dpopup);
        // Load Little Girl Talking
        VI.Dialogue.LoadScript(dialogFile);
        VI.Dialogue.Current.SetTo(1);

        // Setting default P1, PP1, PP2 positions
        VI.Transform.Position.SetX(choice1, 500);
        VI.Transform.Position.SetY(choice1, 46);
        VI.Transform.Position.SetX(choice2, 500);
        VI.Transform.Position.SetY(choice2, -90);
        VI.Transform.Position.SetX(player, 450);
        VI.Transform.Position.SetY(player, 5);

        // Activate little girl dialogue
        int firstSpeaker;
        if (VI.Dialogue.Speaker.IsPlayer(1))
          firstSpeaker = player;
        else
          firstSpeaker = notPlayer;

        VI.Entity.Activate(firstSpeaker);
        VI.Text.Update(firstSpeaker, VI.Dialogue.GetLine(VI.Dialogue.Current.GetId()));

        if (VI.Dialogue.Speaker.IsPlayer(1))
          AlignPlayerText(player);
        else
          AlignNonPlayerText(notPlayer);

        //camZoomingIn = true;
        dialogInit = false;
      }

      // Button click set flags
      if (choiceFlag) {
        if (MouseClick(choice1) || MouseClick(choice2))
                {
          updateChat = true;
                        VI.Audio.PlayNow(dchoice);
                }
      } else if (VI.Input.Mouse.Press())//(VI.Input.Button.s_Released(player, scene) || VI.Input.Button.s_Released(notPlayer, scene))
            {
                if (VI.Dialogue.Speaker.IsPlayer(VI.Dialogue.Current.GetId()))
                    VI.Audio.PlayNow(dchoice);
                else
                    VI.Audio.PlayNow(dadvance);

                updateChat = true;
            }

      // Logic done using those flags
      if (updateChat) {
        updateChat = false;

        //LOG.WRITE("CurrentID before check quit: " + VI.Dialogue.Current.GetId());
        //LOG.WRITE("NextID before check quit: " + VI.Dialogue.GetNextId(VI.Dialogue.Current.GetId()));
        // Finish dialog
        if (VI.Dialogue.GetNextId(VI.Dialogue.Current.GetId()) == 0) {
          DeactivateDialogBox(player, notPlayer, choice1, choice2);
          dialogInit = true;
          //camZoomingOut = true;
          EnableUI();
          //Console.WriteLine("finished dialog");
          //LOG.WRITE("finished dialog");
          return false;
        }

        //Console.WriteLine("Moving on from: " + VI.Dialogue.Current.GetId());
        //LOG.WRITE("Moving on from: " + VI.Dialogue.Current.GetId());
        if (choiceFlag) {
          //Console.WriteLine("It's a choice dialog");
          //LOG.WRITE("It's a choice dialog");
          choiceFlag = false;
          if (MouseClick(choice2)) {
            MoveToNextDialog(2);
            latestChoiceChosen = 2;
            LOG.WRITE("latestChoiceChosen = 2");
            //Console.WriteLine("Choice 2 selected, moving to: " + VI.Dialogue.Current.GetId());
            //LOG.WRITE("Choice 2 selected, moving to: " + VI.Dialogue.Current.GetId());
          } else {
            MoveToNextDialog(1);
            latestChoiceChosen = 1;
            LOG.WRITE("latestChoiceChosen = 1");
            //Console.WriteLine("Choice 1 selected, moving to: " + VI.Dialogue.Current.GetId());
            //LOG.WRITE("Choice 1 selected, moving to: " + VI.Dialogue.Current.GetId());
          }
          VI.Entity.Deactivate(choice1);
          VI.Entity.Deactivate(choice2);
        }
        MoveToNextDialog(1);
        //Console.WriteLine("Moving to: " + VI.Dialogue.Current.GetId());
        //LOG.WRITE("Moving to: " + VI.Dialogue.Current.GetId());

        if (VI.Dialogue.Speaker.IsPlayer(VI.Dialogue.Current.GetId())) {
          VI.Entity.Activate(player);
          VI.Entity.Deactivate(notPlayer);
          VI.Text.Update(player, VI.Dialogue.GetLine(VI.Dialogue.Current.GetId()));
          AlignPlayerText(player);
        } else {
          VI.Entity.Activate(notPlayer);
          VI.Entity.Deactivate(player);
          VI.Text.Update(notPlayer, VI.Dialogue.GetLine(VI.Dialogue.Current.GetId()));
          AlignNonPlayerText(notPlayer);
        }

        if (VI.Dialogue.Choice.Second(VI.Dialogue.Current.GetId()) != 0) {
          //Console.WriteLine("This dialog is a choice dialog: " + VI.Dialogue.Current.GetId());
          //LOG.WRITE("This dialog is a choice dialog: " + VI.Dialogue.Current.GetId());
          VI.Entity.Activate(choice1);
          VI.Entity.Activate(choice2);
          VI.Text.Update(choice1, GetNextDialog(1));
          VI.Text.Update(choice2, GetNextDialog(2));
          TextBoxAlign(choice1, 400, -280, 80, 15, 1);
          TextBoxAlign(choice2, 400, -280, 80, 15, 2);
          choiceFlag = true;
        }
      }
      return true;
    }

    #endregion

    #region minorHelpers
    public void ZoomCameraToDialog() { ChangeZoom(960, 540); }
    void AlignPlayerText(int player) { TextBoxAlign(player, 450, 5, 20, 0); }
    void AlignNonPlayerText(int nonplayer) { TextBoxAlign(nonplayer, 0, 20, 20, 0); }

    #endregion

    // The General function for stand still dialogs. 1-SW, 2-W, 3-NW, 4-N, 5-NE, 7-SE
    public enum PlayerDirection {
      S=0,
      SW,
      W,
      NW,
      N,
      NE,
      E,
      SE
    }

    public void GeneralDialogStart(PlayerDirection direction) {
      GeneralDialogStart((int)direction);
    }
    public void GeneralDialogStart(int direction) { // 1 to face left, 
      PlayerScript.CameraFollowPlayer = false;
      PlayerScript.PlayerInDialogue = true;
      VI.Entity.Deactivate(UIObjectiveTextID);
      ZoomCameraToDialog();
      VI.Animation.SpriteSheet.SheetIndex.Set(playerID, direction); // Make player face the other person
    }

    #region Dialog Endings

    public void UpdateObjective(string objectiveFile) {
      VI.Dialogue.LoadScript(objectiveFile);
      if (latestChoiceChosen == 1)
        VI.Text.Update(UIObjectiveTextID, VI.Dialogue.GetLine(1));
      else if (latestChoiceChosen == 2)
        VI.Text.Update(UIObjectiveTextID, VI.Dialogue.GetLine(2));
      latestChoiceChosen = 0;
    }

    public void GeneralEndDialog() {
      PlayerScript.CameraFollowPlayer = true;
      //runPassenger2Dialog = false;
      VI.Entity.Activate(UIObjectiveTextID);
      normalZoom = false;
      PlayerScript.PlayerInDialogue = false;
    }

    public void EndDropoffDialog(int fragmentId, bool correct) {
      GeneralEndDialog();
      VI.Text.Update(UIObjectiveTextID, "Continue Exploring...");
      passengerDialogProgress = 0;
      PassengerDeliverUI.DeliveredPassenger();
            MemoryFragmentExpand.DeliveredPassenger(fragmentId, correct);
    }

    //public void EndGirlDialog() {
    //  runGirlDialog = false;
    //  PlayerScript.PlayerInDialogue = false;

    //  VI.Entity.s_Deactivate("LittleGirlBox", "Level1"); //Todo change to girl's ID or public static it's vvariable
    //  //ObjectiveTextScript.UpdateText("Finished talking to little girl"); // TODO Christy to update the text needed here
    //  //EndGirlExistance(); // TODO. after talking to girl, make her dissappear.
    //}
    public void MoveCameraRightToDialog() {
      float playerHeight = VI.Transform.Scale.GetY(playerID);
      float playerWidth = VI.Transform.Scale.GetX(playerID);
      float screenHalfHeight = VI.Camera.GetScale.Y()/2;

      MoveCamera(VI.Transform.Position.GetX(playerID) + playerWidth / 3,
                                     VI.Transform.Position.GetY(playerID) + screenHalfHeight - playerHeight / 2);
    }

    public void MoveCameraLeftToDialog() {
      float playerHeight = VI.Transform.Scale.GetY(playerID);
      float playerWidth = VI.Transform.Scale.GetX(playerID);
      float screenHalfHeight = VI.Camera.GetScale.Y()/2;

      MoveCamera(VI.Transform.Position.GetX(playerID) - playerWidth / 3,
                                     VI.Transform.Position.GetY(playerID) + screenHalfHeight - playerHeight / 2);
    }

    #endregion

  }
}
