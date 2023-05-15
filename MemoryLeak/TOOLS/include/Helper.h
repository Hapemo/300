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

struct FPSManager {
  /*!*****************************************************************************
  This function must be called once per game loop. It uses GLFW's time
  functions to compute:
  1. the interval in seconds between each frame
  2. the frames per second every "fps_calc_interval" seconds

  \param double
  - the interval (in seconds) at which fps is to be calculated
  *******************************************************************************/
  static void CalcFPS(double = 1.0);

  /*!*****************************************************************************
  Calculates the time taken for the previous frame to run
  *******************************************************************************/
  static void CalcDeltaTime();

  /*!*****************************************************************************
  Limit the FPS. If limit fps is 0, fps will not be limited.
  *******************************************************************************/
  static void LimitFPS();

  static GLdouble fps;
  static double dt; // time taken to complete most recent game loop
  static double mPrevTime; // Time at beginning of game loop
  static double mLimitFPS;
};

class Helper {
public:
  /*!*****************************************************************************
  Limit the FPS. If limit fps is 0, fps will not be limited.

  /param GLFWwindow*
  - pointer to main game window
  *******************************************************************************/
  static void Init(GLFWwindow* _winPtr);

  /*!*****************************************************************************
  Set the main window to fullscreen

  /param bool
  - True if fullscreen, otherwise false
  *******************************************************************************/
  static void SetFullScreen(bool _fullscreen);

  /*!*****************************************************************************
  Gets the flag for window's fullscreen state

  /return bool
  - True if main window is in fullscreen, otherwise false
  *******************************************************************************/
  static bool GetFullScreenFlag() { return fullscreenFlag; }

  /*!*****************************************************************************
  Get main window's screen width

  /return int
  - Width of screen
  *******************************************************************************/
  static int GetScreenWidth() { return fullscreenFlag ? mWindowMaximisedSize[0] : mWindowSize[0]; }

  /*!*****************************************************************************
  Get main window's screen height

  /return int
  - Height of screen
  *******************************************************************************/
  static int GetScreenHeight() { return fullscreenFlag ? mWindowMaximisedSize[1] : mWindowSize[1]; }

  /*!*****************************************************************************
  Get main window's selected status

  /return bool
  - True if main window is not selected, otherwise false
  *******************************************************************************/
  static bool GetWindowMinimized() { return !glfwGetWindowAttrib(mWindow, GLFW_FOCUSED ); }

private:
  static GLFWwindow* mWindow;
  static GLFWmonitor* mMonitor;
  static std::array<int, 2> mWindowPos;
  static std::array<int, 2> mWindowSize;
  static std::array<int, 2> mWindowMaximisedSize;
  static bool fullscreenFlag;
};
