#include "Editor.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"
#include <iostream>


void Editor::UIinit()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //regular = io.Fonts->AddFontFromFileTTF("../Resources/Fonts/Assistant-Regular.ttf", 21.0f);
    //bold = io.Fonts->AddFontFromFileTTF("../Resources/Fonts/Assistant-SemiBold.ttf", 18.0f);
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

    // ----------------------------------------------------------------------------- // Style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    }

    style.FrameBorderSize = 0.f;
    style.TabBorderSize = 1.5f;
    style.TabRounding = 0.f;
    style.ItemSpacing = ImVec2(4.f, 4.f);
    style.WindowBorderSize = 0.f;


    ImVec4* colors = ImGui::GetStyle().Colors;

    ImGui_ImplGlfw_InitForOpenGL(_Editorwindow, true);
    
    std::cout << "" << "OpenGL Version: " << glGetString(GL_VERSION) <<"\n";
    ImGui_ImplOpenGL3_Init("#version 450");

    //for (auto& windows : windowlist)
    //{
    //    windows.second->init();
    //}

}

void Editor::UIupdate() {

    // ----------------------------------------------------------------------------- // style and basic setups

   // ImGuiWindowFlags windowflag = 0;
    //windowflag |= ImGuiWindowFlags_NoScrollbar;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    //ImGui::Begin("Editor Window", 0, ImGuiWindowFlags_NoBackground
    //    | ImGuiWindowFlags_NoBringToFrontOnFocus
    //    | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar
    //    | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    //ImGui::PopStyleVar();
    //ImGui::PopStyleVar();
    //const ImGuiViewport* viewport = ImGui::GetMainViewport();
    //ImGui::SetWindowPos(viewport->WorkPos);
    //ImGui::SetWindowSize(viewport->WorkSize);

    //ImGui::PushID("dock_space");
    //ImGui::DockSpace(ImGui::GetID("dock_space"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    //// ----------------------------------------------------------------------------- // draw Menu Window

    //ImGui::Begin("Editor Window", 0, ImGuiWindowFlags_NoBringToFrontOnFocus
    //    | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar
    //    | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    //topwindow->update();


    ImGui::ShowDemoWindow();


   // ImGui::End();

   // ImGui::PopID();

    // ----------------------------------------------------------------------------- // draw all Editor Windows 

}

void Editor::WindowUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(_Editorwindow);
    //std::stringstream sstr;
    //sstr << "FPS: " << Timer::fps << " " << "Test FPS: " << test_fps << "Graphics ms: " << Timer::systemDuration[0] << " " << "Graphics ms %: " << Timer::FpsPercentage(Timer::systemDuration[0]) << " " << "Sample ms: " << Timer::systemDuration[1] << " " << "Sample ms %: " << Timer::FpsPercentage(Timer::systemDuration[1]);;
    //glfwSetWindowTitle(window, sstr.str().c_str());

}

void Editor::UIdraw(GLFWwindow* /*win*/)
{
    ImGui::Render();
    //glClearColor(1.0f, 0.0f, 0.0f, 1.f);
    //glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(_Editorwindow);
}



void Editor::UIend()
{

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    for (auto& windows : windowlist) {
        delete windows.second;
    }
    //delete topwindow;
}


EditorWindow* Editor::Getwindow(std::string id)
{
    return windowlist[id];
}