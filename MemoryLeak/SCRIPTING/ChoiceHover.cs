/*!*****************************************************************************
\file PlayerScriptMenu.cs
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
The base script for all the script that will be made
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
  public class ChoiceHover : BaseScript {
    const string choiceBackgroundString = "ChoiceBackground";
    static bool hovering;
    public void Alive(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

    }

    public void Init(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
      
    }

    public void EarlyUpdate(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

      hovering = false;
    }

    public void Update(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

      hovering |= VI.Input.Button.Hover(_ENTITY);
      if (hovering) {
        //if (!VI.Entity.s_IsActive(choiceBackgroundString))
          VI.Entity.s_Activate(choiceBackgroundString);
      } else {
        //if (VI.Entity.s_IsActive(choiceBackgroundString))
          VI.Entity.s_Deactivate(choiceBackgroundString);
      }
      if (VI.Input.Button.Hover(_ENTITY)) {
        VI.Transform.Position.s_SetX(choiceBackgroundString, "", VI.Transform.Position.GetX(_ENTITY));
        VI.Transform.Position.s_SetY(choiceBackgroundString, "", VI.Transform.Position.GetY(_ENTITY));
        VI.Transform.Scale.s_SetX(choiceBackgroundString, "", VI.Transform.Scale.GetX(_ENTITY));
        VI.Transform.Scale.s_SetY(choiceBackgroundString, "", VI.Transform.Scale.GetY(_ENTITY));
      }
    }

    public void FixedUpdate(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

    }

    public void LateUpdate(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

    }

    public void Exit(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
      VI.Entity.s_Deactivate(choiceBackgroundString);
    }

    public void Dead(int _ENTITY) {
      THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

    }
  }
}
