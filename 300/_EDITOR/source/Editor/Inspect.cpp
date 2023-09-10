/*!*************************************************************************
****
\file Inspect.cpp
\author Han ChengWen, Charissa Yip, Lwin Li Yue Sandi
\par DP email: c.han@\digipen.edu, charissa.yip\@digipen.edu, l.liyuesandi\@digipen.edu
\date 22-9-2022
\brief
This header file contains the inspector window and relative functions to run
the system logic.

The functions

- init
Initialize related data needed for Inspector window to draw

- update
Run Inspector logic that checks for component and render related data which
provides user to edit in run time

- tags
Tagging system logic allows User to edit and add tags

- TagPosition
Return tag position (tag id)

- GetTagName
Return tag name (tag string)

- Add_component
Add component button and logic implementation that allows user to add
components

- Transform::Inspect
Inspect display for Transform component
Included ImGui custom dragfloat3 function for data required in command system.

-Collider::Inspect
Inspect display for Collider component
Included ImGui custom dragfloat3 function for data required in command system.

- BoxCollider::Inspect
Inspect display for BoxCollider component
Included ImGui custom dragfloat3 function for data required in command system.

- CircleCollider::Inspect
Inspect display for CircleCollider component
Included ImGui custom dragfloat3 function for data required in command system.

- Texture::Inspect
Inspect display for Texture components
Error pop up will appear when user attempts to include the wrong asset type.

- Camera::Inspect
Inspect display for Camera components
Included ImGui custom dragfloat3 function for data required in command system.

-RigidBody::Inspect
Inspect display for RigidBody components
****************************************************************************
***/

#include "Inspect.h"
#include "Editor.h"
#include "ECS_Components.h"
#include "Hierarchy.h"
#include <math.h>
//int Inspect::inspectmode{ inspector_layer };
//int Inspect::scaleCounter = 0;
//bool prev{}, current{};
//bool Inspect::isEditName = false;
//For future implementation of undo/redo
//bool Inspect::entityFlag = false;

void Inspect::init() {
//empty
}

void Inspect::update() 
{


	if (Hierarchy::selectionOn) {
		
		Entity ent(Hierarchy::selectedId);

		if (ent.HasComponent<Transform>()) {
			Transform transform = ent.GetComponent<Transform>();
			transform.Inspect();
		}



		//--------------------------------------------// must be at the LAST OF THIS LOOP
		Add_component(); 
	}



}

void Inspect::Add_component() {


	auto initialCursorPos = ImGui::GetCursorPos();
	auto windowSize = ImGui::GetWindowSize();
	float centralizedCursorpos = (windowSize.x - 200) * 0.5f;
	//centralizedCursorpos = std::clamp(centralizedCursorpos, initialCursorPos.x, centralizedCursorpos);
	ImGui::SetCursorPosX(centralizedCursorpos);


	ImGui::Button(" Add Component ", ImVec2(200, 30));



	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {

		mPopup = true;

	}

	if (mPopup)
	{
		ImGui::OpenPopup("ComponentList", ImGuiPopupFlags_MouseButtonRight);
	}

	if (ImGui::BeginPopup("ComponentList"))
	{
		if (ImGui::Selectable("Test Component"))
		{

		}
		ImGui::EndPopup();
	}

	mPopup = false; 
}


void Transform::Inspect() {
	if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen) {


		//ImGui::SetCursorPosX(windowWidth / 3.f);
		ImGui::DragFloat3("##Position", (float*)&mTranslate);
		
		ImGui::SameLine();
		ImGui::Text("Position");
		ImGui::Separator();


		ImGui::DragFloat3("##Scale", (float*)&mScale, 1);
		ImGui::SameLine();
		ImGui::Text("Scale");
		ImGui::Separator();


		ImGui::DragFloat3("##Rotation", (float*)&mRotate, 1);
		ImGui::SameLine();
		ImGui::Text("Rotation");

		ImGui::DragFloat("daa", &data, 1);


	}

}


