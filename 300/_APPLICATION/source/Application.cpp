/*!*****************************************************************************
\file main.cpp
\author Kew Yu Jun
\par DP email: k.yujun@digipen.edu
\par Group: Memory Leak Studios
\date 15/05/2023
\brief
Main application that gets called in the main loop. It handles the creation and
start up of window and game system, also runs their update functions.
*******************************************************************************/
#include "Application.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Helper.h"
#include "ECS.h"
#include "ECS_Components.h"
#include "Example.h"
#include "Input.h"
#include "SingletonManager.h"

// Static variables
GFX::Window Application::mWindow;
std::string Application::title;
void Application::StartUp() {
    //gfx glew and glfw startup
    GFX::Window::InitializeSystem();
    mWindow = GFX::Window({ 1920, 1080 });
}

void Application::SystemInit() {
    //gfx init
  Input::Init(mWindow.GetHandle());
}

void Application::SystemUpdate() {
    
}

void Application::Init() {
  StartUp();
  SystemInit();
  Example();
}

bool Application::FirstUpdate() {
  //FPSManager::mPrevTime = glfwGetTime();
  // Part 1
  glfwPollEvents();

  // Part 2
  //FPSManager::CalcFPS(0);
  return true;// !Helper::GetWindowMinimized();

}

void Application::SecondUpdate() {
  Input::UpdatePrevKeyStates();
  //FPSManager::LimitFPS();
  //FPSManager::CalcDeltaTime();
}

void Application::MainUpdate() {
  // Update gamestate and loop it. 
  // Stop when exit or restart game state.
  // Stop when change game state. 

  // Structure is this,
  // 
  // Application and controls first update
  // Editor update
  // Logic & Systems update
  // Graphics update
  // Application ending update

  while (!glfwWindowShouldClose(mWindow.GetHandle())) {
      PrintTitleBar(0.5);

    FirstUpdate();
    SystemUpdate();
    SecondUpdate(); // This should always be the last

    // Graphics update

    mWindow.Update();
  }
}

void Application::Exit() {
    ECS::GetInstance()->DeleteAllEntities();
    SingletonManager::destroyAllSingletons();
  glfwTerminate();
}

// _s, time interval in updating titlebar, in seconds
void Application::PrintTitleBar(double _s) {
  static double timeElapsed{};
  //timeElapsed += FPSManager::dt;
  if (timeElapsed > _s) {
    timeElapsed = 0;

    // write window title with current fps ...
    std::stringstream sstr;

    //sstr << std::fixed << std::setprecision(3) << Application::title << " | " 
    //                                           << "fps: " << FPSManager::fps << " | "
    //                                           << "dt: " << FPSManager::dt;
    mWindow.SetWindowTitle(sstr.str().c_str());
  }
}

void Application::GLFWStartUp() {
  //  glfwInit();
  //const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

  //if (!window_width) window_width = mode->width;
  //if (!window_height) window_height = mode->height;

  //// In case a GLFW function fails, an error is reported to callback function
  //glfwSetErrorCallback(error_cb);
  //
  // //Before asking GLFW to create an OpenGL context, we specify the minimum constraints
  //// in that context:
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  //glfwWindowHint(GLFW_DEPTH_BITS, 24);
  //glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
  //glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
  //glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // window dimensions are not static

  //ptr_window = glfwCreateWindow(window_width, window_height, title.c_str(), NULL, NULL);
  //glfwSetWindowAspectRatio(ptr_window, window_width, window_height);
  //glfwSetWindowSizeCallback(ptr_window, [](GLFWwindow* window, int width, int height)
  //    {
  //        (void)window;
  //        window_width = width;
  //        window_height = height;
  //    });
  //if (!ptr_window) {
  //  glfwTerminate();
  //}

  //glfwMakeContextCurrent(ptr_window);

  //glfwSetFramebufferSizeCallback(ptr_window, fbsize_cb);
  //glfwSwapInterval(0);
}

void Application::GlewStartUp() {
  //// Part 2: Initialize entry points to OpenGL functions and extensions
  //GLenum err = glewInit();

  //std::stringstream string;
  //string << "Unable to initialize GLEW - error " << glewGetErrorString(err) << " abort program\n";
}

void Application::error_cb(int error, char const* description) {
    (void)error;
    (void)description; // This function should only be called for debug mode
  std::cerr << "GLFW error " << error << ": " << description << std::endl;
}

void Application::fbsize_cb(GLFWwindow* ptr_win, int width, int height) {
  std::cout << "fbsize_cb getting called!!!" << std::endl;
  // use the entire framebuffer as drawing region
  glViewport(0, 0, width, height);
  (void)ptr_win;
  // later, if working in 3D, we'll have to set the projection matrix here ...
}
