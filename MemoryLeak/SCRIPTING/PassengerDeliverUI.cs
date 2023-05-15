/*!*****************************************************************************
\file PassengerDeliverUI.cs
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 10-03-2023
\brief
Script to control the UI when passenger is delivered to location
*******************************************************************************/
using System;
using System.Runtime.CompilerServices;

namespace BonVoyage {
    public class PassengerDeliverUI : BaseScript
    {
        static int p1icon, p2icon;
        static bool increase1, increase2;
        static float curr1, curr2;
        static float speed;
        public void Alive(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        }

        public void Init(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            p1icon = VI.Entity.GetId("PassengerIcon1");
            p2icon = VI.Entity.GetId("PassengerIcon2");
            curr1 = curr2 = 0;
            increase1 = false;
            increase2 = false;
            speed = 250;
            VI.Sprite.SetColor(p1icon, 0,0,0, 255);
            VI.Sprite.SetColor(p2icon, 0,0,0, 255);

        }

        public void EarlyUpdate(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
        
        }

        public void Update(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!
            if (VI.Input.Key.Release(258))
                DeliveredPassenger();
            if ((curr1 < 255 && increase1) || (curr2 < 255 && increase2))
                UpdateColor();

            if (VI.Input.Key.Hold(340) && VI.Input.Key.Hold(341))
            {
                if (VI.Input.Key.Press(257))
                {
                    DeliveredPassenger();
                }
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
            curr1 = curr2 = 0;
            increase1 = false;
            increase2 = false;
            speed = 250;
        }

        public void Dead(int _ENTITY) {
            THIS.StoreId(_ENTITY); // DO NOT REMOVE!!!

        }

        static private void UpdateColor()
        {
            if (increase1)
            {
                curr1 += speed * (float)VI.General.DeltaTime();
                if (curr1 > 255)
                    curr1 = 255;
                VI.Sprite.SetColor(p1icon, (int)curr1, (int)curr1, (int)curr1, 255);
            }
            if (increase2)
            {
                curr2 += speed * (float)VI.General.DeltaTime();
                if (curr2 > 255)
                    curr2 = 255;
                VI.Sprite.SetColor(p2icon, (int)curr2, (int)curr2, (int)curr2, 255);
            }
        }

        static public void DeliveredPassenger()
        {
            if (!increase1)
                increase1 = true;
            else if (!increase2)
                increase2 = true;
        }

        static public bool DelieveredAllPassengers()
        {
            return increase1 && increase2;
        }
    }
}
