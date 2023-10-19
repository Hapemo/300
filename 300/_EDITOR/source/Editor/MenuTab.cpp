#include "MenuTab.h"
#include "Hierarchy.h"

void MenuTab::init() {


    mWinFlag = ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar  | ImGuiWindowFlags_NoScrollbar;

}

void MenuTab::update() {

    ImVec2 buttonSize{ 50,50 };

    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - buttonSize.x * 1.5);

    if (ImGui::Button(ICON_FA_CIRCLE_PLAY, buttonSize)) {
        systemManager->Play();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_CIRCLE_PAUSE, buttonSize)) {  
        systemManager->Pause();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_CIRCLE_XMARK, buttonSize)) {
        Hierarchy::selectionOn = false;
        systemManager->Reset();
    }
}