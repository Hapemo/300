/*!*************************************************************************
****
\file MenuPanel.cpp
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 28-09-2023
\brief
This file contains MenuPanel that occupies top area of the Window.

The functions
- update
Contains main loop for the logic of MenuPanel.


****************************************************************************
***/
#include "EditorWindow.h"
#include "Editor.h"
#include "MenuPanel.h"
#include "SceneWindow.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Systems.h"
#include "GameState/GameStateManager.h"
#include "Hierarchy.h"
#include "ScriptingSystem.h"
#include "KeybindWindow.h"
#include "PathfinderWindow.h"
#include "ShaderCompiler.h"
#include "imgui_stdlib.h"

/***************************************************************************/
/*!
\brief
    Init function for Menupanel window
*/
/***************************************************************************/
void MenuPanel::init() {
    // empty
}

/***************************************************************************/
/*!
\brief
    Update function for Menupanel window
*/
/***************************************************************************/
void MenuPanel::update() 
{
    static std::string nextGS{};
    if (ImGui::BeginMenuBar())
    {   

        if (ImGui::BeginMenu(ICON_FA_FILE " Files"))
        {       
            if (ImGui::MenuItem(ICON_FA_FILE " Save GameState"))
            {
                // must save both the scenes and gamestate
                systemManager->mGameStateSystem->mCurrentGameState.Save();
                for(int i = 0; i < systemManager->mGameStateSystem->mCurrentGameState.mScenes.size(); ++i)
                    systemManager->mGameStateSystem->mCurrentGameState.mScenes[i].Save();
            }
            if (ImGui::MenuItem(ICON_FA_FILE_ARROW_DOWN " Change GameState"))
            {
            }
            if (ImGui::IsItemClicked()) {
                ImGui::OpenPopup("ChangeState");
            }

            if (ImGui::BeginPopupModal("ChangeState", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Name: ");
                ImGui::InputText("##SceneName", &nextGS);
                ImGui::Separator();

                static bool dont_ask_me_next_time = false;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
                ImGui::PopStyleVar();

                if (ImGui::Button("OK", ImVec2(120, 0))) { 
                    systemManager->mGameStateSystem->ChangeGameState(nextGS);
                    nextGS = {};
                    ImGui::CloseCurrentPopup(); }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { 
                    nextGS = {};
                    ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }


            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ICON_FA_GEAR " Settings"))
        {
            if (ImGui::MenuItem(ICON_FA_KEY " Input"))
            {

                KeybindWindow::openWindow = true;

            }

            if (ImGui::MenuItem(ICON_FA_KEY " Pathfinder Control")) {

              PathfinderWindow::openWindow = true;

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(ICON_FA_VECTOR_SQUARE " Theme"))
        {

            ImVec4* colors = ImGui::GetStyle().Colors;
            if (ImGui::MenuItem("Dark")) {
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

            }
            if (ImGui::MenuItem("White")) {
                colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
                colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
                colors[ImGuiCol_WindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                colors[ImGuiCol_ChildBg] = ImVec4(0.69f, 0.68f, 0.68f, 0.00f);
                colors[ImGuiCol_PopupBg] = ImVec4(0.91f, 0.91f, 0.91f, 0.94f);
                colors[ImGuiCol_Border] = ImVec4(0.86f, 0.86f, 0.86f, 0.50f);
                colors[ImGuiCol_BorderShadow] = ImVec4(0.23f, 0.23f, 0.23f, 0.00f);
                colors[ImGuiCol_FrameBg] = ImVec4(0.65f, 0.65f, 0.65f, 0.54f);
                colors[ImGuiCol_FrameBgHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.40f);
                colors[ImGuiCol_FrameBgActive] = ImVec4(0.57f, 0.57f, 0.57f, 0.84f);
                colors[ImGuiCol_TitleBg] = ImVec4(0.68f, 0.68f, 0.68f, 1.00f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.43f, 0.43f, 0.43f, 0.51f);
                colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                colors[ImGuiCol_ScrollbarBg] = ImVec4(0.77f, 0.77f, 0.77f, 0.41f);
                colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
                colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
                colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
                colors[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
                colors[ImGuiCol_SliderGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
                colors[ImGuiCol_SliderGrabActive] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
                colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
                colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.22f, 1.00f);
                colors[ImGuiCol_ButtonActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
                colors[ImGuiCol_Header] = ImVec4(0.39f, 0.39f, 0.39f, 0.31f);
                colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.87f, 0.00f, 0.80f);
                colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 0.22f);
                colors[ImGuiCol_Separator] = ImVec4(0.69f, 0.69f, 0.69f, 0.50f);
                colors[ImGuiCol_SeparatorHovered] = ImVec4(0.64f, 0.64f, 0.64f, 0.78f);
                colors[ImGuiCol_SeparatorActive] = ImVec4(0.77f, 0.77f, 0.77f, 0.78f);
                colors[ImGuiCol_ResizeGrip] = ImVec4(0.52f, 0.52f, 0.52f, 0.78f);
                colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.78f);
                colors[ImGuiCol_ResizeGripActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.78f);
                colors[ImGuiCol_Tab] = ImVec4(0.38f, 0.38f, 0.38f, 0.86f);
                colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
                colors[ImGuiCol_TabActive] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
                colors[ImGuiCol_TabUnfocused] = ImVec4(0.63f, 0.63f, 0.63f, 0.97f);
                colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                colors[ImGuiCol_DockingPreview] = ImVec4(0.48f, 0.48f, 0.48f, 0.70f);
                colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
                colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
                colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
                colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
                colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
                colors[ImGuiCol_TableHeaderBg] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
                colors[ImGuiCol_TableBorderStrong] = ImVec4(1.00f, 0.33f, 0.01f, 1.00f);
                colors[ImGuiCol_TableBorderLight] = ImVec4(0.04f, 0.04f, 0.05f, 1.00f);
                colors[ImGuiCol_TableRowBg] = ImVec4(0.46f, 0.46f, 0.46f, 0.88f);
                colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
                colors[ImGuiCol_TextSelectedBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
                colors[ImGuiCol_DragDropTarget] = ImVec4(0.72f, 0.72f, 0.72f, 0.90f);
                colors[ImGuiCol_NavHighlight] = ImVec4(0.07f, 0.04f, 0.00f, 1.00f);
                colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
                colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
                colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

            }

            if (ImGui::MenuItem("CyberPunk")) {

                colors[ImGuiCol_Text] = ImVec4(0.47f, 0.79f, 0.89f, 1.00f);
                colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
                colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.01f, 0.16f, 1.00f);
                colors[ImGuiCol_ChildBg] = ImVec4(0.72f, 0.44f, 0.44f, 0.00f);
                colors[ImGuiCol_PopupBg] = ImVec4(0.30f, 0.05f, 0.30f, 0.94f);
                colors[ImGuiCol_Border] = ImVec4(0.31f, 0.10f, 0.33f, 0.50f);
                colors[ImGuiCol_BorderShadow] = ImVec4(0.46f, 0.04f, 0.04f, 0.00f);
                colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.08f, 0.08f, 0.54f);
                colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.12f, 0.12f, 0.40f);
                colors[ImGuiCol_FrameBgActive] = ImVec4(0.27f, 0.00f, 0.00f, 0.84f);
                colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.00f, 0.15f, 1.00f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0.17f, 0.00f, 0.16f, 1.00f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.22f, 0.01f, 0.21f, 0.51f);
                colors[ImGuiCol_MenuBarBg] = ImVec4(0.17f, 0.01f, 0.20f, 1.00f);
                colors[ImGuiCol_ScrollbarBg] = ImVec4(0.04f, 0.01f, 0.02f, 0.41f);
                colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.32f, 0.32f, 1.00f);
                colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
                colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
                colors[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
                colors[ImGuiCol_SliderGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
                colors[ImGuiCol_SliderGrabActive] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
                colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
                colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.22f, 1.00f);
                colors[ImGuiCol_ButtonActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
                colors[ImGuiCol_Header] = ImVec4(0.64f, 0.18f, 0.57f, 0.31f);
                colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.87f, 0.00f, 0.80f);
                colors[ImGuiCol_HeaderActive] = ImVec4(0.23f, 0.00f, 0.00f, 0.22f);
                colors[ImGuiCol_Separator] = ImVec4(0.83f, 0.00f, 0.98f, 0.50f);
                colors[ImGuiCol_SeparatorHovered] = ImVec4(0.64f, 0.64f, 0.64f, 0.78f);
                colors[ImGuiCol_SeparatorActive] = ImVec4(0.77f, 0.77f, 0.77f, 0.78f);
                colors[ImGuiCol_ResizeGrip] = ImVec4(0.52f, 0.52f, 0.52f, 0.78f);
                colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.78f);
                colors[ImGuiCol_ResizeGripActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.78f);
                colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.00f, 0.00f, 0.86f);
                colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 0.27f, 0.82f, 0.80f);
                colors[ImGuiCol_TabActive] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f);
                colors[ImGuiCol_TabUnfocused] = ImVec4(0.33f, 0.01f, 0.26f, 0.97f);
                colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.82f, 0.00f, 0.59f, 1.00f);
                colors[ImGuiCol_DockingPreview] = ImVec4(0.48f, 0.48f, 0.48f, 0.70f);
                colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.69f, 0.23f, 0.23f, 1.00f);
                colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
                colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.85f, 0.00f, 1.00f, 1.00f);
                colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
                colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
                colors[ImGuiCol_TableHeaderBg] = ImVec4(0.61f, 0.00f, 0.66f, 1.00f);
                colors[ImGuiCol_TableBorderStrong] = ImVec4(1.00f, 0.33f, 0.01f, 1.00f);
                colors[ImGuiCol_TableBorderLight] = ImVec4(0.04f, 0.04f, 0.05f, 1.00f);
                colors[ImGuiCol_TableRowBg] = ImVec4(0.20f, 0.00f, 0.34f, 0.88f);
                colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
                colors[ImGuiCol_TextSelectedBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
                colors[ImGuiCol_DragDropTarget] = ImVec4(0.72f, 0.72f, 0.72f, 0.90f);
                colors[ImGuiCol_NavHighlight] = ImVec4(0.07f, 0.04f, 0.00f, 1.00f);
                colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
                colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
                colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(ICON_FA_ARROW_TREND_DOWN " Shader"))
        {
            if (ImGui::MenuItem(ICON_FA_ID_CARD " Compile Shader"))
            {

                ShaderCompiler::openWindow = true;

            }
            ImGui::EndMenu();
        }


        ImGui::EndMenuBar();
    }
}
