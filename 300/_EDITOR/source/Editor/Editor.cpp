/*!*************************************************************************
****
\file Editor.cpp
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 28-09-2023
\brief
This header file contains main loop logic for drawing all related IMGUI window 
classes declared such as Inspect, ContentBrowser.. etc, setting up / creation 
of all classes, deletion of classes and execution of IMGUI related component.

The functions
- UIinit
Initialize IMGUI related data, prepare for rendering UI necessary resosures
- WindowUpdate
Run swapping of window buffer/polling event
- UIupdate
Contains main loop for rendering all window of inherited class of EditorWindow 
which are sections of UI respectively for their own purposes
- UIend
End call function for IMGUI 
-UIdraw
Draw setup/calling for IMGUI related calls for drawing UI
- Getwindow
Returns main window for docking

****************************************************************************
***/

#include "Editor.h"
#include <iostream>
#include "Hierarchy.h"
#include "SceneWindow.h"
#include "ContentBrower.h"
#include "Performance.h"
#include "Inspect.h"
#include "MenuPanel.h"
#include "EditorLogger.h"
#include "Debug/EnginePerformance.h"
#include "PrefabWindow.h"
#include "GameScene.h"
#include "TabWindow.h"
#include "KeybindWindow.h"
#include "MenuTab.h"
#include "ShaderCompiler.h"
/***************************************************************************/
/*!
\brief
    Init function for Imgui Window which constructs all the child windows
*/
/**************************************************************************/
void Editor::UIinit(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();


    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    float baseFontSize = 16.0f; // 13.0f is the size of the default font. Change to the font size you use.
    float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges);






    // ImGuiIO& io = ImGui::GetIO(); (void)io;
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
    style.FrameRounding = 12.f;
    style.TabRounding = 4.f;
    style.TabBorderSize = 1.5f;
    style.ItemSpacing = ImVec2(4.f, 4.f);
    style.WindowBorderSize = 0.f;


    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.07f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.08f, 0.08f, 0.08f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.28f, 0.28f, 0.28f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.44f, 0.44f, 0.44f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.68f, 0.68f, 0.68f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.40f, 0.40f, 0.22f);
    colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.64f, 0.64f, 0.64f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.77f, 0.77f, 0.77f, 0.78f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.52f, 0.52f, 0.52f, 0.78f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.78f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.78f);
    colors[ImGuiCol_Tab] = ImVec4(0.26f, 0.26f, 0.26f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.08f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.48f, 0.48f, 0.48f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.12f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.72f, 0.72f, 0.72f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.36f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


    // ----------------------------------------------------------------------------- // style end

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    // ----------------------------------------------------------------------------- // Add EditorWindows

    mMenuwindow = new MenuPanel;
    mWindowlist["Objects"] = new Hierarchy;
    //mWindowlist["Menu"] = new MenuPanel;
    mWindowlist["Inspect"] = new Inspect;
    mWindowlist["Performance"] = new Performance;
    mWindowlist["Settings"] = new TabWindow;
    mWindowlist["Editscene"] = new SceneWindow;
    mWindowlist["Contentbrowser"] = new ContentBrowser;
    mWindowlist["Logger"] = new EditorLogger;
    mWindowlist["PrefabScene"] = new PrefabWindow;
    mWindowlist["GameScene"] = new GameScene;
    mWindowlist["KeybindWindow"] = new KeybindWindow;
    mWindowlist["MenuTab"] = new MenuTab;
    mWindowlist["ShaderCompiler"] = new ShaderCompiler;
    for (auto& windows : mWindowlist)
    {
        windows.second->init();
    }
}
/***************************************************************************/
/*!
\brief
    Update function for Imgui Window that loops the window container to run all windows
*/
/**************************************************************************/
void Editor::WindowUpdate(GLFWwindow* window)
{
    glfwPollEvents();
    glfwSwapBuffers(window);
    
}


/***************************************************************************/
/*!
\brief
    Update functionality (temporary) for Opengl implementation
*/
/**************************************************************************/
void Editor::UIupdate([[maybe_unused]]GLFWwindow* window) {

    // ----------------------------------------------------------------------------- // style and basic setups

    ImGuiWindowFlags windowflag = 0;
    windowflag |= ImGuiWindowFlags_NoScrollbar;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
   // ImGui::PushStyleVar(imguistylevar_windowsize, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Editor Window",0 , 
         ImGuiWindowFlags_NoBringToFrontOnFocus 
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar 
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    


    ImGui::PopStyleVar();
    ImGui::PopStyleVar();



    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetWindowPos(viewport->WorkPos);
    ImGui::SetWindowSize(viewport->WorkSize);

    ImGui::PushID("dock_space");
    ImGui::DockSpace(ImGui::GetID("dock_space"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    //// ----------------------------------------------------------------------------- // draw Menu Window

  
    mMenuwindow->update();

    ImGui::PopID();

    ImGui::End();


    // ----------------------------------------------------------------------------- // draw all Editor Windows 
   

    ImGui::ShowDemoWindow();



    for (auto& windows : mWindowlist)
    {

        if (Entity(Hierarchy::selectedId).HasComponent<Transform>() == false) {
            Hierarchy::selectionOn = false;
        }

        if (windows.first == "KeybindWindow") {

            if (KeybindWindow::openWindow ) {
                ImGui::Begin(windows.first.c_str(),&KeybindWindow::openWindow, windows.second->mWinFlag);
                windows.second->update();
                ImGui::End();
            }
        }
        else if (windows.first == "ShaderCompiler") {

            if (ShaderCompiler::openWindow) {
                ImGui::Begin(windows.first.c_str(), &ShaderCompiler::openWindow, windows.second->mWinFlag);
                windows.second->update();
                ImGui::End();
            }
        }

        else if (windows.first == "PrefabScene") {
            
            if (static_cast<unsigned>(PrefabWindow::prefabObj) != 0) {
                ImGui::Begin(windows.first.c_str(), 0, windows.second->mWinFlag);
                windows.second->update();
                ImGui::End();
            }
        }
        else if (windows.first == "MenuTab") {

                ImGuiWindowClass window_class;
                window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
                window_class.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoDecoration;
                ImGui::SetNextWindowClass(&window_class);

                ImGui::Begin(windows.first.c_str(), 0, windows.second->mWinFlag);
                windows.second->update();
                ImGui::End();
        }
        else {

            ImGui::Begin(windows.first.c_str(), 0, windows.second->mWinFlag);

            windows.second->update();
            if (windows.first == "Editscene") 
            {
                (static_cast<SceneWindow*>(windows.second))->RenderGuizmo();
            }
            ImGui::End();
        }

    }
}

/***************************************************************************/
/*!
\brief
    Draw function for Imgui Window
*/
/**************************************************************************/

void Editor::UIdraw(GLFWwindow* window) 
{
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(window);
}

/***************************************************************************/
/*!
\brief
    End function for Imgui Window
*/
/**************************************************************************/

void Editor::UIend() 
{
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    for (auto& windows : mWindowlist) {
        delete windows.second;
    }
    delete mMenuwindow;
}

/***************************************************************************/
/*!
\brief
    return Imgui Window
*/
/**************************************************************************/
EditorWindow* Editor::Getwindow(std::string id) 
{
    return mWindowlist[id];
}
