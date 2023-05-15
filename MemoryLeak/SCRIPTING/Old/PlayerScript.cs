/*!*****************************************************************************
\file PlayerScript
\author Lee Hsien Wei, Joachim
\par DP email: l.hsienweijoachim@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 23-02-2022
\brief
The PlayerScript handles the player movement, its sprite updates & camera 
following player on movement, cheats tied to the player as well as the handling 
of animation when the player dies. It also handles the pausing.

Press "CTRL+SHIFT+B" to toggle player's speed increase.
Press "ESC" to toggle the pause menu.
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class PlayerScript : BaseScript {
        static public bool PlayerInDialogue;
        static public float PlayerHealth;

        private const float MaxPlayerHealth = 12f;
        private bool InDeathAnimation;
        private const float PlayerSpeed = 500f;

        private bool SpeedCheatToggle;
        private const float SpeedCheatMultiplier = 2.5f;

        private const double Pi = 3.141592653589793238f;
        private const float MiniAngle = (float)Pi / 8;
        private const float FixedDT = 1f / 60f;


        // Dictionary for relics?

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY) {
            PlayerInDialogue = false;
            PlayerHealth = MaxPlayerHealth;
            SpeedCheatToggle = false;
            InDeathAnimation = false;
        }

        public void Update(int _ENTITY) {
            #region Pause Game
            // Escape key is pressed
            if (VI.Input.Key.Press(256)) {
                VI.Scene.Pause(VI.GameState.GetName());
                VI.Scene.Play("Pause");
                // Play audio VI::iAudio::ForcePlay("Button_Click_SFX");
                // return;
            }
            #endregion

            #region Cheat Codes
            // Left control & shift held
            if (VI.Input.Key.Hold(340) && VI.Input.Key.Hold(341)) {
                // B is pressed
                if (VI.Input.Key.Press(66)) {
                    SpeedCheatToggle = !SpeedCheatToggle;
                }
            }
            #endregion

            #region Player Movement
            // Get entity position which is assumed to be player
            float PlayerPosX = VI.Transform.Position.GetX(_ENTITY);
            float PlayerPosY = VI.Transform.Position.GetY(_ENTITY);
            
            // Not in dialogue
            if (!PlayerInDialogue && !InDeathAnimation) {
                // Left mouse button held
                if (VI.Input.Key.Hold(349)) {
                    float DirX = VI.Input.Mouse.WorldPosX() + VI.Camera.GetPos.X() - PlayerPosX;
                    float DirY = VI.Input.Mouse.WorldPosY() + VI.Camera.GetPos.Y() - PlayerPosY;
                    // Move if the vector is significant
                    if (VI.Math.SqMagnitude(DirX, DirY) > float.Epsilon * float.Epsilon) {
                        // Apply force
                        float NormX = VI.Math.Normalize.X(DirX, DirY);
                        float NormY = VI.Math.Normalize.Y(DirX, DirY);
                        ApplyForce(_ENTITY, NormX, NormY, PlayerSpeed * (SpeedCheatToggle ? SpeedCheatMultiplier : 1f));

                        // Get new rotation and set sprite
                        float PlayerRot = GetRotation(NormX, NormY);
                        SetPlayerSprite(_ENTITY, PlayerRot, "Idle");

                        // Play rowing boat audio
                        VI.Audio.Play(_ENTITY);
                    }
                }
                else
                {
                    // Stop rowing boat audio
                    VI.Audio.Stop(_ENTITY);
                }
            }
            else {
                // Stop rowing boat audio
                VI.Audio.Stop(_ENTITY);
            }
            #endregion

            #region Camera Follow
            // Update camera position to follow entity assumed to be player
            VI.Camera.SetPos.X(VI.Camera.GetPos.X() + (PlayerPosX - VI.Camera.GetPos.X()) * (SpeedCheatToggle ? SpeedCheatMultiplier : 1f) * FixedDT);
            VI.Camera.SetPos.Y(VI.Camera.GetPos.Y() + (PlayerPosY - VI.Camera.GetPos.Y()) * (SpeedCheatToggle ? SpeedCheatMultiplier : 1f) * FixedDT);
            #endregion

            #region Player Death

            // Set animation and update bool flag
            if (PlayerHealth <= 0f) {
                SetPlayerSprite(_ENTITY, 0f, "Death");
                VI.Animation.FrameCount.Set(_ENTITY, 0);
                InDeathAnimation = true;

            }

            // Check for animation completion, then transition state
            if (InDeathAnimation && VI.Animation.CurrentFrame.Get(_ENTITY) == VI.Animation.FrameCount.Get(_ENTITY)) {
                VI.Scene.Pause(VI.GameState.GetName());
                VI.Scene.Play("Game Over");
            }

            #endregion
        }

        public void FixedUpdate(int _ENTITY) {

        }

        public void Exit(int _ENTITY) {

        }

        public void Dead(int _ENTITY) {

        }

        public float GetRotation(float _x, float _y) {
            float Rotation = 0;
            if (_y != 0f && _x >= 0f)
            Rotation = VI.Math.ArcTangent(_y, _x);
            else if (_y == 0f && _x > 0f)
            Rotation = (float)Pi / 2;
            else if (_y != 0f && _x < 0f) {
            Rotation = VI.Math.ArcTangent(_y, _x);
            Rotation += Rotation < 0f ? (float)Pi * 2f : 0f;
            } else
            Rotation = 3f * (float)Pi / 2f;

            return Rotation;
        }

        private void SetPlayerSprite(int _eID, float _rotation, string _status) {
            int InitialStatus = 0;
            switch (_status) {
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
            VI.Animation.SheetIndex.Set(_eID, InitialStatus - 2);
            else if (_rotation <= 3f * MiniAngle)
            VI.Animation.SheetIndex.Set(_eID, InitialStatus - 3);
            else if (_rotation <= 5f * MiniAngle)
            VI.Animation.SheetIndex.Set(_eID, InitialStatus - 4);
            else if (_rotation <= 7f * MiniAngle)
            VI.Animation.SheetIndex.Set(_eID, InitialStatus - 5);
            else if (_rotation <= 9f * MiniAngle)
            VI.Animation.SheetIndex.Set(_eID, InitialStatus - 6);
            else if (_rotation <= 11f * MiniAngle)
            VI.Animation.SheetIndex.Set(_eID, InitialStatus - 7);
            else if (_rotation <= 13f * MiniAngle)
            VI.Animation.SheetIndex.Set(_eID, InitialStatus - 8);
            else
            VI.Animation.SheetIndex.Set(_eID, InitialStatus - 1);
        }

        public void ApplyForce(int _eID, float _x, float _y, float _multiplier) {
            VI.Physics.ApplyImpulse(_eID,
                (_x * _multiplier * (float)VI.General.DeltaTime()),
                (_y * _multiplier * (float)VI.General.DeltaTime()), 0f, 0f);
        }
    }
}
