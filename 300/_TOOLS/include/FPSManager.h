/*!*****************************************************************************
\file Helper.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
FPSManager class of application that calculates FPS.

For milestone 3:
Helper class of application that handles the glfw related operations which game
Engine requires.
*******************************************************************************/
#pragma once
#include "pch.h"

struct FPSManager 
{
	static void Init();
  /*!*****************************************************************************
  This function must be called once per game loop. It uses GLFW's time
  functions to compute:
  1. the interval in seconds between each frame
  2. the frames per second every "fps_calc_interval" seconds

  \param double
  - the interval (in seconds) at which fps is to be calculated
  *******************************************************************************/
  static void Update(double = 1.0);

  /*!*****************************************************************************
  Calculates the time taken for the previous frame to run
  *******************************************************************************/
  static void CalcDeltaTime();

  /*!*****************************************************************************
  Limit the FPS. If limit is not specified, fps will not be limited.

  \param unsigned int
  - The fps to limit to, default value 0
  *******************************************************************************/
  static void LimitFPS(unsigned int = 0);

  static double fps;
  static double dt; // time taken to complete most recent game loop
  static double mPrevTime; // Time at beginning of game loop

  //static int counter; // for performance visualizer in imGui
  static bool trigger;
};
