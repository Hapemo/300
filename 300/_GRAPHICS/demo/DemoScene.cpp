/**-------------------------------------------------------------------------------------/
 * @file
 *  DemoScene.cpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class implementation of DemoScene. DemoScene class showcases the different shapes
    that can be rendered
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#include "DemoScene.hpp"
#include "Common.hpp"
#include "ImGui.hpp"
#include <cassert>

 /**---------------------------------------------------------------------------/
  * @brief
  *  Constructor for the DemoScene class
  * @param window
  *  Window class object to render the objects onto
  * @param renderer
  *  DebugRenderer class object to render the objects with
  * @return
  *  None
  *---------------------------------------------------------------------------*/
CS380::DemoScene::DemoScene(Window const& window, DebugRenderer* renderer) : mWindow{ window }, mRenderer{ renderer }, mCamera{}
{

}

/**---------------------------------------------------------------------------/
 * @brief
 *  Initializes the DemoScene with the necessary states to be enabled. Sets up
 *  the camera's position, target and projection as well
 * @return
 *  None
 *---------------------------------------------------------------------------*/
void CS380::DemoScene::Initialize()
{
    glViewport(0, 0, mWindow.size().x, mWindow.size().y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);

    // State
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);

    // Camera
    mCamera.SetProjection(-mWindow.size().x / 2.f, mWindow.size().x / 2.f, -mWindow.size().y / 2.f, mWindow.size().y / 2.f, 0.01f, 200.f);
    mCamera.SetPosition({ 0, 0, 10 });
    mCamera.SetTarget({ 0, 0, 0 });
    mCamera.Update();

    // Seed random generator
    CS380::Utils::srand(std::chrono::system_clock::now().time_since_epoch().count(), 0);
    
    // Initialize ImGui
    // ImGui
    IMGUI_CHECKVERSION();
    if (!ImGui::CreateContext())
        throw std::runtime_error("Could not initialize ImGui");

    if (!ImGui_ImplGlfw_InitForOpenGL(mWindow.GetHandle(), true))
        throw std::runtime_error("Could not initialize ImGui::OpenGL");

    if (!ImGui_ImplOpenGL3_Init("#version 150"))
        throw std::runtime_error("Could not initialize ImGui::OpenGL (2)");
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Updates the DemoScene by taking in user inputs and updating the camera's
 *  position and target
 * @return
 *  None
 *---------------------------------------------------------------------------*/
void CS380::DemoScene::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Imgui start frame
    if (ImGui::Begin("Options"))
    {

    }
    ImGui::End();

    float now = static_cast<float>(glfwGetTime());
    mDt = now - mLastTime;
    mLastTime = now;

    // User input
    if (glfwGetKey(mWindow.GetHandle(), GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(mWindow.GetHandle(), 1);
    }

    static char buffer[10];
    sprintf(buffer, "FPS: %d", int(1.0f / mDt));
    glfwSetWindowTitle(mWindow.GetHandle(), buffer);
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Draws the scene onto screen by using the DebugRenderer class instance
 * @return
 *  None
 *---------------------------------------------------------------------------*/
void CS380::DemoScene::Draw()
{
    while (!glfwWindowShouldClose(mWindow.GetHandle()))
    {
        glfwPollEvents();
        Update();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        mRenderer->RenderAll(mCamera.viewProj());

        // Clear all instances
        mRenderer->ClearInstances();
        // ImGui UI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(mWindow.GetHandle());
    } // END WHILE
}

/**---------------------------------------------------------------------------/
 * @brief
 *  Cleans up the DemoScene. Frees up any memory allocated if any
 * @return
 *  None
 *---------------------------------------------------------------------------*/
void CS380::DemoScene::Exit()
{
    ImGui::DestroyContext();
}
