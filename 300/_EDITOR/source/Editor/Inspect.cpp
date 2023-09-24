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
#include "ECS/ECS_Components.h"
#include "ECS/ECS_Systems.h"
#include "Hierarchy.h"
#include <math.h>
#include "imgui_stdlib.h"
#include "../../../_SCRIPTING/include/ScriptingSystem.h"
#include "Guid.h"
#include "ResourceManagerTy.h"
#include "../../../_ENGINE/include/Debug/Logger.h"


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
			Transform& transform = ent.GetComponent<Transform>();
			transform.Inspect();
		}
		if (ent.HasComponent<Scripts>())
		{
			Scripts& scripts = ent.GetComponent<Scripts>();
			scripts.Inspect();
		}

		if (ent.HasComponent<MeshRenderer>()) {
			MeshRenderer& Meshrender = ent.GetComponent<MeshRenderer>();
			Meshrender.Inspect();
		}
		if (ent.HasComponent<RigidBody>()) {
			RigidBody& rigidBody = ent.GetComponent<RigidBody>();
			rigidBody.Inspect();
		}
		if (ent.HasComponent<BoxCollider>()) {
			BoxCollider& boxCollider = ent.GetComponent<BoxCollider>();
			boxCollider.Inspect();
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
		//if (ImGui::Selectable("Test Component"))
		//{

		//}
		if (ImGui::Selectable("RigidBody")) {
			if(!Entity(Hierarchy::selectedId).HasComponent<RigidBody>())
				Entity(Hierarchy::selectedId).AddComponent<RigidBody>();
		}

		if (ImGui::Selectable("BoxCollider")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<BoxCollider>())
				Entity(Hierarchy::selectedId).AddComponent<BoxCollider>();
		}


		if (ImGui::Selectable("Scripts")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<Scripts>())
				Entity(Hierarchy::selectedId).AddComponent<Scripts>();
		}
		ImGui::EndPopup();
	}

	mPopup = false; 
}


void Transform::Inspect() {
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {


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



	}

}

void Scripts::Inspect() {
	bool delete_component = true;
	const char* data_script{};
	static std::string newScript;
	static bool open_popup{ false };
	static std::string deleteScript;

	auto scriptEntities = systemManager->ecs->GetEntitiesWith<Scripts>();
	Scripts& scripts = scriptEntities.get<Scripts>(Hierarchy::selectedId);

	if (ImGui::CollapsingHeader("Scripts"), &delete_component, ImGuiTreeNodeFlags_DefaultOpen)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_LUA"))
			{
				data_script = (const char*)payload->Data;
				scripts.mScriptFile = std::string(data_script);
				std::string dataScript = std::string(data_script);

				// if entity does not contain any script, just add 
				if (scriptEntities.get<Scripts>(Hierarchy::selectedId).scriptsContainer.size() == 0)
				{
					Script script;
					script.scriptFile = dataScript;
					script.env = { systemManager->mScriptingSystem->luaState, sol::create, systemManager->mScriptingSystem->luaState.globals() };
					scripts.scriptsContainer.push_back(script);
					//std::cout << "Script " << script.scriptFile << ".lua added to entity " << std::to_string((int)Hierarchy::selectedId) << std::endl;
				}
				// if entity already has scripts attached, check if duplicate 
				else
				{
					bool hasScript{ };

					for (auto& elem : scripts.scriptsContainer)
					{
						if (elem.scriptFile == scripts.mScriptFile)
						{
							hasScript = true;
							//std::cout << "Script is already attached! " << std::endl;
							PWARNING("Script is already attached! ");
							break;
						}
					}

					if (!hasScript)
					{
						Script script;
						script.scriptFile = dataScript;
						script.env = { systemManager->mScriptingSystem->luaState, sol::create, systemManager->mScriptingSystem->luaState.globals() };
						scripts.scriptsContainer.push_back(script);
						//std::cout << "Script " << script.scriptFile << ".lua added to entity " << std::to_string((int)Hierarchy::selectedId) << std::endl;
						PINFO("Script %s added to entity %s", script.scriptFile.c_str(), std::to_string((int)Hierarchy::selectedId).c_str());
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::Text("Drag drop scripts to header above 'Scripts'");
		ImGui::Text("Entity contains scripts: ");
		for (auto& elem : scripts.scriptsContainer)
		{
			bool selected{};
			ImGui::Selectable(elem.scriptFile.c_str(), &selected);
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			}
			if (ImGui::IsItemClicked(1)) {
				open_popup = true;
				deleteScript = elem.scriptFile;
			}
		}

		if (open_popup) {
			ImGui::OpenPopup("Delete_Script", ImGuiPopupFlags_MouseButtonRight);
		}
		if (ImGui::BeginPopup("Delete_Script"))
		{
			if (ImGui::Selectable("Delete"))
			{
				for (auto i = 0; i < scripts.scriptsContainer.size(); i++)
				{
					if (scripts.scriptsContainer[i].scriptFile == deleteScript)
					{
						scripts.scriptsContainer.erase(scripts.scriptsContainer.begin() + i);
					}
				}
				open_popup = false;
			}
			ImGui::EndPopup();
		}
		open_popup = false;

		ImGui::Text("Create new script: ");
		ImGui::InputText(".lua", &newScript);
		if (ImGui::Button("Add Lua Script"))
		{
			std::ofstream output;
			std::ifstream input{ "../assets/Scripts/DefaultTemplate.lua" };
			std::stringstream ss;
			std::string lua = ".lua";
			std::string line;
			ss << "../assets/Scripts/" << newScript << ".lua";
			std::cout << ss.str() << std::endl;
			std::string pathInString = ss.str();
			const char* path = pathInString.c_str();
			output.open(path, std::ios_base::out);
			while (getline(input, line))
			{
				output << line << std::endl;
			}
			input.close();
			newScript = " ";
			ImGui::InputText(".lua", &newScript);
		}
	}

}

void MeshRenderer::Inspect() {

	if (ImGui::CollapsingHeader("MeshRenderer", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::Text(mMeshPath.c_str());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_GEOM")) {

				const char* data = (const char*)payload->Data;
				std::string data_str = std::string(data);
				mMeshPath = data_str;

				uid temp(mMeshPath);
				mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(temp.id));

			}
			ImGui::EndDragDropTarget();
		}
	}

}


void RigidBody::Inspect() {
	ImGui::DragFloat("##Density", (float*)&mDensity);

	ImGui::SameLine();
	ImGui::Text("Density");
	ImGui::Separator();


	const char* materials[] = { "RUBBER", "WOOD", "METAL", "ICE","CONCRETE","GLASS" };
	const char* motions[] = { "STATIC", "DYNAMIC" };


	if(ImGui::BeginCombo("Material", (materials[mMat]))){

		for (unsigned char i{ 0 }; i < 6; i++) {
			if (ImGui::Selectable(materials[i])) {
					mMat = i;
					mMaterial = (MATERIAL)i;
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Motions", (motions[mMot]))) {

		for (unsigned char i{ 0 }; i < 2; i++) {
			if (ImGui::Selectable(motions[i])) {			
					mMot = i;
					mMotion = (MOTION)i;
			}
		}
		ImGui::EndCombo();
	}


}

void BoxCollider::Inspect() {
	if (ImGui::CollapsingHeader("BoxCollider", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::DragFloat3("##Scale", (float*)&mScaleOffset);

		ImGui::SameLine();
		ImGui::Text("Scale");
		ImGui::Separator();


		ImGui::DragFloat3("##Translate", (float*)&mTranslateOffset);

		ImGui::SameLine();
		ImGui::Text("Translate");
		ImGui::Separator();
	}
}
void SphereCollider::Inspect() {



}
void PlaneCollider::Inspect() {



}