using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {

  public class Level1GirlScript : BaseScript {
    private int colliderBox;
    private int playerBoat;

    public void Alive(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
    }

    public void Init(int _ENTITY) {
      colliderBox = VI.Entity.GetId("LittleGirlBox", "Level1");
      playerBoat = VI.Entity.GetId("Boat", "Level1");
    }

    public void Update(int _ENTITY) {
      if (VI.Physics.EntitiesCollided(colliderBox, playerBoat)) {
        
        Console.WriteLine("collided");
        if (!Level1DialogScript.runGirlDialog)
          Level1DialogScript.runGirlDialog = true;
      }
    }

    public void FixedUpdate(int _ENTITY) {

    }

    public void Exit(int _ENTITY) {

    }

    public void Dead(int _ENTITY) {

    }































  }
}
