using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
  public class ObjectiveTextScript : BaseScript {

    static private bool showingObjective;
    static private string objective;
    private const string toggleText = "Objective: Click to view";

    public void Alive(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
    }

    public void Init(int _ENTITY) {
      showingObjective = false;
    }

    public void Update(int _ENTITY) {
      if (THIS.Input.Button.Clicked()) {
        showingObjective = !showingObjective;

        if (showingObjective) THIS.Text.Update(objective);
        else THIS.Text.Update(toggleText);
      }

    }

    public void FixedUpdate(int _ENTITY) {

    }

    public void Exit(int _ENTITY) {

    }

    public void Dead(int _ENTITY) {

    }

    static public void UpdateText(string str) {
      if (showingObjective && objective != str) THIS.Text.Update(str);
      objective = str;
    }























  }
}
