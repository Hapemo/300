using System;
using System.Runtime.CompilerServices;

namespace BonVoyage
{
    public class TestMonoScript
    {
        private bool intro; // player start
        private bool introdone;

        private bool encounter1; // meet girl
        private bool encounter1done;

        private bool encounter2; // meet pasenger
        private bool encounter2done;

        private bool encounter3; // deliver passenger
        private bool encounter3done;

        private int clickcount;

        private string hello;

        public void Init()
        {
            intro = true;
            introdone = false;

            encounter1 = false;
            encounter1done = false;

            encounter2 = false;
            encounter2done = false;

            encounter3 = false;
            encounter3done = false;

            clickcount = 0;

            // Loading dialogue file
            InternalCalls.LoadDialogs("Dialogue LittleGirl 0");
        }

        public void Update()
        {
            //if (InternalCalls.EntitiesCollided("Boat", "LittleGirlBox", "Level1")) { encounter1 = true; }
            //if (InternalCalls.EntitiesCollided("Boat", "PassengerBox", "Level1")) { encounter2 = true; }
            //if (InternalCalls.EntitiesCollided("PassengerBox", "PassengerDeliver", "Level1")) { encounter3 = true; }

            if (intro && !introdone)
            {

                CameraZoomIn();

                if (clickcount == 0)
                {
                    // [] show playertext
                    InternalCalls.SetPosX("Boat", "Level1", 160.0f); // freeze boat in place at spawn // error
                    InternalCalls.SetPosY("Boat", "Level1", 120.0f); // freeze boat in place at spawn // error
                    ClickCheck();
                }

                if (clickcount == 1)
                {
                    // [] hide playertext
                    // InternalCalls.EntityDeactivate("testbox", "Level1"); // error
                    introdone = true;
                }
            }

            else if (encounter1 && !encounter1done)
            {

                CameraZoomIn();

                if (clickcount == 1)
                {
                    // [] show littlegirltext
                    ClickCheck();
                }

                if (clickcount == 2)
                {
                    // [] change littlegirltext
                    // [] show playertext
                    ClickCheck();
                }

                if (clickcount == 3)
                {
                    // [] hide playertext
                    // [] change littlegirltext
                    ClickCheck();
                }

                if (clickcount == 4)
                {
                    // hide littlegiirltext
                    encounter1done = true;
                }
            }

            else if (encounter2 && !encounter2done)
            {

                CameraZoomIn();

                if (clickcount == 4)
                {
                    // [] show passengertext
                    ClickCheck();
                }

                if (clickcount == 5)
                {
                    // [] change passengertext
                    // [] show playertext
                    ClickCheck();
                }

                if (clickcount == 6)
                {
                    // [] hide playertext
                    // [] change passengeretext
                    ClickCheck();
                }

                if (clickcount == 7)
                {
                    // hide passengertext
                    encounter1done = true;
                }
            }

            else if (encounter3 && !encounter3done)
            {
                // [] show passengertext
                // game over
            }

            else
            {
                CameraZoomOut();
            }

            if (InternalCalls.CheckKeyPress(68))
            {
                this.hello = "PRESSED!";
                Console.WriteLine("Printing from C#: " + this.hello);

                // Printing dialogue index 1 of loaded dialogue file
                Console.WriteLine(InternalCalls.GetDialogue(1));
            }
        }

        public void Exit()
        {
            Console.WriteLine("Exiting...");
        }

        public void ClickCheck()
        {
            if (InternalCalls.CheckKeyPress(349)) { clickcount++; } //left click
        }

        public void CameraZoomIn()
        {
            InternalCalls.SetCurrentCameraScaleX(700);
        }

        public void CameraZoomOut()
        {
            InternalCalls.SetCurrentCameraScaleX(1600);
        }
    }
}
