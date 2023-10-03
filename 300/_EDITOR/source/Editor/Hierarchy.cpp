/*!*************************************************************************
****
\file Hierarchy.cpp
\author Han ChengWen, Kew Yu Jun, Charissa Yip, Jazz Teoh Yu Jue
\par DP email: c.han@digipen.edu, charissa.yip@digipen.edu
\date 28-9-2023
\brief
This header file contains the Leftpanel(Hierarchy) window and relative functions to run
the system logic, allowing user to select Entites, opening up Inspector window and modify
related variables.

The functions

- init
Initialize related data needed for Leftpanel window to draw

- update
Run Leftpanel logic that checks for Entity and render related data allows user
to select current Entity and activates inspector

****************************************************************************
***/

#include "imgui.h"
#include "Hierarchy.h"
#include "ScriptingSystem.h"
#include "GameState/GameStateManager.h"
#include "Misc.h"
#include "imgui_stdlib.h"

//#define DEBUG



entt::entity Hierarchy::selectedId;
entt::entity Hierarchy::RselectedId;
int Hierarchy::selectedScene;
int Hierarchy::RselectedScene;
bool Hierarchy::selectionOn;

/***************************************************************************/
/*!
\brief
    Init function for Imgui Window
*/
/**************************************************************************/
void Hierarchy::init() {
    // empty
}



#ifndef DEBUG
/***************************************************************************/
/*!
\brief
    Update function for Imgui Window
*/
/**************************************************************************/
void Hierarchy::update() {
    
    copyPaste();
    ImGui::Text("GameState");
    ImGui::SameLine();

    ImGui::InputText(" ", &systemManager->mGameStateSystem->mCurrentGameState.mName);

    auto& allScene = systemManager->mGameStateSystem->mCurrentGameState.mScenes;



    if (ImGui::Button("Add", ImVec2(50, 50)))
    {

        if (allScene.size() <= 0) {
            systemManager->mGameStateSystem->mCurrentGameState.AddScene("NewScene");
            Entity newEntity = allScene[0].AddEntity();

        }
        else {
            Entity newEntity = allScene[Hierarchy::selectedScene].AddEntity();
            newEntity.GetComponent<General>().name = "NewObject"/* + static_cast<int> (newEntity.id)*/;

        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Scene", ImVec2(50, 50))) {
        systemManager->mGameStateSystem->mCurrentGameState.AddScene();

    }


    for (int i{ 0 }; i < allScene.size(); ++i) {
    

        ImGuiWindowFlags selectflagscene{ 0 };
        if (i == selectedScene)
            selectflagscene |= ImGuiTreeNodeFlags_Selected;

        if (ImGui::TreeNodeEx(allScene[i].mName.c_str(), selectflagscene | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick))
        {

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {  
                selectedScene = i;
            }
            //--------------------------------------------------------------------------// Delete Object
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                RselectedScene = i;
                sCPopup = true;
            }

            if (ImGui::BeginDragDropTarget()) {

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PREFAB")) {
                    auto data = (const char*)payload->Data;
                    std::string newdata(data);
                    Entity newent = systemManager->ecs->NewEntityFromPrefab(newdata);
                }
                ImGui::EndDragDropTarget();
            }

            int imguid{0};
            for (Entity ent : allScene[i].mEntities) {


                if (ent.HasParent() == false && ent.HasChildren() == false) {

                    General& info = ent.GetComponent<General>();
                    ImGui::PushID(imguid);

                    ImGuiWindowFlags selectflag{ 0 };
                    if (ent.id == selectedId)
                        selectflag |= ImGuiTreeNodeFlags_Selected;

                    ImGui::TreeNodeEx((info.name /*+std::to_string(i)*/).c_str(), selectflag | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf);
                    ImGui::PopID();
                    imguid++;

                    if (!ent.HasChildren()) {

                        if (ImGui::BeginDragDropSource()) {

                            ImGui::SetDragDropPayload("PARENT_CHILD", &ent.id, sizeof(ent.id), ImGuiCond_Once);

                            ImGui::EndDragDropSource();
                        }

                    }

                    if (ImGui::BeginDragDropTarget()) {

                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENT_CHILD")) {

                            auto data = static_cast<entt::entity*> (payload->Data);    
                            ent.AddChild(*data);
                        }
                        ImGui::EndDragDropTarget();
                    }

                    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
                        selectedScene = i;
                        selectionOn = true;
                        selectedId = ent.id;
                    }
                    //--------------------------------------------------------------------------// Delete Object
                    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                        RselectedScene = i;
                        mPopup = true;
                        RselectedId = ent.id;
                    }
                }

                if (ent.HasChildren() == true) {

                    ImGuiWindowFlags selectflag{ 0 };
                    if (ent.id == selectedId)
                        selectflag |= ImGuiTreeNodeFlags_Selected;
                    ImGui::PushID(imguid);
                    if (ImGui::TreeNodeEx((ent.GetComponent<General>().name).c_str(), selectflag | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen)) {

                        if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
                            selectedScene = i;
                            selectionOn = true;
                            selectedId = ent.id;
                        }

                        if (ImGui::BeginDragDropTarget()) {
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENT_CHILD")) {

                                auto data = static_cast<entt::entity*> (payload->Data);
                                ent.AddChild(*data);
                            }
                            ImGui::EndDragDropTarget();
                        }
                        auto allchild = ent.GetAllChildren();

                        for (auto& child : allchild) {
                            ImGuiWindowFlags selectflag1{ 0 };
                            if (child.id == selectedId)
                                selectflag1 |= ImGuiTreeNodeFlags_Selected;

                            ImGui::TreeNodeEx((child.GetComponent<General>().name).c_str(), selectflag1 | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf);

                            if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                                RselectedScene = i;
                                RselectedId = child.id;
                                mCPopup = true;
                            }

                            if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
                                selectedScene = i;
                                selectionOn = true;
                                selectedId = child.id;
                            }

                        }
                        ImGui::TreePop();

                    }
                    ImGui::PopID();
                }

            }

            ImGui::TreePop();
        }


    }


    ImGui::Dummy(ImGui::GetContentRegionAvail());
    

    if (ImGui::BeginDragDropTarget()) {

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_SCN")) {
            auto data = (const char*)payload->Data;
            std::string name = Misc::GetFileName(std::string(data));

            systemManager->mGameStateSystem->AddScene(name);
        }
        ImGui::EndDragDropTarget();
    }



    if (mCPopup)
        ImGui::OpenPopup("Edit_entity", ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup("Edit_entity"))
    {
        if (ImGui::Selectable("Delete")) {
            systemManager->mGameStateSystem->mCurrentGameState.mScenes[selectedScene].mEntities.erase(Hierarchy::selectedId);
            systemManager->ecs->DeleteEntity(Hierarchy::selectedId);
        }
        if (ImGui::Selectable("RemoveChild")) {

            Entity child = RselectedId;
            child.GetParent().RemoveChild(child);

        }
        mCPopup = false;
        ImGui::EndPopup();
    }
    mCPopup = false;
    //-----------------------------------------------------------------------------------------------------------// Popup for Parent

    if (mPopup)
        ImGui::OpenPopup("Delete_entity", ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup("Delete_entity"))
    {
        if (ImGui::Selectable("Delete")) {
            selectionOn = false;

            Entity ent(RselectedId);

            auto allchild = ent.GetAllChildren();

            for (auto& child : allchild) {

                systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].mEntities.erase(child.id);
            }

            systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].mEntities.erase(Hierarchy::RselectedId);
            systemManager->ecs->DeleteEntity(Hierarchy::RselectedId);

        }
        if (ImGui::Selectable("Prefab")) {
            Entity tempEnt(RselectedId);
            systemManager->ecs->NewPrefab(tempEnt);
        }
        mPopup = false;
        ImGui::EndPopup();
    }
    mPopup = false;



    if (sCPopup)
        ImGui::OpenPopup("Edit_scene", ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup("Edit_scene"))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

        if (ImGui::Button("Rename"))
        {

            ImGui::OpenPopup("Delete?");
        }

        ImGui::PopStyleVar();
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Name: ");
            ImGui::InputText("##SceneName", &systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].mName);
            ImGui::Separator();

            static bool dont_ask_me_next_time = false;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
            ImGui::PopStyleVar();

            if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        if (ImGui::Selectable("Save"))
        {
            systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].Save();
        }
        if (ImGui::Selectable("Delete")) {
            
            selectionOn = false;
             if (systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].mEntities.size() > 0) {

                 for (int i{ 0 }; i < systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].mEntities.size(); i++) {
                   systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].Unload();
                 }
             }
             systemManager->mGameStateSystem->
                 mCurrentGameState.RemoveScene(systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].mName);
        }
       
        sCPopup = false;
        ImGui::EndPopup();
    }
    sCPopup = false;


    // Always center this window when appearing
   
}

/***************************************************************************/
/*!
\brief
    Copy function for objects in hierarchy (only within scene)
*/
/**************************************************************************/
void Hierarchy::copyPaste() {
    if (Input::CheckKey(E_STATE::HOLD, E_KEY::LEFT_CONTROL))
    {
        if (Input::CheckKey(E_STATE::PRESS, E_KEY::C))
        {
            systemManager->ecs->CopyEntity(Hierarchy::selectedId);
        }
        else if (Input::CheckKey(E_STATE::PRESS, E_KEY::V))
        {
            Hierarchy::selectedId = systemManager->ecs->PasteEntity(Hierarchy::selectedScene).id;
        }
    }
}
#endif // 1
