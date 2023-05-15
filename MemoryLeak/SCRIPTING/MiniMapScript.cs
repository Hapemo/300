/*!*****************************************************************************
\file MiniMapScript.cs
\author Huang Wei Jhin
\par DP email: h.weijhin\@digipen.edu
\par Group: Memory Leak Studios
\date 28-01-2023
\brief
MiniMapScript contains multiple script components to run the Level1 of the game.
Part of which are:
- Functions of the minimap
- Expanding the minimap
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class MiniMapScript
    {
        public static bool big = false;
        int minimapID;
        int boatID;
        int blurID;
        int closeID;
        int playerheadID;
        float textOffsetX;
        float textOffsetY;
        float textbigOffsetX;
        float textbigOffsetY;
        float textScale;
        float textbigScale;
        private float MapX, MapY;
        //int i = 1;
        public void Alive(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            
        }
        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            string currlevel = VI.GameState.GetName();
            string currlevelBG = currlevel + "Background";
            //if (currlevel == "Level1")
            boatID = VI.Entity.GetId("Boat", currlevel);
            MapX = VI.Transform.Scale.GetX(VI.Entity.GetId("Water", currlevelBG));
            MapY = VI.Transform.Scale.GetY(VI.Entity.GetId("Water", currlevelBG));
            minimapID = VI.Entity.GetId("minimap", "MiniMap");
            playerheadID = VI.Entity.GetId("playerhead", "MiniMap");
            blurID = VI.Entity.GetId("blur", "MiniMap");
            closeID = VI.Entity.GetId("CloseBtn", "MiniMap");

            textOffsetX = -59;// VI.Text.Offset.GetX(minimapID);
            textOffsetY = -170;// VI.Text.Offset.GetY(minimapID);
            textbigOffsetX = -100;
            textbigOffsetY = 360;
            textScale = VI.Text.Scale.Get(minimapID);
            textbigScale = 1.0f;

            big = false;
            VI.MovementAI.SetNext(minimapID, 0);
            VI.Text.Offset.Set(minimapID, textOffsetX, textOffsetY);
            VI.Text.Scale.Set(minimapID, textScale);
            VI.MovementAI.Run(minimapID);
            VI.Viewport.SetIsUI(minimapID, false);
            VI.Viewport.SetWidth(minimapID, (int)(VI.Camera.GetScale.X() * 4));
            VI.Sprite.SetType(minimapID, 1);
            VI.Sprite.SetAlpha(playerheadID, 255);
            VI.ColorAI.SetNextStep(playerheadID, 0);
            //VI.MovementAI.SetNext(playerheadID, 0);
            //VI.MovementAI.Run(playerheadID);
            VI.Entity.SetActive(blurID, false);
            VI.Entity.SetActive(closeID, false);
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

           
            if (!big && (VI.Input.Button.Released(minimapID)))
            {
                big = true;
                VI.Entity.SetActive(closeID, true);
                PlayerScript.PlayerInOtherAnimation = true;
                VI.MovementAI.SetNext(minimapID, 1);
                VI.Text.Offset.Set(minimapID, textbigOffsetX, textbigOffsetY);
                VI.Text.Scale.Set(minimapID, textbigScale);
                VI.MovementAI.Run(minimapID);
                VI.Viewport.SetIsUI(minimapID, true);
                float minimapwidth =   MapX < MapY/9.0f*16.0f? MapY / 9.0f * 16.0f: MapX;
                VI.Viewport.SetWidth(minimapID, (int)(minimapwidth));
                VI.Sprite.SetType(minimapID, 0);

                //float posx = VI.Transform.Position.GetX(boatID) * (miniMapX / MapX) ;
                //float posy = VI.Transform.Position.GetY(boatID) * (miniMapY / MapY) ;//helppp
                //VI.MovementAI.Remove(playerheadID, 1);
                //VI.MovementAI.Add.Transform(playerheadID, 100, 100, 0, posx, posy, 0.5f);

                //VI.MovementAI.SetNext(playerheadID, 1);
                //VI.MovementAI.Run(playerheadID);
                VI.Entity.SetActive(blurID, true);

                VI.ColorAI.StartAnimation(playerheadID);
            }
            else if (big && (VI.Input.Mouse.Release(349)) && (!VI.Input.Button.Released(minimapID)))
            {
                big = false;
                VI.Entity.SetActive(closeID, false);
                PlayerScript.PlayerInOtherAnimation = false;
                VI.MovementAI.SetNext(minimapID, 0);
                VI.Text.Offset.Set(minimapID, textOffsetX, textOffsetY);
                VI.Text.Scale.Set(minimapID, textScale);
                VI.MovementAI.Run(minimapID);
                VI.Viewport.SetIsUI(minimapID, false);
                VI.Viewport.SetWidth(minimapID, (int)VI.Camera.GetScale.X()*4); //(MapX / 4));
                VI.Sprite.SetType(minimapID, 1);

                //VI.MovementAI.SetNext(playerheadID, 0);
                //VI.MovementAI.Run(playerheadID);
                VI.Entity.SetActive(blurID, false);
                VI.ColorAI.StartAnimation(playerheadID);

            }

        }

        public void Exit(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            big = false;
            PlayerScript.PlayerInOtherAnimation = false;
            VI.MovementAI.SetNext(minimapID, 0);
            VI.Text.Offset.Set(minimapID, textOffsetX, textOffsetY);
            VI.Text.Scale.Set(minimapID, textScale);
            VI.MovementAI.Run(minimapID);
            VI.Viewport.SetIsUI(minimapID, false);
            VI.Viewport.SetWidth(minimapID, (int)(VI.Camera.GetScale.X() * 4));
            VI.Sprite.SetType(minimapID, 1);

            VI.ColorAI.SetNextStep(playerheadID, 1);
            //VI.MovementAI.SetNext(playerheadID, 0);
            //VI.MovementAI.Run(playerheadID);
            VI.Entity.SetActive(blurID, false);
        }
        public void Dead(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }
    }
}
