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
#include "../../../_SCRIPTING/include/ScriptingSystem.h"


entt::entity Hierarchy::selectedId;
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

        for (Entity ent : allObjects)													
        {
            General& info = ent.GetComponent<General>();
            ImGui::TreeNodeEx(info.name.c_str(),
                ImGuiTreeNodeFlags_NoTreePushOnOpen| ImGuiTreeNodeFlags_Leaf);


            //--------------------------------------------------------------------------// Select Object

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
                selectionOn = true;
                selectedId = ent.id;                
            }
            //--------------------------------------------------------------------------// Delete Object

            if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {

                mPopup= true;
            }
        }

        //-----------------------------------------------------------------------------------------------------// End of Deletion of entity

        ImGui::TreePop();
    }


    if(mPopup)
        ImGui::OpenPopup("Delete_entity", ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup("Delete_entity"))
    {
        if (ImGui::Selectable("Delete")) {
            selectionOn = false;
            //Entity ent(Hierarchy::selectedId);
            systemManager->ecs->DeleteEntity(Hierarchy::selectedId);
        }
        mPopup = false;
        ImGui::EndPopup();
    }
    mPopup = false;
}
