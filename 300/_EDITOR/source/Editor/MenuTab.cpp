#include "MenuTab.h"
#include "Hierarchy.h"
#include "ScriptingSystem.h"

void MenuTab::init() {


    mWinFlag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

}

void MenuTab::update() {

    ImVec2 buttonSize{ 50,50 };

    ImGui::SetCursorPosX( static_cast<float>((ImGui::GetWindowSize().x / 2) - (buttonSize.x * 1.5)) );

    if (ImGui::Button(ICON_FA_CIRCLE_PLAY, buttonSize)) {
        PINFO("PLAY")
        systemManager->Play();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_CIRCLE_PAUSE, buttonSize)) {
        PINFO("PAUSED");
        systemManager->mIsPlay = false;
        systemManager->Pause();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_CIRCLE_XMARK, buttonSize)) {
        PINFO("RESET")
        Hierarchy::selectionOn = false;
        systemManager->Reset();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_ROTATE_RIGHT, buttonSize)) {
        //Reload Helper.lua
        systemManager->GetScriptingPointer()->LoadHelper();

        auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();

        for (Entity entity : scriptEntities)
        {
            for (Script* script : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
            {
                script->Load(entity);
                script->Run("Alive");
            }
        }
    }
}