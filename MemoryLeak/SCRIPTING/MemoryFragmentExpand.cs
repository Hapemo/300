/*!*****************************************************************************
\file MemoryFragmentExpand.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 01-03-2023
\brief
This file contains scripting functions for logic system for fragments.
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;
using VI;

namespace BonVoyage {
    public class MemoryFragmentExpand : BaseScript
    {
        static int[] relics;
        static int[] backs;
        static int[] activated;
        static int[] tooltips;
        static bool activatedChanged;
        static int[] expandedRelics;
        int expanded;
        bool showing;
        int toggleMap;
        static int[] relicBIG;
        static int[] relicBIGLOST;
        static int relicFound;
        static int relicNotFound;
        static int relicFoundSound;
        static int relicNotFoundSound;
        private float ZoomScaleFactorX;
        private float ZoomScaleFactorY;
        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            
        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            relicFoundSound = VI.Entity.GetId("PassengerIcon1");
            relicNotFoundSound = VI.Entity.GetId("PassengerIcon2");

            relics = new int[]
            {
                VI.Entity.GetId("Relic1"),
                VI.Entity.GetId("Relic2"),
                VI.Entity.GetId("Relic3"),
                VI.Entity.GetId("Relic4"),
                VI.Entity.GetId("Relic5"),
                VI.Entity.GetId("Relic6")
            };
            backs = new int[]
            {
                VI.Entity.GetId("Back1"),
                VI.Entity.GetId("Back2"),
                VI.Entity.GetId("Back3"),
                VI.Entity.GetId("Back4"),
                VI.Entity.GetId("Back5"),
                VI.Entity.GetId("Back6")
            };
            tooltips = new int[]
            {
                VI.Entity.GetId("tooltip1"),
                VI.Entity.GetId("tooltip2"),
                VI.Entity.GetId("tooltip3"),
                VI.Entity.GetId("tooltip4"),
                VI.Entity.GetId("tooltip5"),
                VI.Entity.GetId("tooltip6"),
                VI.Entity.GetId("tooltipNO")
            };
            if (VI.Data.GetData1(0) == -1)
            {
                for (int i = 0; i < 6; ++i)
                    VI.Data.SetData1(i, 0);
            }    

            activated = new int[] { VI.Data.GetData1(0), VI.Data.GetData1(1), VI.Data.GetData1(2), 
                                        VI.Data.GetData1(3), VI.Data.GetData1(4), VI.Data.GetData1(5) };
            expandedRelics = new int[]
            {
                VI.Entity.GetId("Relic1Expanded"),
                VI.Entity.GetId("Relic2Expanded"),
                VI.Entity.GetId("Relic3Expanded"),
                VI.Entity.GetId("Relic4Expanded"),
                VI.Entity.GetId("Relic5Expanded"),
                VI.Entity.GetId("Relic6Expanded")
            };
            expanded = VI.Entity.GetId("memoryfragmentscreen");
            showing = false;
            activatedChanged = false;
            toggleMap = VI.Entity.GetId("toggleMap");
            relicBIG = new int[]
            {
                VI.Entity.GetId("Relic1BIG"),
                VI.Entity.GetId("Relic2BIG"),
                VI.Entity.GetId("Relic3BIG"),
                VI.Entity.GetId("Relic4BIG"),
                VI.Entity.GetId("Relic5BIG"),
                VI.Entity.GetId("Relic6BIG")
            };
            relicBIGLOST = new int[]
            {
                VI.Entity.GetId("Relic1BIGLOST"),
                VI.Entity.GetId("Relic2BIGLOST"),
                VI.Entity.GetId("Relic3BIGLOST"),
                VI.Entity.GetId("Relic4BIGLOST"),
                VI.Entity.GetId("Relic5BIGLOST"),
                VI.Entity.GetId("Relic6BIGLOST")
            };
            relicFound = VI.Entity.GetId("RelicFound");
            relicNotFound = VI.Entity.GetId("RelicLost");
        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            //make button toggle showing
            //activate every frame
            if (THIS.Input.Button.Clicked())
                showing = !showing;

            if (showing)
            {
                ActivateFragMenu();
                
            }
            else
            {
                DeactivateFragMenu();
            }
            if (showing)
            {
                bool nonehovering = true;
                for (int i = 0; i < 6; ++i)
                { 
                    if (VI.Input.Button.Hover(backs[i]))
                    {
                        nonehovering = false;
                        ZoomScaleFactorX = VI.Camera.GetScale.X() / VI.Window.GetScreenWidth();
                        ZoomScaleFactorY = VI.Camera.GetScale.Y() / VI.Window.GetScreenHeight();
                        if (activated[i] == 1)
                        {
                            VI.Entity.Activate(tooltips[i]);
                            VI.Entity.Deactivate(tooltips[6]);
                            VI.Transform.Position.SetX(tooltips[i], ZoomScaleFactorX * (VI.Input.Mouse.WorldPosX() + VI.Transform.Scale.GetX(tooltips[i]) / 2 + 60));
                            VI.Transform.Position.SetY(tooltips[i],  ZoomScaleFactorY * (VI.Input.Mouse.WorldPosY() - VI.Transform.Scale.GetY(tooltips[i]) / 2));
                        }
                        else
                        {
                            VI.Entity.Deactivate(tooltips[i]);
                            VI.Entity.Activate(tooltips[6]);
                            VI.Transform.Position.SetX(tooltips[6], ZoomScaleFactorX * (VI.Input.Mouse.WorldPosX() + VI.Transform.Scale.GetX(tooltips[i]) / 2 + 60));
                            VI.Transform.Position.SetY(tooltips[6], ZoomScaleFactorY * (VI.Input.Mouse.WorldPosY() - VI.Transform.Scale.GetY(tooltips[i]) / 2));
                        }

                    }
                    else
                    {
                        VI.Entity.Deactivate(tooltips[i]);
                    }
                }
                if (nonehovering)
                    VI.Entity.Deactivate(tooltips[6]);

                for (int i = 0; i < 6; ++i)
                { 
                    if (VI.Input.Button.Clicked(backs[i]) && activated[i] == 1)
                    {
                        string gsname = VI.GameState.GetName();

                        VI.Scene.Pause(gsname);
                        VI.Scene.Pause(gsname + "Background");
                        VI.Scene.Pause("WeatherScene");
                        VI.Scene.SetForceRender(gsname, true);
                        VI.Scene.SetForceRender(gsname + "Background", true);
                        VI.Scene.SetForceRender("WeatherScene", true);

                        VI.Scene.Pause("GUI Scene");
                        VI.Scene.Pause("TutorialUILvl" + gsname[gsname.Length - 1]);
                        VI.Scene.Pause("L" + gsname[gsname.Length - 1] + "NameFade");

                        VI.Entity.Deactivate(toggleMap);

                        VI.Scene.Play("GUI Scene Expanded");
                        VI.Entity.Activate(expandedRelics[i]);

                    }
                }
            }
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            if (activatedChanged) { 
                activatedChanged = false;
                VI.ParticleSystem.GenerateLoop(VI.Entity.GetId("memoryfragment"), 5.0f);
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
            DeactivateFragMenu();
        }

        public void Dead(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        private void ActivateFragMenu()
        {
            foreach (int back in backs)
                VI.Entity.Activate(back);
            for (int i = 0; i < 6; ++ i)
            {
                if (activated[i] == 1)
                    VI.Entity.Activate(relics[i]);
            }
            VI.Entity.Activate(expanded);
            showing = true;
        }

        private void DeactivateFragMenu()
        {
            foreach (int back in backs)
                VI.Entity.Deactivate(back);
            foreach (int relic in relics)
                VI.Entity.Deactivate(relic);
            VI.Entity.Deactivate(expanded);
            showing = false;
        }

        public static void ActivateFragment(int fragmentId)
        {
            if (fragmentId < 0) return;
            if (fragmentId > 5) return;
            activatedChanged = true;
            activated[fragmentId] = 1;
            VI.Data.SetData1(fragmentId, 1);
        }

        public static void DeliveredPassenger(int fragmentId, bool correctly)
        {
            if (fragmentId < 0) return;
            if (fragmentId > 5) return;
            if (correctly)
            {
                VI.Entity.Activate(relicBIG[fragmentId]);
                VI.Entity.Activate(relicFound);
                VI.ColorAI.StartAnimation(relicFound);
                VI.ColorAI.SetNextStep(relicFound, 1);
                VI.Audio.Play(relicFoundSound);

            }
            else
            {
                VI.Entity.Activate(relicBIGLOST[fragmentId]);
                VI.Entity.Activate(relicNotFound);
                VI.ColorAI.StartAnimation(relicNotFound);
                VI.ColorAI.SetNextStep(relicNotFound, 1);
                VI.Audio.Play(relicNotFoundSound);

            }
        }
    }
}
