/*!*****************************************************************************
\file EnemyAI.cs
\author Huang Wei Jhin
\par DP email: h.weijhin\@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
This script controls all the enemy movement and attacks
*******************************************************************************/
using System;
using System.Data.SqlTypes;
using System.Net.NetworkInformation;
using System.Runtime.CompilerServices;

namespace BonVoyage
{
    public class EnemyAI : BaseScript
    {
        private const int HitMax = 11;
        static public bool EnemyActivated = false;
        private float RightAngle = (float)VI.Math.Pi() / 2;
        bool takingDamage = false;
        private EnemyState eState = EnemyState.IDLE;
        private int PlayerId;
        private int EnemyId;
        private int HpBarId;
        private int toggleMapID;
        private int OverlayId;
        private string currlevel;
        private float enemySpeed;
        private float rainSpeed;
        Random rand = new Random();
        public enum EnemyState
        {
            IDLE = 0,
            RISING = 4,
            ATTACK1 = 8,
            ATTACK2 = 12,
        }

        public enum Axis { x, y }

        public void Alive(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            OverlayId = VI.Entity.GetId("overlay");
            EnemyId = VI.Entity.GetId("Enemy");
            PlayerId = VI.Entity.GetId("Boat");
            HpBarId = VI.Entity.GetId("hpbar");
            toggleMapID = VI.Entity.GetId("toggleMap", "CrystalBalls");
            takingDamage = false;
            ChangeState(EnemyState.IDLE);
            currlevel = VI.GameState.GetName();
            if (currlevel == "Level2")
                enemySpeed = 0.92f;
            else if(currlevel == "Level3")
                enemySpeed = 0.97f;
        }

        public void EarlyUpdate(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Update(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            //Init(_ENTITY);
            
            float diffx = GetDistance(PlayerScript.PlayerPosX, PlayerScript.PlayerPosY, Axis.x);
            float diffy = GetDistance(PlayerScript.PlayerPosX, PlayerScript.PlayerPosY, Axis.y);
            float eDirection = GetEnemyRotation(diffx, diffy);
            if (!VI.Entity.IsActive(toggleMapID))
            {
                SetDirection(eDirection, EnemyState.IDLE);
                THIS.MovementAI.ForceStop();
            }
            else
            {
                int index = VI.Weather.GetCurrent(12, VI.Transform.Position.GetY(PlayerId), VI.Transform.Position.GetY(PlayerId));
                
                if (index == 1 || index == 3 || index == 5 || index == 7)
                {//rain 
                    rainSpeed = 1.045f;
                    THIS.MovementAI.ForceStop();
                    if (!OnScreen(1.2f))
                        ApplyForce(_ENTITY, diffx, diffy, PlayerScript.PlayerSpeed *15.0f);
                }
                else
                {
                    rainSpeed = 1.0f;
                }
                SetDirection(eDirection, eState);
                if (OnScreen(0.98f))
                {
                    if (eState == EnemyState.IDLE)
                    {
                        ChangeState(EnemyState.RISING);
                    }
                    else if (eState == EnemyState.RISING)
                    {
                        if (THIS.Animation.SpriteSheet.FrameCount.Get() == THIS.Animation.SpriteSheet.CurrentFrame.Get() + 1)
                            ChangeState(EnemyState.ATTACK1);
                    }
                }
                else if (!OnScreen(1.3f))
                {
                    THIS.MovementAI.Run();
                    eState = EnemyState.IDLE;
                }
                if (eState != EnemyState.IDLE && eState != EnemyState.RISING)
                {
                    THIS.MovementAI.ForceStop();
                    float xDis = Math.Abs(PlayerScript.PlayerPosX - THIS.Transform.Position.GetX());
                    float yDis = Math.Abs(PlayerScript.PlayerPosY - THIS.Transform.Position.GetY());
                    if ((xDis * 2.0f < THIS.Transform.Scale.GetX() + VI.Transform.Scale.GetX(PlayerId)) && (yDis * 2.0f < THIS.Transform.Scale.GetY() + VI.Transform.Scale.GetX(PlayerId)))
                    {
                        eState = EnemyState.ATTACK2;
                        if (!VI.Physics.IsCollided(PlayerId, THIS.GetId()))
                            ApplyForce(_ENTITY, diffx, diffy, PlayerScript.PlayerSpeed * 0.7f* enemySpeed * rainSpeed);
                        //minus health
                    }
                    else
                    {
                        eState = EnemyState.ATTACK1;
                        ApplyForce(_ENTITY, diffx, diffy, PlayerScript.PlayerSpeed * enemySpeed* rainSpeed);
                    }
                }
            }
        }

        public void FixedUpdate(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            
            // Detecting player hits
            if (eState == EnemyState.ATTACK2 )
            {
                if (THIS.Animation.SpriteSheet.CurrentFrame.Get() == THIS.Animation.SpriteSheet.FrameCount.Get() / 12 * 8) //8th frame (takes 0.3 sec to full red)
                {
                    VI.ColorAI.StartAnimation(OverlayId);
                }
                if (THIS.Animation.SpriteSheet.CurrentFrame.Get() == THIS.Animation.SpriteSheet.FrameCount.Get() -1) //11th frame
                {
                    VI.Audio.Play(OverlayId);
                    if (!takingDamage)
                    { 
                        PlayerScript.PlayerHealth += 1;
                        VI.Animation.SpriteSheet.SheetIndex.Set(HpBarId, PlayerScript.PlayerHealth);
                        
                    }
                    takingDamage = true;
                }
                else
                    takingDamage = false;

            }
            else if (eState == EnemyState.IDLE && PlayerScript.PlayerHealth>0)
            {
                if (THIS.Animation.SpriteSheet.CurrentFrame.Get() == 0)
                {
                    if (!takingDamage)
                    {
                        
                        PlayerScript.PlayerHealth -= 1;
                        VI.Animation.SpriteSheet.SheetIndex.Set(HpBarId, PlayerScript.PlayerHealth);
                    }
                    takingDamage = true;
                }
                else
                    takingDamage = false;
            }
        }

        public void LateUpdate(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void Exit(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void Dead(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }
        private void ChangeState(EnemyState state)
        {
            eState = state;
            THIS.Animation.SpriteSheet.CurrentFrame.Set(0);
        }
        private bool OnScreen(float offset)
        {
            if (!VI.Entity.IsActive(toggleMapID))
            {
                eState = EnemyState.IDLE;
            }
            float EnemyPosX = THIS.Transform.Position.GetX();
            float EnemyPosY = THIS.Transform.Position.GetY();
            // Enemy is in screen
            if (EnemyPosX <= VI.Camera.GetPos.X() + (VI.Camera.GetScale.X() / 2 * offset) &&
                EnemyPosY <= VI.Camera.GetPos.Y() + (VI.Camera.GetScale.Y() / 2 * offset) &&
                EnemyPosX >= VI.Camera.GetPos.X() - (VI.Camera.GetScale.X() / 2 * offset) &&
                EnemyPosY >= VI.Camera.GetPos.Y() - (VI.Camera.GetScale.Y() / 2 * offset))
            {
                return true;
            }
            else return false;
        }
        private float GetDistance(float _x, float _y, Axis _axis)
        {
            float xDis = _x - THIS.Transform.Position.GetX();
            float yDis = _y - THIS.Transform.Position.GetY();
            float xNorm = 0f, yNorm = 0f;
            if (VI.Math.SqMagnitude(xDis, yDis) > VI.Math.Epsilon() * VI.Math.Epsilon())
            {
                xNorm = VI.Math.Normalize.X(xDis, yDis);
                yNorm = VI.Math.Normalize.Y(xDis, yDis);
            }
            return (_axis == Axis.x ? xNorm : yNorm);
        }

        private float GetEnemyRotation(float _x, float _y)
        {
            float Rotation = 0;
            if (_y != 0f && _x >= 0f)
                Rotation = VI.Math.ArcTangent(_y, _x);
            else if (_y == 0f && _x > 0f)
                Rotation = (float)VI.Math.Pi() / 2;
            else if (_y != 0f && _x < 0f)
            {
                Rotation = VI.Math.ArcTangent(_y, _x);
                Rotation += Rotation < 0f ? (float)VI.Math.Pi() * 2f : 0f;
            }
            else Rotation = 3f * (float)VI.Math.Pi() / 2f;
            return Rotation;
        }

        private void SetDirection(float _rotation, EnemyState _status)
        {
            // 1st Quadrant
            if (0 <= _rotation && _rotation < RightAngle)
                THIS.Animation.SpriteSheet.SheetIndex.Set((int)_status + 2);
            // 2nd Quadrant
            else if (RightAngle <= _rotation && _rotation < VI.Math.Pi())
                THIS.Animation.SpriteSheet.SheetIndex.Set((int)_status + 1);
            // 3rd Quadrant
            else if (VI.Math.Pi() <= _rotation && _rotation < 3f * RightAngle)
                THIS.Animation.SpriteSheet.SheetIndex.Set((int)_status);
            // 4th Quadrant
            else
                THIS.Animation.SpriteSheet.SheetIndex.Set((int)_status + 3);
        }
    }
}
