/*!*****************************************************************************
\file PlayerScriptMenu.cs
\author Lee Hsien Wei Joachim
\par DP email: l.hsienweijoachim@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
A script that stores the values of the player
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class PlayerScriptMenu : BaseScript
    {
        static public bool PlayerInDialogue;
        static public bool PlayerInDeathAnimation;
        static public float PlayerHealth;

        private const float MaxHealth = 12f;
        private const float PlayerSpeed = 10f;
        private const float SpeedCheatMultiplier = 2.5f;

        private bool SpeedCheatToggle;

        private float PlayerPosX;
        private float PlayerPosY;
        private float PlayerRotation;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!


        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            // Initialize bool variables
            PlayerHealth = MaxHealth;
            PlayerInDialogue = false;
            PlayerInDeathAnimation = false;

            SpeedCheatToggle = false;

            // Initialize position variables
            PlayerPosX = VI.Transform.Position.GetX(_ENTITY);
            PlayerPosY = VI.Transform.Position.GetY(_ENTITY);
        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void FixedUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            #region Movement
            // Update player position variables
            PlayerPosX = VI.Transform.Position.GetX(_ENTITY);
            PlayerPosY = VI.Transform.Position.GetY(_ENTITY);

            // Move only if player is not in dialogue or death animation
            if (!PlayerInDialogue && !PlayerInDeathAnimation)
            {
                // Left mouse button is held
                if (VI.Input.Mouse.Hold())
                {
                    float DirX = VI.Input.Mouse.WorldPosX() + VI.Camera.GetPos.X() - PlayerPosX;
                    float DirY = VI.Input.Mouse.WorldPosY() + VI.Camera.GetPos.Y() - PlayerPosY;

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

                        // Play rowing boat audio
                        VI.Audio.Play(_ENTITY);
                    }

                }
                else // Not held anymore
                {
                    // Stop rowing audio
                    VI.Audio.Stop(_ENTITY);
                }
            }
            else // In dialogue or death animation
            {
                // Stop rowing audio in case
                VI.Audio.Stop(_ENTITY);
            }

            #endregion

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
