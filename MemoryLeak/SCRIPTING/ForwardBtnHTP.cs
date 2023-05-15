/*!*****************************************************************************
\file ForwardBtnHTP.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
Controls the forward button
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;
using VI;

namespace BonVoyage {
    public class ForwardBtnHTP : BaseScript
    {
        int ControlsID;
        int PlayerMovementID;
        int DialogueID;
        int CrystalBallID;
        int WeatherID;
        int MemeoryFragmentID;
        int EnemyID;
        int BackBtnID;
        int ForwardBtnID;
        int BoatID;
        //bool init = false;
        int c1;
        int c2;
        int c3;
        int c4;
        int pm1;
        int pm2;
        int pm3;
        int pm4;
        int pm5;
        int pm6;
        int pm7;
        int d1;
        int d2;
        int d3;
        int d4;
        int d5;
        int d6;
        int d7;
        int d8;
        int cb1;
        int cb2;
        int cb3;
        int cb4;
        int cb5;
        int cb6;
        int w1;
        int w2;
        int w3;
        int w4;
        int w5;
        int w6;
        int w7;
        int w8;
        int w9;
        int w10;
        int mf1;
        int mf2;
        int mf3;
        int mf4;
        int e1;
        int e2;
        int e3;
        int e4;
        int e5;
        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            
        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

            BackBtnID = VI.Entity.GetId("BackBtnHTP", "How_To_Play");
            ForwardBtnID = VI.Entity.GetId("ForwardBtnHTP", "How_To_Play");
            ControlsID = VI.Entity.GetId("Controls", "How_To_Play");
            PlayerMovementID = VI.Entity.GetId("PlayerMovement", "How_To_Play");
            DialogueID = VI.Entity.GetId("Dialogue", "How_To_Play");
            CrystalBallID = VI.Entity.GetId("CrystalBall", "How_To_Play");
            WeatherID = VI.Entity.GetId("Weather", "How_To_Play");
            MemeoryFragmentID = VI.Entity.GetId("MemeoryFragment", "How_To_Play");
            EnemyID = VI.Entity.GetId("Enemy", "How_To_Play");
            BoatID = VI.Entity.GetId("Boat", "UIBackground");
            c1 = VI.Entity.GetId("C1", "How_To_Play");
            c2 = VI.Entity.GetId("C2", "How_To_Play");
            c3 = VI.Entity.GetId("C3", "How_To_Play");
            c4 = VI.Entity.GetId("C4", "How_To_Play");
            pm1 = VI.Entity.GetId("PM1", "How_To_Play");
            pm2 = VI.Entity.GetId("PM2", "How_To_Play");
            pm3 = VI.Entity.GetId("PM3", "How_To_Play");
            pm4 = VI.Entity.GetId("PM4", "How_To_Play");
            pm5 = VI.Entity.GetId("PM5", "How_To_Play");
            pm6 = VI.Entity.GetId("PM6", "How_To_Play");
            pm7 = VI.Entity.GetId("PM7", "How_To_Play");
            d1 = VI.Entity.GetId("D1", "How_To_Play");
            d2 = VI.Entity.GetId("D2", "How_To_Play");
            d3 = VI.Entity.GetId("D3", "How_To_Play");
            d4 = VI.Entity.GetId("D4", "How_To_Play");
            d5 = VI.Entity.GetId("D5", "How_To_Play");
            d6 = VI.Entity.GetId("D6", "How_To_Play");
            d7 = VI.Entity.GetId("D7", "How_To_Play");
            d8 = VI.Entity.GetId("D8", "How_To_Play");
            cb1 = VI.Entity.GetId("CB1", "How_To_Play");
            cb2 = VI.Entity.GetId("CB2", "How_To_Play");
            cb3 = VI.Entity.GetId("CB3", "How_To_Play");
            cb4 = VI.Entity.GetId("CB4", "How_To_Play");
            cb5 = VI.Entity.GetId("CB5", "How_To_Play");
            cb6 = VI.Entity.GetId("CB6", "How_To_Play");
            w1 = VI.Entity.GetId("W1", "How_To_Play");
            w2 = VI.Entity.GetId("W2", "How_To_Play");
            w3 = VI.Entity.GetId("W3", "How_To_Play");
            w4 = VI.Entity.GetId("W4", "How_To_Play");
            w5 = VI.Entity.GetId("W5", "How_To_Play");
            w6 = VI.Entity.GetId("W6", "How_To_Play");
            w7 = VI.Entity.GetId("W7", "How_To_Play");
            w8 = VI.Entity.GetId("W8", "How_To_Play");
            w9 = VI.Entity.GetId("W9", "How_To_Play");
            w10 = VI.Entity.GetId("W10", "How_To_Play");
            mf1 = VI.Entity.GetId("MF1", "How_To_Play");
            mf2 = VI.Entity.GetId("MF2", "How_To_Play");
            mf3 = VI.Entity.GetId("MF3", "How_To_Play");
            mf4 = VI.Entity.GetId("MF4", "How_To_Play");
            e1 = VI.Entity.GetId("E1", "How_To_Play");
            e2 = VI.Entity.GetId("E2", "How_To_Play");
            e3 = VI.Entity.GetId("E3", "How_To_Play");
            e4 = VI.Entity.GetId("E4", "How_To_Play");
            e5 = VI.Entity.GetId("E5", "How_To_Play");
            //init = true;
            VI.Entity.SetActive(w9, false);
            VI.Entity.SetActive(w10, false);

            //init = false;
            VI.MovementAI.SetNext(c1, 0);
            VI.MovementAI.SetNext(c2, 0);
            VI.MovementAI.SetNext(c3, 0);
            VI.MovementAI.SetNext(c4, 0);
            VI.MovementAI.SetNext(pm1, 0);
            VI.MovementAI.SetNext(pm2, 0);
            VI.MovementAI.SetNext(pm3, 0);
            VI.MovementAI.SetNext(pm4, 0);
            VI.MovementAI.SetNext(pm5, 0);
            VI.MovementAI.SetNext(pm6, 0);
            VI.MovementAI.SetNext(pm7, 0);
            VI.MovementAI.SetNext(d1, 0);
            VI.MovementAI.SetNext(d2, 0);
            VI.MovementAI.SetNext(d3, 0);
            VI.MovementAI.SetNext(d4, 0);
            VI.MovementAI.SetNext(d5, 0);
            VI.MovementAI.SetNext(d6, 0);
            VI.MovementAI.SetNext(d7, 0);
            VI.MovementAI.SetNext(d8, 0);
            VI.MovementAI.SetNext(cb1, 0);
            VI.MovementAI.SetNext(cb2, 0);
            VI.MovementAI.SetNext(cb3, 0);
            VI.MovementAI.SetNext(cb4, 0);
            VI.MovementAI.SetNext(cb5, 0);
            VI.MovementAI.SetNext(cb6, 0);
            VI.MovementAI.SetNext(w1, 0);
            VI.MovementAI.SetNext(w2, 0);
            VI.MovementAI.SetNext(w3, 0);
            VI.MovementAI.SetNext(w4, 0);
            VI.MovementAI.SetNext(w5, 0);
            VI.MovementAI.SetNext(w6, 0);
            VI.MovementAI.SetNext(w7, 0);
            VI.MovementAI.SetNext(w8, 0);
            VI.MovementAI.SetNext(mf1, 0);
            VI.MovementAI.SetNext(mf2, 0);
            VI.MovementAI.SetNext(mf3, 0);
            VI.MovementAI.SetNext(mf4, 0);
            VI.MovementAI.SetNext(e1, 0);
            VI.MovementAI.SetNext(e2, 0);
            VI.MovementAI.SetNext(e3, 0);
            VI.MovementAI.SetNext(e4, 0);
            VI.MovementAI.SetNext(e5, 0);
            VI.Text.Update(d4, "Yes!");
            VI.Text.Update(d5, "No...");
            VI.Text.Update(d6, "Can you bring me home?");
        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Update(int _ENTITY) {
            //if (!init)
            //{
            //    Alive(_ENTITY);
                
            //    init = true;
            //}
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            BackBtnID = VI.Entity.GetId("BackBtnHTP", "How_To_Play");
            ForwardBtnID = VI.Entity.GetId("ForwardBtnHTP", "How_To_Play");
            ControlsID = VI.Entity.GetId("Controls", "How_To_Play");
            BoatID = VI.Entity.GetId("Boat", "UIBackground");
            w3 = VI.Entity.GetId("W3", "How_To_Play");
            w4 = VI.Entity.GetId("W4", "How_To_Play");
            w5 = VI.Entity.GetId("W5", "How_To_Play");
            w6 = VI.Entity.GetId("W6", "How_To_Play");
            w7 = VI.Entity.GetId("W7", "How_To_Play");
            w8 = VI.Entity.GetId("W8", "How_To_Play");
            w9 = VI.Entity.GetId("W9", "How_To_Play");
            w10 = VI.Entity.GetId("W10", "How_To_Play");
            d4 = VI.Entity.GetId("D4", "How_To_Play");
            d5 = VI.Entity.GetId("D5", "How_To_Play");
            d6 = VI.Entity.GetId("D6", "How_To_Play");
            cb3 = VI.Entity.GetId("CB3", "How_To_Play");
            cb4 = VI.Entity.GetId("CB4", "How_To_Play");
            cb5 = VI.Entity.GetId("CB5", "How_To_Play");
            cb6 = VI.Entity.GetId("CB6", "How_To_Play");
            VI.Entity.SetActive(w9, false);
            VI.Entity.SetActive(w10, false);
            if (VI.Input.Button.Released(ForwardBtnID))
            {
                Alive(_ENTITY);
                int i = VI.MovementAI.GetCurrentIndex(ControlsID) + 1;
                VI.MovementAI.SetNext(ControlsID, i);
                VI.MovementAI.SetNext(PlayerMovementID, i);
                VI.MovementAI.SetNext(DialogueID, i);
                VI.MovementAI.SetNext(CrystalBallID, i);
                VI.MovementAI.SetNext(WeatherID, i);
                VI.MovementAI.SetNext(MemeoryFragmentID, i);
                VI.MovementAI.SetNext(EnemyID, i);
                VI.MovementAI.SetNext(c1, i);
                VI.MovementAI.SetNext(c2, i);
                VI.MovementAI.SetNext(c3, i);
                VI.MovementAI.SetNext(c4, i);
                VI.MovementAI.SetNext(pm1, i);
                VI.MovementAI.SetNext(pm2, i);
                VI.MovementAI.SetNext(pm3, i);
                VI.MovementAI.SetNext(pm4, i);
                VI.MovementAI.SetNext(pm5, i);
                VI.MovementAI.SetNext(pm6, i);
                VI.MovementAI.SetNext(pm7, i);
                VI.MovementAI.SetNext(d1, i);
                VI.MovementAI.SetNext(d2, i);
                VI.MovementAI.SetNext(d3, i);
                VI.MovementAI.SetNext(d4, i);
                VI.MovementAI.SetNext(d5, i);
                VI.MovementAI.SetNext(d6, i);
                VI.MovementAI.SetNext(d7, i);
                VI.MovementAI.SetNext(d8, i);
                VI.MovementAI.SetNext(cb1, i);
                VI.MovementAI.SetNext(cb2, i);
                VI.MovementAI.SetNext(cb3, i);
                VI.MovementAI.SetNext(cb4, i);
                VI.MovementAI.SetNext(cb5, i);
                VI.MovementAI.SetNext(cb6, i);
                VI.MovementAI.SetNext(w1, i);
                VI.MovementAI.SetNext(w2, i);
                VI.MovementAI.SetNext(w3, i);
                VI.MovementAI.SetNext(w4, i);
                VI.MovementAI.SetNext(w5, i);
                VI.MovementAI.SetNext(w6, i);
                VI.MovementAI.SetNext(w7, i);
                VI.MovementAI.SetNext(w8, i);
                VI.MovementAI.SetNext(mf1, i);
                VI.MovementAI.SetNext(mf2, i);
                VI.MovementAI.SetNext(mf3, i);
                VI.MovementAI.SetNext(mf4, i);
                VI.MovementAI.SetNext(e1, i);
                VI.MovementAI.SetNext(e2, i);
                VI.MovementAI.SetNext(e3, i);
                VI.MovementAI.SetNext(e4, i);
                VI.MovementAI.SetNext(e5, i);


                VI.MovementAI.Run(ControlsID);
                VI.MovementAI.Run(PlayerMovementID);
                VI.MovementAI.Run(DialogueID);
                VI.MovementAI.Run(CrystalBallID);
                VI.MovementAI.Run(WeatherID);
                VI.MovementAI.Run(MemeoryFragmentID);
                VI.MovementAI.Run(EnemyID);


                VI.MovementAI.Run(c1);
                VI.MovementAI.Run(c2);
                VI.MovementAI.Run(c3);
                VI.MovementAI.Run(c4);
                VI.MovementAI.Run(pm1);
                VI.MovementAI.Run(pm2);
                VI.MovementAI.Run(pm3);
                VI.MovementAI.Run(pm4);
                VI.MovementAI.Run(pm5);
                VI.MovementAI.Run(pm6);
                VI.MovementAI.Run(pm7);
                VI.MovementAI.Run(d1);
                VI.MovementAI.Run(d2);
                VI.MovementAI.Run(d3);
                VI.MovementAI.Run(d4);
                VI.MovementAI.Run(d5);
                VI.MovementAI.Run(d6);
                VI.MovementAI.Run(d7);
                VI.MovementAI.Run(d8);
                VI.MovementAI.Run(cb1);
                VI.MovementAI.Run(cb2);
                VI.MovementAI.Run(cb3);
                VI.MovementAI.Run(cb4);
                VI.MovementAI.Run(cb5);
                VI.MovementAI.Run(cb6);
                VI.MovementAI.Run(w1);
                VI.MovementAI.Run(w2);
                VI.MovementAI.Run(w3);
                VI.MovementAI.Run(w4);
                VI.MovementAI.Run(w5);
                VI.MovementAI.Run(w6);
                VI.MovementAI.Run(w7);
                VI.MovementAI.Run(w8);
                VI.MovementAI.Run(mf1);
                VI.MovementAI.Run(mf2);
                VI.MovementAI.Run(mf3);
                VI.MovementAI.Run(mf4);
                VI.MovementAI.Run(e1);
                VI.MovementAI.Run(e2);
                VI.MovementAI.Run(e3);
                VI.MovementAI.Run(e4);
                VI.MovementAI.Run(e5);
            }
            else
            {
                if (VI.MovementAI.GetCurrentIndex(ControlsID) == 3)
                {
                    if (VI.Input.Button.Released(cb3))
                    {
                        if (VI.Animation.SpriteSheet.SheetIndex.Get(cb4) < 2)
                        {
                            VI.Animation.SpriteSheet.SheetIndex.Set(cb4, VI.Animation.SpriteSheet.SheetIndex.Get(cb4) + 1);
                            VI.Animation.SpriteSheet.SheetIndex.Set(cb5, VI.Animation.SpriteSheet.SheetIndex.Get(cb5) + 1);
                            VI.Animation.SpriteSheet.SheetIndex.Set(cb6, VI.Animation.SpriteSheet.SheetIndex.Get(cb6) + 1);
                        }
                        else
                        {
                            VI.Animation.SpriteSheet.SheetIndex.Set(cb4, 0);
                            VI.Animation.SpriteSheet.SheetIndex.Set(cb5, 0);
                            VI.Animation.SpriteSheet.SheetIndex.Set(cb6, 0);
                        }
                    }
                }
                if (VI.MovementAI.GetCurrentIndex(ControlsID) == 4)
                {
                    if (VI.Input.Button.Hover(w3) || VI.Input.Button.Hover(w4))
                    {
                        VI.Entity.SetActive(w9, true);
                    }
                    if (VI.Input.Button.Hover(w5) || VI.Input.Button.Hover(w6))
                    {
                        if (VI.GameState.GetName() == "Menu")
                        { 
                            BoatID = VI.Entity.GetId("Boat", "UIBackground");
                            VI.MovementAI.Run(BoatID);
                        }
                    }
                    if (VI.Input.Button.Hover(w7) || VI.Input.Button.Hover(w8))
                    {
                        VI.Entity.SetActive(w10, true);
                    }
                }
                else if (VI.MovementAI.GetCurrentIndex(ControlsID) == 2)
                { 
                    if (VI.Input.Button.Released(d4))
                    {
                        VI.Text.Update(d4, "Welcome!");
                        VI.Text.Update(d5, "Wait I cant");
                        VI.Text.Update(d6, "Thank you so much!!");
                    }
                    else if (VI.Input.Button.Released(d5))
                    {
                        VI.Text.Update(d4, "Ok then");
                        VI.Text.Update(d5, "Idk How");
                        VI.Text.Update(d6, "Why not? I miss home..");
                    }
                }
            }
            
            if (VI.MovementAI.GetCurrentIndex(ControlsID) == 0)
            {
                VI.Entity.SetActive(BackBtnID, false);
            }
            else if (VI.MovementAI.GetCurrentIndex(ControlsID) == 1)
            {
                VI.Text.Update(d4, "Yes!");
                VI.Text.Update(d5, "No...");
                VI.Text.Update(d6, "Can you bring me home?");
            }
            else
                VI.Entity.SetActive(BackBtnID, true);
        }

        public void FixedUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        public void LateUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Exit(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            //init = false;
        }

        public void Dead(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }
    }
}
