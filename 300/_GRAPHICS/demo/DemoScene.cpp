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
GFX::DemoScene::DemoScene(Window const& window, DebugRenderer* renderer) : mWindow{ window }, mRenderer{ renderer }, mCamera{}
{

}

/**---------------------------------------------------------------------------/
 * @brief
 *  Initializes the DemoScene with the necessary states to be enabled. Sets up
 *  the camera's position, target and projection as well
 * @return
 *  None
 *---------------------------------------------------------------------------*/
void GFX::DemoScene::Initialize()
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
    GFX::Utils::srand(std::chrono::system_clock::now().time_since_epoch().count(), 0);
    
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
void GFX::DemoScene::Update()
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

    double mouseX = 0.0;
    double mouseY = 0.0;
    glfwGetCursorPos(mWindow.GetHandle(), &mouseX, &mouseY);

    // User input
    if (glfwGetKey(mWindow.GetHandle(), GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(mWindow.GetHandle(), 1);
    }

    auto  side = glm::normalize(glm::cross(mCamera.direction(), { 0, 1, 0 }));
    auto  up = glm::normalize(glm::cross(mCamera.direction(), side));

    // Move
    vec3 moveVector{};
    if (glfwGetKey(mWindow.GetHandle(), GLFW_KEY_W)) {
        moveVector += glm::normalize(mCamera.direction()) * mDt * mCamSpeed;
    }
    if (glfwGetKey(mWindow.GetHandle(), GLFW_KEY_S)) {
        moveVector -= glm::normalize(mCamera.direction()) * mDt * mCamSpeed;
    }
    if (glfwGetKey(mWindow.GetHandle(), GLFW_KEY_A)) {
        moveVector -= glm::normalize(side) * mDt * mCamSpeed;
    }
    if (glfwGetKey(mWindow.GetHandle(), GLFW_KEY_D)) {
        moveVector += glm::normalize(side) * mDt * mCamSpeed;
    }
    if (glfwGetKey(mWindow.GetHandle(), GLFW_KEY_SPACE)) {
        moveVector -= up * mDt * mCamSpeed;
    }
    if (glfwGetKey(mWindow.GetHandle(), GLFW_KEY_LEFT_CONTROL)) {
        moveVector += up * mDt * mCamSpeed;
    }

    mCamera.SetPosition(mCamera.position() + moveVector);

    if (glfwGetMouseButton(mWindow.GetHandle(), GLFW_MOUSE_BUTTON_2)) {
        // View
        glm::vec2 cursor_delta = { (float)mouseX - mCamera.cursorPosition().x, (float)mouseY - mCamera.cursorPosition().y };
        mCamera.SetTarget(glm::vec3(glm::vec4(mCamera.direction(), 0) * glm::rotate(glm::radians(5.0f) * mDt * cursor_delta.y, side)));
        mCamera.SetTarget(glm::vec3(glm::vec4(mCamera.direction(), 0) * glm::rotate(glm::radians(5.0f) * mDt * cursor_delta.x, glm::vec3(0, 1, 0))));
    }

    mCamera.SetCursorPosition({ mouseX, mouseY });
    mCamera.Update();

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
void GFX::DemoScene::Draw()
{
    while (!glfwWindowShouldClose(mWindow.GetHandle()))
    {
        glfwPollEvents();
        Update();

        // Clears buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mRenderer->AddAabb({ 0, 0, -50 }, { 100, 300, 100 }, { 1, 0, 0, 1 });
        //mRenderer->AddPoint({ 0, 0, 0 }, { 1, 0, 0, 1 });
        //mRenderer->AddTriangle({ 0.f, 30.f, 0.f }, { -30.f, -30.f, 0.f }, { 30.f, -30.f, 0.f }, { 0, 1, 0, 1 });
        //mRenderer->AddQuad({ 0, 0, 0 }, 50, 100, { 0, 0, 1, 1 });

        // Renders all instances
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
void GFX::DemoScene::Exit()
{
    ImGui::DestroyContext();
}
