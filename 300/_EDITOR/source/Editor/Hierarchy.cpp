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


entt::entity Hierarchy::selectedId;
entt::entity Hierarchy::RselectedId;
bool Hierarchy::selectionOn;

void Hierarchy::init() {}
//int Hierarchy::selectCnt{ -1 };

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
                            mPopup = true;
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
            systemManager->ecs->DeleteEntity(Hierarchy::selectedId);
        }
        if (ImGui::Selectable("RemoveChild")) {
            selectionOn = false;

           //ntity tempEnt(RselectedId);

           // tempEnt.GetParent().;


            //systemManager->ecs->DeleteEntity(Hierarchy::selectedId);
        }


        mPopup = false;
        ImGui::EndPopup();
    }
    mPopup = false;
}


//void update() {
//
//}