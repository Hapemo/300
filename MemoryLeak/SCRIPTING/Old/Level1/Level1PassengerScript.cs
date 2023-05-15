using System;
using System.Runtime.CompilerServices;
using System.Security.Policy;

namespace BonVoyage {
  public class Level1PassengerScript : BaseScript {
    private int colliderBox;
    private int playerBoat;
    private int destination;
    private int destinationHouse;

    private float boatTransformX;
    private float boatTransformY;
    private float passengerTransformX;
    private float passengerTransformY;
    private float diffTransformX;
    private float diffTransformY;

    private float ogPassengerSizeX;

    private int passengerState; // 0 = waiting for dialog
                                // 1 = waiting for dialog to end then transporting passenger to boat
                                // 2 = passenger on the boat otw to destination
                                // 3 = transporting passenger to destination
                                // 4 = passenger transported
    public void Alive(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
    }

    public void Init(int _ENTITY) {
      colliderBox = VI.Entity.GetId("PassengerBox", "Level1");
      playerBoat = VI.Entity.GetId("Boat", "Level1");
      destination = VI.Entity.GetId("destination", "Level1");
      destinationHouse = VI.Entity.GetId("Single Story House", "Level1");

      passengerState = 0;
      ogPassengerSizeX = THIS.Transform.Scale.GetX();
    }

    public void Update(int _ENTITY) {
      switch (passengerState) {
      case 0: // Waiting for dialog
        if (VI.Physics.EntitiesCollided(colliderBox, playerBoat)) {
          if (!Level1DialogScript.runPassengerDialog) {
            Level1DialogScript.runPassengerDialog = true;
            passengerState = 1;
          }
        }
        break;
      case 1: // Transporting to boat
        boatTransformX = VI.Transform.Position.GetX(playerBoat);
        boatTransformY = VI.Transform.Position.GetY(playerBoat);
        passengerTransformX = THIS.Transform.Position.GetX();
        passengerTransformY = THIS.Transform.Position.GetY();
        diffTransformX = boatTransformX - passengerTransformX;
        diffTransformY = boatTransformY - passengerTransformY;

        if (abs(diffTransformX) > boatTransformX/3 || abs(diffTransformY) > boatTransformY / 3) { 
          THIS.Transform.Position.SetX(passengerTransformX + diffTransformX/magnitude(diffTransformX, diffTransformY) * (float)VI.General.DeltaTime());
          THIS.Transform.Position.SetY(passengerTransformY + diffTransformY/magnitude(diffTransformX, diffTransformY) * (float)VI.General.DeltaTime());
          
          THIS.Transform.Scale.SetX(THIS.Transform.Scale.GetX()/(1+8*(float)VI.General.DeltaTime()));
          THIS.Transform.Scale.SetY(THIS.Transform.Scale.GetY()/(1+8*(float)VI.General.DeltaTime()));

          // TODO Need to decrease passenger alpha, passenger.GetComponent<Sprite>().color.a -= 10;
        } else {
          passengerState = 2;
        }
        //diff = boatTransform->translation - passengerTransform->translation;
        //if (std::fabs(diff.x) > boatTransform->scale.x / 3 || std::fabs(diff.y) > boatTransform->scale.y / 3) {
        //  passengerTransform->translation += diff.Normalized() * (float)FPSManager::dt * 200;
        //  passengerTransform->scale /= 1 + 8 * (float)FPSManager::dt;
        //  passenger.GetComponent<Sprite>().color.a -= 10;
        //} else {
        //  pickedUp = true;
        //  pickingUp = false;
        //}
        break;
      case 2: { // Travelling on boat
            //if (passenger.GetComponent<Sprite>().color.a < 255) { TODO change alpha
            //  passenger.GetComponent<Sprite>().color.a += 10;
            //}
        if (THIS.Transform.Scale.GetX() < ogPassengerSizeX) {
          THIS.Transform.Scale.SetX(THIS.Transform.Scale.GetX() * (1 + 8 * (float)VI.General.DeltaTime()));
          //passengerTransform->scale *= 1 + 8 * (float)FPSManager::dt;
        }
        //passengerTransform->translation = boatTransform->translation;
        int direction = VI.Animation.CurrentFrame.Get(playerBoat) % 8;
        //int direction = boat.GetComponent<Animation>().currentImageIndex % 8;
        switch (direction) {
          case 0://front
            VI.Texture.SetLayer(THIS.GetId(), VI.Texture.GetLayer(playerBoat) + 1);
            VI.Transform.Position.SetX(THIS.GetId(), VI.Transform.Position.GetX(playerBoat));
            VI.Transform.Position.SetY(THIS.GetId(), VI.Transform.Position.GetY(playerBoat));
            break;
          case 1:
            VI.Texture.SetLayer(THIS.GetId(), VI.Texture.GetLayer(playerBoat) + 1);
            VI.Transform.Position.SetX(THIS.GetId(), VI.Transform.Position.GetX(playerBoat) - VI.Transform.Scale.GetX(playerBoat) / 10.0f);
            VI.Transform.Position.SetY(THIS.GetId(), VI.Transform.Position.GetY(playerBoat));
            break;
          case 2://left
            VI.Texture.SetLayer(THIS.GetId(), VI.Texture.GetLayer(playerBoat) + 1);
            VI.Transform.Position.SetX(THIS.GetId(), VI.Transform.Position.GetX(playerBoat) - VI.Transform.Scale.GetX(playerBoat) / 4.0f);
            VI.Transform.Position.SetY(THIS.GetId(), VI.Transform.Position.GetY(playerBoat) + VI.Transform.Scale.GetY(playerBoat) / 16.0f);
            break;
          case 3:
            VI.Texture.SetLayer(THIS.GetId(), VI.Texture.GetLayer(playerBoat) + 1);
            VI.Transform.Position.SetX(THIS.GetId(), VI.Transform.Position.GetX(playerBoat) - VI.Transform.Scale.GetX(playerBoat) / 6.0f);
            VI.Transform.Position.SetY(THIS.GetId(), VI.Transform.Position.GetY(playerBoat) + VI.Transform.Scale.GetY(playerBoat) / 8.0f);
            break;
          case 4://back
            VI.Texture.SetLayer(THIS.GetId(), VI.Texture.GetLayer(playerBoat) - 1);
            VI.Transform.Position.SetX(THIS.GetId(), VI.Transform.Position.GetX(playerBoat));
            VI.Transform.Position.SetY(THIS.GetId(), VI.Transform.Position.GetY(playerBoat));
            break;
          case 5:
            VI.Texture.SetLayer(THIS.GetId(), VI.Texture.GetLayer(playerBoat) + 1);
            VI.Transform.Position.SetX(THIS.GetId(), VI.Transform.Position.GetX(playerBoat) + VI.Transform.Scale.GetX(playerBoat) / 6.0f);
            VI.Transform.Position.SetY(THIS.GetId(), VI.Transform.Position.GetY(playerBoat) + VI.Transform.Scale.GetY(playerBoat) / 8.0f);
            break;
          case 6://right
            VI.Texture.SetLayer(THIS.GetId(), VI.Texture.GetLayer(playerBoat) + 1);
            VI.Transform.Position.SetX(THIS.GetId(), VI.Transform.Position.GetX(playerBoat) + VI.Transform.Scale.GetX(playerBoat) / 4.0f);
            VI.Transform.Position.SetY(THIS.GetId(), VI.Transform.Position.GetY(playerBoat) + VI.Transform.Scale.GetY(playerBoat) / 16.0f);
            break;
          case 7:
            VI.Texture.SetLayer(THIS.GetId(), VI.Texture.GetLayer(playerBoat) + 1);
            VI.Transform.Position.SetX(THIS.GetId(), VI.Transform.Position.GetX(playerBoat) + VI.Transform.Scale.GetX(playerBoat) / 10.0f);
            VI.Transform.Position.SetY(THIS.GetId(), VI.Transform.Position.GetY(playerBoat));
            break;
            /*default:
              passenger.GetComponent<Sprite>().layer = boat.GetComponent<Sprite>().layer + 1;
              passengerTransform->translation = boatTransform->translation;*/

        }


        break;
      }
      case 3: // Transporting to destination
      
        break;
      case 4: // Transported
      
        break;
      }

    }

    public void FixedUpdate(int _ENTITY) {

    }

    public void Exit(int _ENTITY) {

    }

    public void Dead(int _ENTITY) {

    }


    float abs(float x) { return x < 0 ? -x : x; }

    float magnitude(float x, float y) { return (float)Math.Sqrt(x*x + y*y); }






  }
}
