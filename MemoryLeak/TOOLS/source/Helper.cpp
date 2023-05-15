/*!*****************************************************************************
\file Helper.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
General helper class of application that calculates FPS and prints GLFW info
*******************************************************************************/
#include "Helper.h"
#include "Logger.h"

// static data members declared in Helper
GLdouble FPSManager::fps;
double FPSManager::dt;
double FPSManager::mPrevTime;
double FPSManager::mLimitFPS = 0;

GLFWwindow* Helper::mWindow = nullptr;
GLFWmonitor* Helper::mMonitor = nullptr;
std::array<int, 2> Helper::mWindowPos{};
std::array<int, 2> Helper::mWindowSize{};
std::array<int, 2> Helper::mWindowMaximisedSize{};
bool Helper::fullscreenFlag{ false };

void FPSManager::CalcFPS(double fps_calc_interval) {
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
  FPSManager::fps = count / FPSManager::dt; // elapsed_time;
    start_time = curr_time;
    count = 0.0;
  }
}

void FPSManager::CalcDeltaTime() {
  dt = glfwGetTime() - mPrevTime;
  mPrevTime = glfwGetTime();
}

void FPSManager::LimitFPS() {
  if (!mLimitFPS) return;

  double targetedDT = 1 / mLimitFPS;
  while ((glfwGetTime() - mPrevTime) < targetedDT) {}
}

void Helper::Init(GLFWwindow* _winPtr) {
  mWindow = _winPtr;
  mMonitor = glfwGetPrimaryMonitor();
  glfwGetWindowSize(mWindow, &mWindowSize[0], &mWindowSize[1]);
  glfwGetWindowPos(mWindow, &mWindowPos[0], &mWindowPos[1]);
}

void Helper::SetFullScreen(bool _fullscreen) {
  if (fullscreenFlag == _fullscreen) return;

  // Set fullscreen
  if (_fullscreen) {
    // backup window position and window size
    glfwGetWindowPos(mWindow, &mWindowPos[0], &mWindowPos[1]);
    glfwGetWindowSize(mWindow, &mWindowSize[0], &mWindowSize[1]);

    // get resolution of monitor
    const GLFWvidmode* mode = glfwGetVideoMode(mMonitor);

    // switch to full screen
    glfwSetWindowMonitor(mWindow, mMonitor, 0, 0, mode->width, mode->height, 0);
    mWindowMaximisedSize = { mode->width, mode->height };
    LOG_INFO("Fullscreen activated");
    
    fullscreenFlag = true;
  } else {
    // restore last window size and position
    glfwSetWindowMonitor(mWindow, nullptr, mWindowPos[0], mWindowPos[1], mWindowSize[0], mWindowSize[1], 0);
    fullscreenFlag = false;
  }


}

