/*!*****************************************************************************
\file CrystalBallScript.cs
\author Huang Wei Jhin
\par DP email: h.weijhin\@digipen.edu
\par Group: Memory Leak Studios
\date 28-01-2023
\brief
CrystalBallScript contains multiple script components to run the Level1 of the game.
Part of which are:
- Toggling of the mini maps
    -Minimap
    -WeatherMap
    -EmemyMap
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    
    public class CrystalBallScript
    {
        int toggle = 0; // 0 minimap, 1, weathermap, 2 enemymap
        int prevTog = -1;
        //static bool big = false;
        //static bool active = true;
        int playerID;
        int rainID;
        int fogID;
        int fog2ID;
        int puddleID;
        int windID;
        int crystalBallID;
        int toggleMapID;
        int minimapID;
        int weathermapID;
        int enemymapID;
        string currlevel;
        string currlevelBG;
        public void Alive(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            currlevel = VI.GameState.GetName();
            currlevelBG = currlevel + "Background";
            //if (currlevel == "Level1")
            crystalBallID = VI.Entity.GetId("crystalBall", "CrystalBalls");
            toggleMapID = VI.Entity.GetId("toggleMap", "CrystalBalls");
            minimapID = VI.Entity.GetId("minimap", "MiniMap");
            weathermapID = VI.Entity.GetId("weathermap", "WeatherMap");
            enemymapID = VI.Entity.GetId("enemymap", "EnemyMap");
            VI.Entity.Activate(crystalBallID);
            VI.Audio.PlayBGM("Bon_Voyage_BGM");

            playerID = VI.Entity.GetId("Boat");
            rainID = VI.Entity.GetId("rain");
            puddleID = VI.Entity.GetId("rainpuddle");
            windID = VI.Entity.GetId("wind");
            fogID = VI.Entity.GetId("fog");
            fog2ID = VI.Entity.GetId("fogSmall");
            toggle = 0;
            prevTog = -1;
            VI.Entity.Activate(crystalBallID);
        }

        public void Update(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            if ((VI.Input.Button.Released(toggleMapID)) == true && !WeatherMapScript.big && !EnemyMapScript.big && !MiniMapScript.big)
            {
                toggle = toggle >= 2 ? 0 : (toggle + 1);
                if (currlevel == "Level1" && toggle == 2)
                    toggle = 0;
            }
            if(prevTog != toggle)
            { 
                if (toggle == 0)
                {
                    VI.Entity.Deactivate(weathermapID);
                    VI.Entity.Deactivate(enemymapID);
                    VI.Scene.Play("MiniMap");
                    VI.Entity.Activate(minimapID);
                    VI.Scene.Pause("WeatherMap");
                    VI.Scene.Pause("EnemyMap");
                    VI.Entity.Activate(crystalBallID);

                }
                else if (toggle == 1)
                {
                    VI.Entity.Deactivate(minimapID);
                    VI.Entity.Deactivate(enemymapID);
                    VI.Scene.Pause("MiniMap");
                    VI.Scene.Play("WeatherMap");
                    VI.Entity.Activate(weathermapID);
                    VI.Scene.Pause("EnemyMap");
                    VI.Entity.Activate(crystalBallID);


                }
                else if (toggle == 2)
                {
                    VI.Entity.Deactivate(minimapID);
                    VI.Entity.Deactivate(weathermapID);
                    VI.Scene.Pause("MiniMap");
                    VI.Scene.Pause("WeatherMap");
                    VI.Scene.Play("EnemyMap");
                    VI.Entity.Activate(enemymapID);
                    VI.Entity.Activate(crystalBallID);

                }
                else //if (toggle == 3)
                {
                    VI.Entity.Deactivate(minimapID);
                    VI.Entity.Deactivate(enemymapID);
                    VI.Entity.Deactivate(weathermapID);
                    VI.Scene.Pause("MiniMap");
                    VI.Scene.Pause("WeatherMap");
                    VI.Scene.Pause("EnemyMap");
                    VI.Entity.Deactivate(crystalBallID);
                }
                prevTog = toggle;
            }
            int index = VI.Weather.GetCurrent(12, VI.Transform.Position.GetY(playerID), VI.Transform.Position.GetY(playerID));
            if(index == 1 || index == 3 || index == 5 || index == 7)
            {
                //LOG.WRITE("Raining");
                VI.Transform.Position.SetX(rainID, VI.Transform.Position.GetX(playerID));
                VI.Transform.Position.SetY(rainID, VI.Transform.Position.GetY(playerID));
                VI.ParticleSystem.GenerateLoop(rainID, 1.0f);
                VI.Transform.Position.SetX(puddleID, VI.Transform.Position.GetX(playerID));
                VI.Transform.Position.SetY(puddleID, VI.Transform.Position.GetY(playerID));
                VI.ParticleSystem.GenerateLoop(puddleID, 1.0f);
            }

            if (index == 4 || index == 5 || index == 6 || index == 7)
            {
                //VI.Entity.SetActive(fogID, true);
                //LOG.WRITE("Foggy");
                VI.Transform.Position.SetX(fogID, VI.Transform.Position.GetX(playerID));
                VI.Transform.Position.SetY(fogID, VI.Transform.Position.GetY(playerID));
                VI.ParticleSystem.GenerateOnce(fogID);
                VI.Transform.Position.SetX(fog2ID, VI.Transform.Position.GetX(playerID));
                VI.Transform.Position.SetY(fog2ID, VI.Transform.Position.GetY(playerID));
                VI.ParticleSystem.GenerateOnce(fog2ID);
            }

            if (index == 2 || index == 3 || index == 6 || index == 7)
            {
                //VI.Entity.SetActive(fogID, true);
                //LOG.WRITE("WINDY");
                VI.Transform.Position.SetX(windID, VI.Transform.Position.GetX(playerID));
                VI.Transform.Position.SetY(windID, VI.Transform.Position.GetY(playerID));
                VI.ParticleSystem.GenerateOnce(windID);
            }

        }
        public void Exit(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            toggle=0;
            prevTog = -1;
            VI.Entity.Deactivate(crystalBallID);
            VI.Scene.Pause("MiniMap");
            VI.Scene.Pause("WeatherMap");
            VI.Scene.Pause("EnemyMap");
        }
        public void Dead(int _ENTITY)
        {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            VI.Audio.StopBGM();
        }
    }
}
