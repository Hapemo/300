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
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "Example.h"
#include "Input.h"
#include "SingletonManager.h"
#include "../../_SCRIPTING/ScriptingSystem.h"
#include "../../_ENGINE/include/Object/ObjectFactory.h"

// Static variables
GFX::DebugRenderer* Application::mRenderer;
GFX::Window Application::mWindow;
std::string Application::title;
SystemManager Application::systemManager;

void Application::Init() 
{
    StartUp();
    SystemInit();
    //Example();
    // test serialization
    // working, just no information for the existing entities yet
    //ObjectFactory::SerializeScene("../resources/Scenes/test.json");
}

void Application::StartUp() 
{
    //gfx glew and glfw startup
    GFX::Window::InitializeSystem();
    mWindow = GFX::Window({ 1920, 1080 });
    mWindow.SetWindowTitle("Application");
}

void Application::SystemInit() 
{
    systemManager.Init();
    //gfx init
    Input::Init(mWindow);
    // To remove (Script test with entities)
    //ScriptTestInit();
}

void Application::FirstUpdate() 
{
    mWindow.PollEvents();
}

void Application::SystemUpdate() 
{
    systemManager.Update(1.f /*insert dt here*/);
}

void Application::SecondUpdate() 
{
    Input::UpdatePrevKeyStates();
}

void Application::MainUpdate() 
{
    while (!glfwWindowShouldClose(mWindow.GetHandle())) {
        FirstUpdate();
        SystemUpdate();
        // To remove (Script test with entities)
        //ScriptTestUpdate();
        SecondUpdate(); // This should always be the last

        // Graphics update
        mWindow.Update();
    }
}

void Application::Exit() 
{
    systemManager.Exit();
    ECS::GetInstance()->DeleteAllEntities();
    SingletonManager::destroyAllSingletons();
    glfwTerminate();
}

void Application::error_cb(int error, char const* description) 
{
    (void)error;
    (void)description; // This function should only be called for debug mode
    std::cerr << "GLFW error " << error << ": " << description << std::endl;
}

void Application::fbsize_cb(GLFWwindow* ptr_win, int width, int height) 
{
    std::cout << "fbsize_cb getting called!!!" << std::endl;
    // use the entire framebuffer as drawing region
    glViewport(0, 0, width, height);
    (void)ptr_win;
    // later, if working in 3D, we'll have to set the projection matrix here ...
}
