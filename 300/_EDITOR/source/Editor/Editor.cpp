/*!*************************************************************************
****
\file Editor.cpp
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 22-9-2022
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
#include "PrefabWindow.h"
#include "PrefabWindow.h"
//bool Editor::show_Inspector;
//int Editor::entity {}; // static var for selected entity ID
//bool Editor::Entity_Selected; // static var for Inspector to show

void Editor::UIinit(GLFWwindow* window)
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
    style.ItemSpacing = ImVec2( 4.f,4.f);
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
    mWindowlist["Editscene"] = new SceneWindow;
    mWindowlist["Contentbrowser"] = new ContentBrowser;
    mWindowlist["Logger"] = new EditorLogger;

    mWindowlist["PrefabScene"] = new PrefabWindow;
    //std::cout<< mWindowlist.size() << "test\n";

    //windowlist["ContentBrowser"] = new ContentBrowser;
    //windowlist["ContentBrowserL"] = new ContentBrowserL;
    //windowlist["Scene"] = new SceneWindow;
    //windowlist["Display"] = new SceneWindow;
    //windowlist["Layer"] = new LayerWindow;

//#ifndef RELEASE
//    windowlist["Profiler"] = new Performance;
//#endif
//
//    windowlist["Logger"] = new LoggerWindow;
//    windowlist["Inspect"] = new Inspect;
//    topwindow = new MenuPanel;
//
//    static_cast<SceneWindow*>(windowlist["Scene"])->Framebuffer_spec(Editor_fb->color_attachment);
//    static_cast<SceneWindow*>(windowlist["Display"])->Framebuffer_spec(MainCam_fb->color_attachment);
//
    for (auto & windows : mWindowlist ) 
    {
        windows.second->init();
    }
}

void Editor::WindowUpdate(GLFWwindow* window)
{
    glfwPollEvents();
    glfwSwapBuffers(window);
    //std::stringstream sstr;
    //sstr << "FPS: " << Timer::fps << " " << "Test FPS: " << test_fps << "Graphics ms: " << Timer::systemDuration[0] << " " << "Graphics ms %: " << Timer::FpsPercentage(Timer::systemDuration[0]) << " " << "Sample ms: " << Timer::systemDuration[1] << " " << "Sample ms %: " << Timer::FpsPercentage(Timer::systemDuration[1]);;
    //glfwSetWindowTitle(window, sstr.str().c_str());
    
}



void Editor::UIupdate(GLFWwindow* window) {

    // ----------------------------------------------------------------------------- // style and basic setups

    ImGuiWindowFlags windowflag = 0;
    windowflag |= ImGuiWindowFlags_NoScrollbar;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
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

//#ifndef SANDBOX_RELEASE

    //if (g_engine->graphicsSys->editorOn) {


    for (auto& windows : mWindowlist)
    {
        //if (windows.first == "Editscene")
        //    static_cast<SceneWindow*>(windows.second)->ConstrainedResize(nullptr);
        //
        ImGui::Begin(windows.first.c_str(), 0, windows.second->mWinFlag);


        
        if (windows.first == "Editscene") {
            (static_cast<SceneWindow*>(windows.second))->RenderGuizmo();
        }
        
        if (windows.first == "PrefabScene") {
            if (static_cast<int>(PrefabWindow::prefabObj) != -1) {
                windows.second->update();
            }

        }
        else {
            windows.second->update();
        }


        ImGui::End();
    }
    //        if (windows.first == "Scene" || windows.first == "Display")
    //        {
    //           // windowflag |= ImGuiWindowFlags_NoScrollWithMouse;
    //            ImGui::Begin(windows.first.c_str(), 0, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
    //        }
    //        else if (windows.first == "Inspect") {
    //            
    //            //windowflag |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
    //            ImGui::Begin(windows.first.c_str(), 0, windowflag);
    //        }
    //        else {
    //            ImGui::Begin(windows.first.c_str(), 0, windowflag);
    //        }
    //        // only enable click and drag for scene

    //        
    //        //  if (windows.first != "Profiler" ) {
    //        windows.second->update();
    //        // }

    //        if (windows.first == "Scene") {

    //            if (g_engine->graphicsSys->displayOn == false) {
    //                static_cast<SceneWindow*>(windows.second)->ClickDrop();
    //                static_cast<SceneWindow*>(windows.second)->RenderGuizmo();
    //            }
    //        }
    //        ImGui::End();
    //    }
    //    ImGui::End();

    //}

//#endif // RELEASE_SANDBOX
}



void Editor::UIdraw(GLFWwindow* window) 
{
    ImGui::Render();
    //glClearColor(1.0f, 0.0f, 0.0f, 1.f);
    //glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(window);
}



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


EditorWindow* Editor::Getwindow(std::string id) 
{
    return mWindowlist[id];
}
