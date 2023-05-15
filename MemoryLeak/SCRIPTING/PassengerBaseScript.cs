/*!*****************************************************************************
\file PassengerBaseScript.cs
\author Lee Hsien Wei Joachim
\par DP email: l.hsienweijoachim@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
Base script for passenger control, movement and everything related to passenger
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;
using VI;

namespace BonVoyage {
    public class PassengerBaseScript : BaseScript {
        protected int playerBoat = 0;
        protected int triggerBox = 0;

        protected int correctDestination_Box = 0;
        protected int correctDestination_RenderLocation = 0;

        protected int wrongDestination_Box = 0;
        protected int wrongDestination_RenderLocation = 0;

        protected bool correctDestinationDelivery;

        protected bool ReadyToAttach;
        protected bool fadingOut;
        protected bool AttachToPlayerAnimation;
        protected bool AttachedToPlayer;
        protected bool DetachFromPlayerAnimation;
        protected bool DestinationReached;

        protected float InitialScaleX;
        protected int InitialLayerVal;

    protected enum FADE {
      IN,
      OUT
    }
        public virtual void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public virtual void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            correctDestinationDelivery = false;

            ReadyToAttach = false;
            fadingOut = false;
            AttachToPlayerAnimation = false;
            AttachedToPlayer = false;
            DetachFromPlayerAnimation = false;
            DestinationReached = false;

            // Store original scale x value
            InitialScaleX = VI.Transform.Scale.GetX(_ENTITY);
            InitialLayerVal = VI.Sprite.GetLayer(_ENTITY);
        }

        public virtual void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public virtual void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            if (AttachedToPlayer && PlayerScript.PlayerInDeathAnimation) {
                // Passenger animation for when player is dying
                VI.ParticleSystem.GenerateOnce(_ENTITY);
                Fading(_ENTITY, 300f, FADE.OUT);
            }
        }

        public virtual void FixedUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public virtual void LateUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public virtual void Exit(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            
            correctDestinationDelivery = false;

            ReadyToAttach = false;
            fadingOut = false;
            AttachToPlayerAnimation = false;
            AttachedToPlayer = false;
            DetachFromPlayerAnimation = false;
            DestinationReached = false;
        }

        public virtual void Dead(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        // Plays animation for attaching a passsenger entity to player entity
        // Returns true once complete
        protected bool AttachPassengerToPlayer(int _ePassenger, int _ePlayer, float _passengerInitialScaleX) {
            // Get distance between passenger and boat
            float diffX = VI.Transform.Position.GetX(_ePlayer) - VI.Transform.Position.GetX(_ePassenger);
            float diffY = VI.Transform.Position.GetY(_ePlayer) - VI.Transform.Position.GetY(_ePassenger);

            if (diffX * diffX > (_passengerInitialScaleX * _passengerInitialScaleX + VI.Transform.Scale.GetX(_ePlayer) * VI.Transform.Scale.GetX(_ePlayer)) / 4f) {
                // Animation
                VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_ePassenger) + VI.Math.Normalize.X(diffX, diffY) * 200f * (float)VI.General.DeltaTime());
                VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_ePassenger) + VI.Math.Normalize.Y(diffX, diffY) * 200f * (float)VI.General.DeltaTime());

                //VI.Transform.Scale.SetX(_ePassenger, VI.Transform.Scale.GetX(_ePassenger) / (1 + 8 * (float)VI.General.DeltaTime()));
                //VI.Transform.Scale.SetY(_ePassenger, VI.Transform.Scale.GetY(_ePassenger) / (1 + 8 * (float)VI.General.DeltaTime()));

                return false;
            }
            else {
                return true;
            }
        }

        // Moves the passenger along with the player and updates its layer values accordingly
        protected void MovePassengerWithPlayer(int _ePassenger, int _ePlayer, float _passengerInitialScaleX) {
            // Restore passenger's original scale x value
            this.RestorePassengerScale(_ePassenger, _passengerInitialScaleX);

            // Get player's direction
            int playerDirection = VI.Animation.SpriteSheet.SheetIndex.Get(_ePlayer) % 8;
            switch (playerDirection)
            {
                case 0: // South / Front
                    VI.Sprite.SetLayer(_ePassenger, VI.Sprite.GetLayer(_ePlayer) + 1);
                    VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_ePlayer));
                    VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_ePlayer));
                    VI.Animation.SpriteSheet.SheetIndex.Set(_ePassenger, 6);
                    break;
                case 1: // SW
                    VI.Sprite.SetLayer(_ePassenger, VI.Sprite.GetLayer(_ePlayer) + 1);
                    VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_ePlayer) - VI.Transform.Scale.GetX(_ePlayer) / 16f);
                    VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_ePlayer));
                    VI.Animation.SpriteSheet.SheetIndex.Set(_ePassenger, 5);
                    break;
                case 2: // West / Left
                    VI.Sprite.SetLayer(_ePassenger, VI.Sprite.GetLayer(_ePlayer) + 1);
                    VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_ePlayer) - VI.Transform.Scale.GetX(_ePlayer) / 4f);
                    VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_ePlayer) + VI.Transform.Scale.GetY(_ePlayer) / 32f);
                    VI.Animation.SpriteSheet.SheetIndex.Set(_ePassenger, 7);
                    break;
                case 3: // NW
                    VI.Sprite.SetLayer(_ePassenger, VI.Sprite.GetLayer(_ePlayer) + 1);
                    VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_ePlayer) - VI.Transform.Scale.GetX(_ePlayer) / 6f);
                    VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_ePlayer) + VI.Transform.Scale.GetY(_ePlayer) / 8f);
                    VI.Animation.SpriteSheet.SheetIndex.Set(_ePassenger, 3);
                    break;
                case 4: // North / Back
                    VI.Sprite.SetLayer(_ePassenger, VI.Sprite.GetLayer(_ePlayer) - 1);
                    VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_ePlayer));
                    VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_ePlayer));
                    break;
                case 5: // NE
                    VI.Sprite.SetLayer(_ePassenger, VI.Sprite.GetLayer(_ePlayer) + 1);
                    VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_ePlayer) + VI.Transform.Scale.GetX(_ePlayer) / 6f);
                    VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_ePlayer) + VI.Transform.Scale.GetY(_ePlayer) / 8f);
                    VI.Animation.SpriteSheet.SheetIndex.Set(_ePassenger, 2);
                    break;
                case 6: // East / Right
                    VI.Sprite.SetLayer(_ePassenger, VI.Sprite.GetLayer(_ePlayer) + 1);
                    VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_ePlayer) + VI.Transform.Scale.GetX(_ePlayer) / 4f);
                    VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_ePlayer) + VI.Transform.Scale.GetY(_ePlayer) / 32f);
                    VI.Animation.SpriteSheet.SheetIndex.Set(_ePassenger, 1);
                    break;
                case 7: // SE
                    VI.Sprite.SetLayer(_ePassenger, VI.Sprite.GetLayer(_ePlayer) + 1);
                    VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_ePlayer) + VI.Transform.Scale.GetX(_ePlayer) / 10f);
                    VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_ePlayer));
                    VI.Animation.SpriteSheet.SheetIndex.Set(_ePassenger, 4);
                    break;
                default:
                    break;
            }
        }

        // Plays animation for detaching a passsenger entity from player entity to a destination entity
        // Returns true once complete
        protected bool DetachPassengerFromPlayer(int _ePassenger, int _eDestination, float _passengerInitialScaleX) {
            // Get distance between passenger and boat
            float diffX = VI.Transform.Position.GetX(_eDestination) - VI.Transform.Position.GetX(_ePassenger);
            float diffY = VI.Transform.Position.GetY(_eDestination) - VI.Transform.Position.GetY(_ePassenger);

            if (diffX * diffX > (_passengerInitialScaleX * _passengerInitialScaleX + VI.Transform.Scale.GetX(_eDestination) * VI.Transform.Scale.GetX(_eDestination)) / 3f)
            {
                // Animation
                VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_ePassenger) + diffX * (float)VI.General.DeltaTime());
                VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_ePassenger) + diffY * (float)VI.General.DeltaTime());

                //VI.Transform.Scale.SetX(_ePassenger, VI.Transform.Scale.GetX(_ePassenger) / (1 + 8 * (float)VI.General.DeltaTime()));
                //VI.Transform.Scale.SetY(_ePassenger, VI.Transform.Scale.GetY(_ePassenger) / (1 + 8 * (float)VI.General.DeltaTime()));
                return false;
            }
            else {
                VI.Transform.Position.SetX(_ePassenger, VI.Transform.Position.GetX(_eDestination));
                VI.Transform.Position.SetY(_ePassenger, VI.Transform.Position.GetY(_eDestination));

                return true;
            }
        }

        // Plays animation to restore passsenger entity's scale value
        // Returns true once complete
        protected bool RestorePassengerScale(int _ePassenger, float _passengerInitialScaleX) {
            //LOG.WRITE("Passenger original X: " + _passengerInitialScaleX);
            //LOG.WRITE("Current scale value: " + VI.Transform.Scale.GetX(_ePassenger));

            // Restore passenger's original scale x value
            if (VI.Transform.Scale.GetX(_ePassenger) < _passengerInitialScaleX) {
                VI.Transform.Scale.SetX(_ePassenger, VI.Transform.Scale.GetX(_ePassenger) * (1 + 8 * (float)VI.General.DeltaTime()));
                VI.Transform.Scale.SetY(_ePassenger, VI.Transform.Scale.GetY(_ePassenger) * (1 + 8 * (float)VI.General.DeltaTime()));
                return false;
            }
            else {
                return true;
            }
        }

    protected void SetAlpha(int _e, float _alpha) {
      VI.Sprite.SetColor(_e,
                         VI.Sprite.GetColor(_e, 0),
                         VI.Sprite.GetColor(_e, 1),
                         VI.Sprite.GetColor(_e, 2),
                         (int)_alpha);
    }

    // _e = entity, _speed = fading speed, _fadeIn = fade in if true, fade out if false
    // return true if fading completed, otherwise false
    protected bool Fading(int _e, float _speed, FADE _fadeStatus) {
      float alpha = VI.Sprite.GetColor(_e, 3); // VI.LightSource.SpriteColor.GetA(); // Get color TODO
      if (_fadeStatus == FADE.IN) {
        if (alpha >= 255) return true;
        //Console.WriteLine("fade in alpha: " + alpha + '\n');
        alpha += (_speed * (float)VI.General.DeltaTime() < 1 ? 1 : _speed * (float)VI.General.DeltaTime());
        SetAlpha(_e, alpha > 255 ? 255 : alpha);
        if (alpha >= 255) return true;
      } else {
        if (alpha <= 0) return true;
        alpha -= (_speed * (float)VI.General.DeltaTime() < 1 ? 1 : _speed * (float)VI.General.DeltaTime());
        SetAlpha(_e, alpha <= 0 ? 0 : alpha);
        if (alpha <= 0) return true;
      }
      return false;
    }

    protected void DefaultAttachToPlayer(int _e) {
      // Animate attachment to player
      // returns true once complete
      if (fadingOut) { // Fading out
        VI.ParticleSystem.GenerateOnce(_e);
        fadingOut = !Fading(_e, 300.0f, FADE.OUT);
        if (!fadingOut) { // Just finished fading out
          VI.Audio.Play(_e);
          VI.Transform.Position.SetX(_e, VI.Transform.Position.GetX(playerBoat));
          VI.Transform.Position.SetY(_e, VI.Transform.Position.GetY(playerBoat));
        }
      } else {
        if (Fading(_e, 300.0f, FADE.IN)) { // Run the conditional when finished fading in
          AttachedToPlayer = true;
          AttachToPlayerAnimation = false;
          PlayerScript.PlayerInOtherAnimation = false;
          fadingOut = true;
        } else {
          VI.ParticleSystem.GenerateOnce(_e);
        }
      }
    }

    protected void DefaultAttachToDestination(int _e) {
      // Animate attachment to player
      // returns true once complete
      if (fadingOut) { // Fading out
        TransferEntityPos(_e, playerBoat); // Because the boat will be moving when transfering
        VI.ParticleSystem.GenerateOnce(_e);
        fadingOut = !Fading(_e, 300.0f, FADE.OUT);
        if (!fadingOut) { // Just finished fading out
          int renderLocation = correctDestinationDelivery ? correctDestination_RenderLocation : wrongDestination_RenderLocation;
          VI.Transform.Position.SetX(_e, VI.Transform.Position.GetX(renderLocation));
          VI.Transform.Position.SetY(_e, VI.Transform.Position.GetY(renderLocation));
        }
      } else {
        if (Fading(_e, 300.0f, FADE.IN)) { // Run the conditional when finished fading in
          //Console.WriteLine("fading in\n");
          DetachFromPlayerAnimation = false;
          PlayerScript.PlayerInOtherAnimation = false;
          DestinationReached = true;
          fadingOut = true;
        } else {
          VI.ParticleSystem.GenerateOnce(_e);
        }
      }
    }

  }
}
