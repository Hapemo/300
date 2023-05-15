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
        static bool big = false;
        static bool active = true;

        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY) {
            VI.Entity.s_Deactivate("weathermap", "WeatherMap");
            VI.Entity.s_Deactivate("minimap", "WeatherMap");
            VI.Entity.s_Deactivate("enemymap", "WeatherMap");
            VI.Entity.s_Deactivate("Ball", "WeatherMap");
        }

        public void Update(int _ENTITY)
        {
            if (active)
            {
                VI.Entity.s_Activate("Ball", "WeatherMap");
                if ((VI.Input.Button.s_Released("cyclemap", "WeatherMap")))
                {
                    toggle = toggle >= 2 ? 0 : (toggle + 1);
                }
                if ((VI.Input.Button.s_Released("Ball", "WeatherMap")))
                {
                    big = !big;
                    Console.WriteLine("toggled size!");
                }
                if (prevTog != toggle)
                { 
                    if (toggle == 0)
                    {
                        VI.Entity.s_Deactivate("weathermap", "WeatherMap");
                        VI.Entity.s_Deactivate("enemymap", "WeatherMap");
                        VI.Entity.s_Activate("minimap", "WeatherMap");
                    }
                    else if (toggle == 1)
                    {
                        VI.Entity.s_Deactivate("minimap", "WeatherMap");
                        VI.Entity.s_Deactivate("enemymap", "WeatherMap");
                        VI.Entity.s_Activate("weathermap", "WeatherMap");
                    }
                    else if (toggle == 2)
                    {
                        VI.Entity.s_Deactivate("minimap", "WeatherMap");
                        VI.Entity.s_Deactivate("weathermap", "WeatherMap");
                        VI.Entity.s_Activate("enemymap", "WeatherMap");
                    }
                    else
                    {
                        VI.Entity.s_Deactivate("minimap", "WeatherMap");
                        VI.Entity.s_Deactivate("enemymap", "WeatherMap");
                        VI.Entity.s_Deactivate("weathermap", "WeatherMap");
                    }
                    prevTog = toggle;
                }
            }
            else
            {
                VI.Entity.s_Deactivate("minimap", "WeatherMap");
                VI.Entity.s_Deactivate("enemymap", "WeatherMap");
                VI.Entity.s_Deactivate("weathermap", "WeatherMap");
            }

        }

        public static bool Big()
        {
            return big;
        }

        public static void TurnOff()
        {
            active = false;
        }

        public static void TurnOn()
        {
            active = true;
        }

        public void FixedUpdate(int _ENTITY) {

        }
        public void Exit(int _ENTITY) {
            toggle=0;
            prevTog = -1;
            VI.Entity.s_Deactivate("minimap", "WeatherMap");
            VI.Entity.s_Deactivate("enemymap", "WeatherMap");
            VI.Entity.s_Deactivate("weathermap", "WeatherMap");
            VI.Entity.s_Deactivate("Ball", "Dialogue");
        }
        public void Dead(int _ENTITY) {

        }
    }
}
