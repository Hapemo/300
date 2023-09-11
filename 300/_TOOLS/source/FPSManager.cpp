/*!*****************************************************************************
\file Helper.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
General helper class of application that calculates FPS and prints GLFW info
*******************************************************************************/
#include "FPSManager.h"
// static data members declared in Helper
double FPSManager::fps;
double FPSManager::dt;
double FPSManager::mPrevTime;
GFX::Window* FPSManager::mWindow;

void FPSManager::Init(GFX::Window* window)
{
	mWindow = window;
}

void FPSManager::Update(double fps_calc_interval) {
  double curr_time = glfwGetTime();

  // fps calculations
  static double count = 0.0; // number of game loop iterations
  static double start_time = glfwGetTime();
  // get elapsed time since very beginning (in seconds) ...
  double elapsed_time = curr_time - start_time;

  ++count;

  // update fps at least every 10 seconds ...
  fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
  fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
  if (elapsed_time > fps_calc_interval) {
    FPSManager::fps = count / elapsed_time; // elapsed_time;
    start_time = curr_time;
    count = 0.0;
  }
}

void FPSManager::CalcDeltaTime() {
  dt = glfwGetTime() - mPrevTime;
  mPrevTime = glfwGetTime();
}

void FPSManager::LimitFPS(unsigned int fpsCap) {
  if (fpsCap) {
    double targetedDT = 1.f / fpsCap;
    while ((glfwGetTime() - mPrevTime) < targetedDT) {}
  }

  CalcDeltaTime();
}