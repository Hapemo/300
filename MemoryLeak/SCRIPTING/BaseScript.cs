/*!*****************************************************************************
\file BaseScript.cs
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Group: Memory Leak Studios
\date 15-02-2023
\brief
Base script to be inherited by other scripts.
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage
{
    
    public class BaseScript
    {
        public const double Pi = 3.141592653589793238f;
        public const float MiniAngle = (float)Pi / 8;


        public static void CamZoom33Percent() {
          VI.Camera.SetScale.X(1200);
          VI.Camera.SetScale.Y(675);
        }

        public static void CamZoomNormal() {
          VI.Camera.SetScale.X(1600);
          VI.Camera.SetScale.Y(900);
        }

        public static bool MovePlayer(int player, float x, float y) {
          float playerPosX = VI.Transform.Position.GetX(player);
          float playerPosY = VI.Transform.Position.GetY(player);
          //if (((int)playerPosX == (int)x) && ((int)playerPosY == (int)y))
          //  return true;
          float DirX = x - playerPosX;
          float DirY = y - playerPosY;

          // Move if the vector is significant
          if (VI.Math.SqMagnitude(DirX, DirY) > 9f) {
            // Find direction
            float NormX = VI.Math.Normalize.X(DirX, DirY);
            float NormY = VI.Math.Normalize.Y(DirX, DirY);

            // Apply force
            //VI.Physics.ApplyImpulse(player,
            //    (NormX * 200) * (float)VI.General.DeltaTime() * 3.0f,
            //    (NormY * 200) * (float)VI.General.DeltaTime() * 3.0f, 0f, 0f);
            ApplyForce(player, NormX, NormY, 5f);

            // Get new rotation and set sprite
            //PlayerRotation = GetRotation(NormX, NormY);
            //SetPlayerSprite(player, PlayerRotation, "Idle");

            // Play rowing boat audio
            VI.Audio.Play(player);
          }
            else {
                VI.Audio.Stop(player);
                return true;
            }

          //float tempX = playerPosX - x;
          //float tempY = playerPosY - y;
          //if (tempX < 0) { tempX = -tempX; }
          //if (tempY < 0) { tempY = -tempY; }
          //if (((int)tempX < 2) && ((int)tempY < 2)) {
          //  VI.Transform.Position.SetX(player, x);
          //  VI.Transform.Position.SetY(player, y);
          //}

          return false;
        }

        public static void MoveCamera(float x, float y) {
            //VI.Camera.SetPos.X(VI.Camera.GetPos.X() + (x - VI.Camera.GetPos.X()) * (float)VI.General.DeltaTime() * 3.0f);
            //VI.Camera.SetPos.Y(VI.Camera.GetPos.Y() + (y - VI.Camera.GetPos.Y()) * (float)VI.General.DeltaTime() * 3.0f);
           
            //VI.Test.ArgString("Player: " + x + ", " + y);
            //VI.Test.ArgString("Camera: " + VI.Camera.GetPos.X() + ", " + VI.Camera.GetPos.Y());

            VI.Camera.SetPos.X(Lerp(VI.Camera.GetPos.X(), x, (float)(VI.General.DeltaTime() * 3.0)));
            VI.Camera.SetPos.Y(Lerp(VI.Camera.GetPos.Y(), y, (float)(VI.General.DeltaTime() * 3.0)));
        }

        public static bool ChangeZoom(float x, float speed) {
          if (VI.Camera.GetScale.X() == x)
            return true;
          float currX = VI.Camera.GetScale.X();

          if (x > currX) {
            VI.Camera.SetScale.X(currX + speed * (float)VI.General.DeltaTime());
            //VI.Camera.SetScale.X(Lerp(currX, x, 1f / 60f ));
            if (VI.Camera.GetScale.X() > x)
              VI.Camera.SetScale.X(x);
          } else {
            VI.Camera.SetScale.X(currX - speed * (float)VI.General.DeltaTime());
            //VI.Camera.SetScale.X(Lerp(x, currX, 1f / 60f));
            if (VI.Camera.GetScale.X() < x)
              VI.Camera.SetScale.X(x);
          }

          return false;
        }

        public float GetRotation(float _x, float _y)
        {
            float Rotation = 0;
            if (_y != 0f && _x >= 0f)
                Rotation = VI.Math.ArcTangent(_y, _x);
            else if (_y == 0f && _x > 0f)
                Rotation = (float)Pi / 2;
            else if (_y != 0f && _x < 0f)
            {
                Rotation = VI.Math.ArcTangent(_y, _x);
                Rotation += Rotation < 0f ? (float)Pi * 2f : 0f;
            }
            else
                Rotation = 3f * (float)Pi / 2f;

            return Rotation;
        }

        public static void ApplyForce(int _eID, float _x, float _y, float _multiplier)
        {
            VI.Physics.ApplyImpulse(_eID,
                (_x * _multiplier),
                (_y * _multiplier), 0f, 0f);
        }

        public static float Lerp(float _f1, float _f2, float _t)
        {
            //VI.Test.ArgString("Value: " + (_f1 + (_f2 - _f1) * _t));
            return _f1 + (_f2 - _f1) * _t;
        }

    public void TransferEntityPos(int _e1, int _e2) {
      VI.Transform.Position.SetX(_e1, VI.Transform.Position.GetX(_e2));
      VI.Transform.Position.SetY(_e1, VI.Transform.Position.GetY(_e2));
    }

    }
}
