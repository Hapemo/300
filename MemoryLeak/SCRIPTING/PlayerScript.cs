/*!*****************************************************************************
\file PlayerScript.cs
\author Lee Hsien Wei Joachim
\par DP email: l.hsienweijoachim@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
Script to control the movement and controls of the player, and everything related
to the player
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class PlayerScript : BaseScript
    {
        static private Random rand;

        private const int MaxHealth = 11;
        public const float PlayerSpeed = 10f;
        private const float SpeedCheatMultiplier = 2.5f;

        static public bool PlayerInDialogue = false;
        static public bool PlayerInOtherAnimation = false;
        static public bool PlayerInDeathAnimation = false;
        static public int PlayerHealth = 0;
        static public bool CameraFollowPlayer = true;

        private bool SpeedCheatToggle = false;

        static public float PlayerPosX = 0f;
        static public float PlayerPosY = 0f;
        //private float PlayerScaleX;
        private float PlayerScaleY = 0f;
        private float PlayerRotation = 0f;

        private int PlayerCurrWeather = 0;
        private double ChangeDirTimer = -1.0;
        private float MovementXModifier = 0f;
        private float MovementYModifier = 0f;

        private int toggleMap;
        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            rand = new Random();

            // Initialize bool variables
            PlayerInDialogue = false;
            PlayerInOtherAnimation = false;
            PlayerInDeathAnimation = false;
            PlayerHealth = 0;
            CameraFollowPlayer = true;

            SpeedCheatToggle = false;

            // Initialize position variables
            PlayerPosX = VI.Transform.Position.GetX(_ENTITY);
            PlayerPosY = VI.Transform.Position.GetY(_ENTITY);
            //PlayerScaleX = VI.Transform.Scale.GetX(_ENTITY);
            PlayerScaleY = VI.Transform.Scale.GetY(_ENTITY);
            PlayerRotation = 0;

            PlayerCurrWeather = 0;
            ChangeDirTimer = -1.0;
            MovementXModifier = 0f;
            MovementYModifier = 0f;

            toggleMap = VI.Entity.GetId("toggleMap");
        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            #region Player Death
            // Once health hits 0
            if (PlayerHealth > MaxHealth && !PlayerInDeathAnimation)
            {
                // Update bool flag
                PlayerInDeathAnimation = true;

                // Set animation
                SetPlayerSprite(_ENTITY, PlayerRotation, "Death");
                VI.Animation.SpriteSheet.CurrentFrame.Set(_ENTITY, 0);
                VI.Animation.SpriteSheet.FrameCount.Set(_ENTITY, 11);
            }

            // Check for completion of death animation
            if (PlayerInDeathAnimation && (VI.Animation.SpriteSheet.CurrentFrame.Get(_ENTITY) + 1 == VI.Animation.SpriteSheet.FrameCount.Get(_ENTITY)))
            {
                // Deactive itself to hand over control to GameOver script
                VI.Entity.Deactivate(_ENTITY);   
            }
            #endregion

            // Things allowed to run when player is not dead
            if (!PlayerInDeathAnimation)
            {
                // Pausing the game
                #region Pause Game
                // Escape key is pressed
                if (VI.Input.Key.Release(256))
                {
                    string gsname = VI.GameState.GetName();
                    if (gsname == "LevelTutorial")
                    {
                        VI.Scene.Pause(gsname);
                        VI.Scene.Pause(gsname + "Background");
                        //VI.Scene.Pause("WeatherScene");
                        VI.Scene.SetForceRender(gsname, true);
                        VI.Scene.SetForceRender(gsname + "Background", true);
                        //VI.Scene.SetForceRender("WeatherScene", true);

                        //VI.Scene.Pause("GUI Scene");
                        VI.Scene.Pause("TutorialUI");
                        VI.Scene.Pause("TutorialNameFade");

                        //VI.Entity.Deactivate(toggleMap);

                        VI.Scene.Play("Pause");
                    }
                    else
                    {
                        VI.Scene.Pause(gsname);
                        VI.Scene.Pause(gsname + "Background");
                        VI.Scene.Pause("WeatherScene");
                        VI.Scene.SetForceRender(gsname, true);
                        VI.Scene.SetForceRender(gsname + "Background", true);
                        VI.Scene.SetForceRender("WeatherScene", true);

                        VI.Scene.Pause("GUI Scene");
                        VI.Scene.Pause("TutorialUILvl" + gsname[gsname.Length - 1]);
                        VI.Scene.Pause("L" + gsname[gsname.Length - 1] + "NameFade");

                        VI.Entity.Deactivate(toggleMap);

                        VI.Scene.Play("Pause");
                    }
                    
                    // Assumes the main game scene level running is the same as the gamestate name
                    //VI.Scene.Play(VI.GameState.GetName());
                    //if (VI.Scene.IsPaused("Pause") == false)
                    //    VI.Scene.Pause("Pause");
                    //else
                    //VI.Scene.Play("Pause");
                    //VI.Scene.Pause("Game Over");
                    //VI.Scene.Pause("How_To_Play");
                    //VI.Scene.Pause("Quit Confirmation");
                    //VI.Scene.Pause("Dialogue");
                    //VI.Scene.Pause("cutscene1");
                    //VI.Scene.Pause("CrystalBalls");
                    //VI.Scene.Pause("MiniMap");
                    //VI.Scene.Pause("EnemyMap");
                    //VI.Scene.Pause("WeatherMap");
                    //VI.Scene.Pause("BackGround");

                    // Play audio transition if required

                }

                #endregion

                // Game cheats
                #region Player Cheats
                // Left control & shift held
                if (VI.Input.Key.Hold(340) && VI.Input.Key.Hold(341))
                {
                    // A is pressed
                    if (VI.Input.Key.Press(65))
                    {
                        PlayerInDialogue = !PlayerInDialogue;
                    }

                    // B is pressed
                    if (VI.Input.Key.Press(66))
                    {
                        SpeedCheatToggle = !SpeedCheatToggle;
                    }

                    // D is pressed
                    if (VI.Input.Key.Press(68))
                    {
                        PlayerHealth = 12;
                    }
                }
                #endregion

                // Weather effects on the player
                #region Weather Effects
                // Update player's modifier based on current weather
                if (!PlayerInDialogue && !PlayerInOtherAnimation && PlayerCurrWeather == 2) {
                    if (ChangeDirTimer < 0.0) {
                        MovementXModifier = (float)(rand.NextDouble() * 10.0 - 5.0);
                        MovementYModifier = (float)(rand.NextDouble() * 10.0 - 5.0);
                        ChangeDirTimer = rand.Next(3, 8);
                    }
                    else
                        ChangeDirTimer -= VI.General.DeltaTime();
                }
                else {
                    MovementXModifier = 0f;
                    MovementYModifier = 0f;
                    ChangeDirTimer = -1.0;
                }
                #endregion
            }
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void FixedUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            #region Movement
            // Move only if player is not in dialogue or death animation
            if (!PlayerInDialogue && !PlayerInDeathAnimation && !PlayerInOtherAnimation) {
                float DirX = 0f;
                float DirY = 0f;

                if (VI.Input.Mouse.Hold())
                {
                    DirX += VI.Input.Mouse.WorldPosX() + VI.Camera.GetPos.X() - PlayerPosX;
                    DirY += VI.Input.Mouse.WorldPosY() + VI.Camera.GetPos.Y() - PlayerPosY;
                    VI.ParticleSystem.GenerateOnce(_ENTITY);
                }

                // Move if the vector is significant
                if (VI.Math.SqMagnitude(DirX, DirY) > float.Epsilon * float.Epsilon)
                {
                    // Apply force
                    float NormX = VI.Math.Normalize.X(DirX, DirY);
                    float NormY = VI.Math.Normalize.Y(DirX, DirY);
                    ApplyForce(_ENTITY, NormX, NormY, PlayerSpeed * (SpeedCheatToggle ? SpeedCheatMultiplier : 1f));

                    // Get new rotation and set sprite
                    PlayerRotation = GetRotation(NormX, NormY);
                    SetPlayerSprite(_ENTITY, PlayerRotation, "Idle");


                    // Play rowing audio
                    VI.Audio.Play(_ENTITY);
                }
                else
                {
                    // Stop rowing audio
                    VI.Audio.Stop(_ENTITY);
                }

                if (VI.Math.SqMagnitude(MovementXModifier, MovementYModifier) > float.Epsilon * float.Epsilon)
                {
                    ApplyForce(_ENTITY, MovementXModifier, MovementYModifier, 1f);
                }
            }

            // Update player position variables
            PlayerPosX = VI.Transform.Position.GetX(_ENTITY);
            PlayerPosY = VI.Transform.Position.GetY(_ENTITY);

            #endregion

        }

        public void LateUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            // Update player current weather
            if (VI.GameState.GetName() != "LevelTutorial")
                PlayerCurrWeather = VI.Weather.GetCurrent(12, PlayerPosX, PlayerPosY);

            #region Camera Follow Player
            if (CameraFollowPlayer)
                MoveCamera(PlayerPosX, PlayerPosY - PlayerScaleY / 2f);
            #endregion
        }

        public void Exit(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void Dead(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            Init(_ENTITY);
        }

        private void SetPlayerSprite(int _eID, float _rotation, string _status)
        {
            int InitialStatus = 0;
            switch (_status)
            {
                case "Idle":
                    InitialStatus = 8;
                    break;
                case "Hit":
                    InitialStatus = 16;
                    break;
                case "Death":
                    InitialStatus = 24;
                    break;
                default:
                    break;
            }
            if (_rotation < 0)
                _rotation += 2 * (float)Pi;

            if (_rotation >= 15f * MiniAngle || _rotation <= MiniAngle)
                VI.Animation.SpriteSheet.SheetIndex.Set(_eID, InitialStatus - 2);
            else if (_rotation <= 3f * MiniAngle)
                VI.Animation.SpriteSheet.SheetIndex.Set(_eID, InitialStatus - 3);
            else if (_rotation <= 5f * MiniAngle)
                VI.Animation.SpriteSheet.SheetIndex.Set(_eID, InitialStatus - 4);
            else if (_rotation <= 7f * MiniAngle)
                VI.Animation.SpriteSheet.SheetIndex.Set(_eID, InitialStatus - 5);
            else if (_rotation <= 9f * MiniAngle)
                VI.Animation.SpriteSheet.SheetIndex.Set(_eID, InitialStatus - 6);
            else if (_rotation <= 11f * MiniAngle)
                VI.Animation.SpriteSheet.SheetIndex.Set(_eID, InitialStatus - 7);
            else if (_rotation <= 13f * MiniAngle)
                VI.Animation.SpriteSheet.SheetIndex.Set(_eID, InitialStatus - 8);
            else
                VI.Animation.SpriteSheet.SheetIndex.Set(_eID, InitialStatus - 1);
        }
    }
}
