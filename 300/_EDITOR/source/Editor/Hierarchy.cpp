/*!*************************************************************************
****
\file LeftPanel.cpp
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 22-9-2022
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
#include "imgui_stdlib.h"

//#define DEBUG



entt::entity Hierarchy::selectedId;
entt::entity Hierarchy::RselectedId;
int Hierarchy::selectedScene;
int Hierarchy::RselectedScene;
bool Hierarchy::selectionOn;

void Hierarchy::init() {

}
//int Hierarchy::selectCnt{ -1 };


#ifdef DEBUG

void Hierarchy::update()
{
    auto allObjects = systemManager->ecs->GetEntitiesWith<Transform>();

    int i = allObjects.size();

    if (ImGui::Button("Add", ImVec2(50, 50)))
    {
        Entity newEntity = systemManager->ecs->NewEntity();
        

        newEntity.GetComponent<General>().name = "NewObject"/* + static_cast<int> (newEntity.id)*/;
    }


    if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginDragDropTarget()) {
            //ImGui::SetDragDropPayload("PARENT_CHILD", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PREFAB")) {
                // auto payload = ImGui::AcceptDragDropPayload("PARENT_CHILD");

                auto data = (const char*)payload->Data;


                std::string newdata(data);
                systemManager->ecs->NewEntityFromPrefab(newdata);
                //Entity tempEnt(RselectedId);
                //systemManager->ecs->NewPrefab(tempEnt);
            }
            ImGui::EndDragDropTarget();
        }


        int i = 5462;
        for (Entity ent : allObjects)													
        {
            if (ent.HasParent() == false&& ent.HasChildren() == false) {

                General& info = ent.GetComponent<General>();
                ImGui::PushID(i);
            
                ImGuiWindowFlags selectflag{ 0 };
                if (ent.id == selectedId)
                    selectflag |= ImGuiTreeNodeFlags_Selected;
                
                ImGui::TreeNodeEx((info.name /*+std::to_string(i)*/).c_str(), selectflag|ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf);
                ImGui::PopID();

                //--------------------------------------------------------------------------// Select Object

                if (!ent.HasChildren()) {

                    if (ImGui::BeginDragDropSource()) {

                        //auto data = ent.id;
                        //const char* source_path = "CHILD";
                        ImGui::SetDragDropPayload("PARENT_CHILD", &ent.id, sizeof(ent.id), ImGuiCond_Once);

                        ImGui::EndDragDropSource();
                    }
                
                }

                if (ImGui::BeginDragDropTarget()) {

               
                    //ImGui::SetDragDropPayload("PARENT_CHILD", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENT_CHILD")) {
                       // auto payload = ImGui::AcceptDragDropPayload("PARENT_CHILD");

                        auto data = static_cast<entt::entity*> (payload->Data);
                        std::cout << "im here for thed ata\n";
                        ent.AddChild(*data);
                    }
                    ImGui::EndDragDropTarget();
                }





                if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
                    selectionOn = true;
                    selectedId = ent.id;
                }
                //--------------------------------------------------------------------------// Delete Object
                if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                    mPopup = true;
                    RselectedId = ent.id;
                }
            }
            



                //-------------------------------------------------------------------------------------// 
                //-------------------------------------------------------------------------------------// 

            if (ent.HasChildren()==true) {

                ImGuiWindowFlags selectflag{ 0 };
                if (ent.id == selectedId)
                    selectflag |= ImGuiTreeNodeFlags_Selected; 
                if(ImGui::TreeNodeEx((ent.GetComponent<General>().name).c_str(), selectflag|ImGuiTreeNodeFlags_OpenOnDoubleClick| ImGuiTreeNodeFlags_DefaultOpen)) {

                    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
                        selectionOn = true;
                        selectedId = ent.id;
                    }

                    if (ImGui::BeginDragDropTarget()) {
                        //ImGui::SetDragDropPayload("PARENT_CHILD", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENT_CHILD")) {
                            // auto payload = ImGui::AcceptDragDropPayload("PARENT_CHILD");

                            auto data = static_cast<entt::entity*> (payload->Data);
                            std::cout << "im here for thed ata\n";
                            ent.AddChild(*data);
                        }
                        ImGui::EndDragDropTarget();
                    }
                    auto allchild = ent.GetAllChildren();

                    for (auto& child : allchild) {
                        ImGuiWindowFlags selectflag{ 0 };
                        if (child.id == selectedId)
                            selectflag |= ImGuiTreeNodeFlags_Selected;

                        ImGui::TreeNodeEx((child.GetComponent<General>().name + "xiaohai").c_str(), selectflag | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf);

                        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                            RselectedId = child.id;
                            mCPopup = true;
                        }

                        if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
                            selectionOn = true;
                            selectedId = child.id;
                        }

                    }
                    ImGui::TreePop();

                }
                        //if(allchild.size()>=1 )  
                
                }
            i++; // for id
            
        }

        //-----------------------------------------------------------------------------------------------------// End of Deletion of entity

        ImGui::TreePop();
    }


    //-----------------------------------------------------------------------------------------------------------// Popup for Child

    if(mCPopup)
        ImGui::OpenPopup("Edit_entity", ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup("Edit_entity"))
    {
        if (ImGui::Selectable("Delete")) {
           // selectionOn = false;
            //Entity ent(Hierarchy::selectedId);
           // systemManager->ecs->DeleteEntity(Hierarchy::selectedId);
        }
        if (ImGui::Selectable("RemoveChild")) {

            Entity child = RselectedId;
            child.GetParent().RemoveChild(child);
            //  selectionOn = false;

            //ntity tempEnt(RselectedId);

            // tempEnt.GetParent().;


            //systemManager->ecs->DeleteEntity(Hierarchy::selectedId);
        }
        mCPopup = false;
        ImGui::EndPopup();
    }
    mCPopup = false;
    //-----------------------------------------------------------------------------------------------------------// Popup for Parent

    if(mPopup)
        ImGui::OpenPopup("Delete_entity", ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup("Delete_entity"))
    {
        if (ImGui::Selectable("Delete")) {
            selectionOn = false;
            //Entity ent(Hierarchy::selectedId);
            systemManager->ecs->DeleteEntity(Hierarchy::RselectedId);
        }
        if (ImGui::Selectable("Prefab")) {
        //    selectionOn = false;

            Entity tempEnt(RselectedId);
            systemManager->ecs->NewPrefab(tempEnt);
        //   // tempEnt.GetParent().;


        //    //systemManager->ecs->DeleteEntity(Hierarchy::selectedId);
        }


        mPopup = false;
        ImGui::EndPopup();
    }
    mPopup = false;
}

#endif


#ifndef DEBUG

void Hierarchy::update() {
    

    ImGui::InputText(" ", &systemManager->mGameStateSystem->mCurrentGameState.mName);
    // ImGui::Text(systemManager->mGameStateSystem->mCurrentGameState.mName.c_str());
    //auto allObjects = systemManager->ecs->GetEntitiesWith<Transform>();

    //int i = allObjects.size();

    //for (Entity ent : allObjects)
    //{
    //    if (ent.HasParent() == false && ent.HasChildren() == false) {
    //        General& info = ent.GetComponent<General>();

    //        ImGui::TreeNodeEx((info.name /*+std::to_string(i)*/).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf);

    //    }

    //}
    auto& allScene = systemManager->mGameStateSystem->mCurrentGameState.mScenes;

    //if (ImGui::Button("AddS", ImVec2(50, 50)))
    //{
    //  //  Entity newEntity = systemManager->ecs->NewEntity();    

    //}

    if (ImGui::Button("Add", ImVec2(50, 50)))
    {
        //  Entity newEntity = systemManager->ecs->NewEntity();
      //  if (allScene.size() <= 0)
        //    systemManager->mGameStateSystem->mCurrentGameState.AddScene("NewScene");

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
        systemManager->mGameStateSystem->mCurrentGameState.AddScene("NewScene"+ std::to_string(allScene.size()));

    }


    for (int i{ 0 }; i < allScene.size(); ++i) {
    

        ImGuiWindowFlags selectflagscene{ 0 };
        if (i == selectedScene)
            selectflagscene |= ImGuiTreeNodeFlags_Selected;

        if (ImGui::TreeNodeEx(allScene[i].mName.c_str(), selectflagscene| ImGuiTreeNodeFlags_DefaultOpen| ImGuiTreeNodeFlags_OpenOnDoubleClick))
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

                            //auto data = ent.id;
                            //const char* source_path = "CHILD";
                            ImGui::SetDragDropPayload("PARENT_CHILD", &ent.id, sizeof(ent.id), ImGuiCond_Once);

                            ImGui::EndDragDropSource();
                        }

                    }

                    if (ImGui::BeginDragDropTarget()) {


                        //ImGui::SetDragDropPayload("PARENT_CHILD", source_path, strlen(source_path) * sizeof(wchar_t), ImGuiCond_Once);

                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENT_CHILD")) {
                            // auto payload = ImGui::AcceptDragDropPayload("PARENT_CHILD");

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
                            ImGuiWindowFlags selectflag{ 0 };
                            if (child.id == selectedId)
                                selectflag |= ImGuiTreeNodeFlags_Selected;

                            ImGui::TreeNodeEx((child.GetComponent<General>().name).c_str(), selectflag | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf);

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

                    //if(allchild.size()>=1 )  

                }

            }

            ImGui::TreePop();
        }
    }



    




    if (mCPopup)
        ImGui::OpenPopup("Edit_entity", ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup("Edit_entity"))
    {
        if (ImGui::Selectable("Delete")) {
            // selectionOn = false;
             //Entity ent(Hierarchy::selectedId);
            // systemManager->ecs->DeleteEntity(Hierarchy::selectedId);
            systemManager->mGameStateSystem->mCurrentGameState.mScenes[selectedScene].mEntities.erase(Hierarchy::selectedId);
            systemManager->ecs->DeleteEntity(Hierarchy::selectedId);


        }
        if (ImGui::Selectable("RemoveChild")) {

            Entity child = RselectedId;
            child.GetParent().RemoveChild(child);
            //  selectionOn = false;

            //ntity tempEnt(RselectedId);

            // tempEnt.GetParent().;

            //systemManager->ecs->DeleteEntity(Hierarchy::selectedId);

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
        if (ImGui::Selectable("Delete")) {


             
            selectionOn = false;

             if (systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].mEntities.size() > 0) {

                 for (int i{ 0 }; i < systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].mEntities.size(); i++) {
                     systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].mEntities.clear();
                 }
             }

             systemManager->mGameStateSystem->
                 mCurrentGameState.RemoveScene(systemManager->mGameStateSystem->mCurrentGameState.mScenes[RselectedScene].mName);
             //Entity ent(Hierarchy::selectedId);
            // systemManager->ecs->DeleteEntity(Hierarchy::selectedId);
        }
       
        sCPopup = false;
        ImGui::EndPopup();
    }
    sCPopup = false;



}

#endif // 1
