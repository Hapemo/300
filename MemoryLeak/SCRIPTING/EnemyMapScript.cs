/*!*****************************************************************************
\file EnemyMapScript.cs
\author Huang Wei Jhin
\par DP email: h.weijhin\@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
EnemyMapScript contains multiple script components to run the Level1 of the game.
Part of which are:
- Functions of the ememy map
- Expanding the ememy minimap
- Calculating the position of the player and enemy in the mini and expanded map
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class EnemyMapScript
    {
        public static bool big = false;
        int enemymapID;
        int miniplayerID;
        int minienemyID;
        int boatID;
        int enemyID;
        int radarID;
        int swiperID;
        int blurID;
        int closeID;
        float textOffsetX;
        float textOffsetY;
        float textbigOffsetX;
        float textbigOffsetY;
        float textScale;
        float textbigScale;
        private float MapX, MapY;
        private float scaleMapX, scaleMapY;
        private float miniMapX, miniMapY;
        public void Alive(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }
        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            string currlevel = VI.GameState.GetName();
            string currlevelBG = currlevel + "Background";
            enemymapID = VI.Entity.GetId("enemymap", "EnemyMap");
            boatID = VI.Entity.GetId("Boat", currlevel);
            enemyID = VI.Entity.GetId("Enemy", currlevel);
            miniplayerID = VI.Entity.GetId("player", "EnemyMap");
            minienemyID = VI.Entity.GetId("enemy", "EnemyMap");
            radarID = VI.Entity.GetId("Radar", "EnemyMap");
            swiperID = VI.Entity.GetId("Swiper", "EnemyMap");
            blurID = VI.Entity.GetId("blur", "EnemyMap");
            MapX = VI.Transform.Scale.GetX(VI.Entity.GetId("Water", currlevelBG));
            MapY = VI.Transform.Scale.GetY(VI.Entity.GetId("Water", currlevelBG));
            closeID = VI.Entity.GetId("CloseBtn", "EnemyMap");
            textOffsetX = -42;// VI.Text.Offset.GetX(enemymapID);
            textOffsetY = -170;// VI.Text.Offset.GetY(enemymapID);
            textbigOffsetX = -120;
            textbigOffsetY = 360;
            textScale = VI.Text.Scale.Get(enemymapID);
            textbigScale = 1.0f;

            VI.Entity.SetActive(blurID, false);
            VI.Entity.SetActive(closeID, false);
            scaleMapX = MapX / 2;
            scaleMapY = MapY / 2;
            miniMapX = VI.Transform.Scale.GetX(enemymapID);
            miniMapY = VI.Transform.Scale.GetY(enemymapID);


        }

        public void FixedUpdate(int _ENTITY)
        {
            VI.Transform.Rotate.Set(swiperID, VI.Transform.Rotate.Get(swiperID)+0.01f);
            if (VI.Transform.Rotate.Get(swiperID) > 6.29)
                VI.Transform.Rotate.Set(swiperID, 0.0f);
        }
        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            
            if (!big && (VI.Input.Button.Released(enemymapID)))
            {
                big = true;
                VI.Entity.SetActive(closeID, true);
                PlayerScript.PlayerInOtherAnimation = true;
                VI.MovementAI.SetNext(enemymapID, 1);
                VI.Text.Offset.Set(enemymapID, textbigOffsetX, textbigOffsetY);
                VI.Text.Scale.Set(enemymapID, textbigScale);
                VI.MovementAI.Run(enemymapID);
                VI.Sprite.SetType(enemymapID, 0);
                scaleMapX = MapX;
                scaleMapY = MapY;
                VI.MovementAI.SetNext(miniplayerID, 1);
                VI.MovementAI.Run(miniplayerID);
                VI.MovementAI.SetNext(minienemyID, 1);
                VI.MovementAI.Run(minienemyID);
                VI.MovementAI.SetNext(radarID, 1);
                VI.MovementAI.Run(radarID);
                VI.MovementAI.SetNext(swiperID, 1);
                VI.MovementAI.Run(swiperID);
                VI.Entity.SetActive(blurID, true);
            }
            else if (big && (VI.Input.Mouse.Release(349)) && (!VI.Input.Button.Released(enemymapID)))
            {
                big = false;
                VI.Entity.SetActive(closeID, false);
                PlayerScript.PlayerInOtherAnimation = false;
                VI.MovementAI.SetNext(enemymapID, 0);
                VI.Text.Offset.Set(enemymapID, textOffsetX, textOffsetY);
                VI.Text.Scale.Set(enemymapID, textScale);
                VI.MovementAI.Run(enemymapID);
                VI.Sprite.SetType(enemymapID, 1);
                scaleMapX = MapX/2;
                scaleMapY = MapY/2;
                VI.MovementAI.SetNext(miniplayerID, 0);
                VI.MovementAI.Run(miniplayerID);
                VI.MovementAI.SetNext(minienemyID, 0);
                VI.MovementAI.Run(minienemyID);
                VI.MovementAI.SetNext(radarID, 0);
                VI.MovementAI.Run(radarID);
                VI.MovementAI.SetNext(swiperID, 0);
                VI.MovementAI.Run(swiperID);
                VI.Entity.SetActive(blurID, false);
            }
            float posx = VI.Transform.Position.GetX(enemyID) - VI.Transform.Position.GetX(boatID);
            float posy = VI.Transform.Position.GetY(enemyID) - VI.Transform.Position.GetY(boatID);
            float vecx = (posx * (VI.Transform.Scale.GetX(enemymapID) / scaleMapX));
            float vecy = (posy * (VI.Transform.Scale.GetX(enemymapID) / scaleMapY));
            float eposx = VI.Transform.Position.GetX(miniplayerID) + vecx;
            float eposy = VI.Transform.Position.GetY(miniplayerID) + vecy;
            VI.Transform.Position.SetX(minienemyID, eposx);
            VI.Transform.Position.SetY(minienemyID, eposy);
            if ((vecx * vecx + vecy * vecy) > (VI.Transform.Scale.GetX(enemymapID)/2 * VI.Transform.Scale.GetX(enemymapID)/2 - VI.Transform.Scale.GetX(minienemyID)* VI.Transform.Scale.GetX(minienemyID)))
            {
                VI.Entity.SetActive(minienemyID, false);
            }
            else
            { 
                VI.Entity.SetActive(minienemyID, true);
            }
        }

        public void Exit(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            big= false;
            PlayerScript.PlayerInOtherAnimation = false;
            VI.MovementAI.SetNext(enemymapID, 0);
            VI.Text.Offset.Set(enemymapID, textOffsetX, textOffsetY);
            VI.Text.Scale.Set(enemymapID, textScale);
            VI.MovementAI.Run(enemymapID);
            VI.Sprite.SetType(enemymapID, 1);
            scaleMapX = MapX / 2;
            scaleMapY = MapY / 2;
            VI.MovementAI.SetNext(miniplayerID, 0);
            VI.MovementAI.Run(miniplayerID);
            VI.MovementAI.SetNext(minienemyID, 0);
            VI.MovementAI.Run(minienemyID);
            VI.Entity.SetActive(blurID, false);
        }
        public void Dead(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }
    }
}
