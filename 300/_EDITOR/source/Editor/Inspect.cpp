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

		General& Inspector = ent.GetComponent<General>();
		Inspector.Inspect();



		if (ent.HasComponent<Transform>()) {
			Transform& transform = ent.GetComponent<Transform>();
			transform.Inspect();
		}
		if (ent.HasComponent<Scripts>())
		{
			Scripts& scripts = ent.GetComponent<Scripts>();
			scripts.Inspect();
		}
		if (ent.HasComponent<Animator>()) {
			Animator& ani = ent.GetComponent<Animator>();
			ani.Inspect();
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
		if (ent.HasComponent<SphereCollider>()) {
			SphereCollider& sphereCollider = ent.GetComponent<SphereCollider>();
			sphereCollider.Inspect();
		}
		if (ent.HasComponent<PlaneCollider>()) {
			PlaneCollider& planeCollider = ent.GetComponent<PlaneCollider>();
			planeCollider.Inspect();
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

		if (ImGui::Selectable("SphereCollider")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<SphereCollider>())
				Entity(Hierarchy::selectedId).AddComponent<SphereCollider>();
		}

		if (ImGui::Selectable("PlaneCollider")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<PlaneCollider>())
				Entity(Hierarchy::selectedId).AddComponent<PlaneCollider>();
		}

		if (ImGui::Selectable("Animator")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<Animator>())
				Entity(Hierarchy::selectedId).AddComponent<Animator>();
		}
		ImGui::EndPopup();
	}

	mPopup = false; 
}

void General::Inspect() {


	ImGui::Text("Name");
	ImGui::SameLine();
	ImGui::InputText( "##", &name);
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

	if (ImGui::CollapsingHeader("Scripts", &delete_component, ImGuiTreeNodeFlags_DefaultOpen))
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
	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<Scripts>();
}

void Animator::Inspect()
{
	bool delete_component{ true };
	if (ImGui::CollapsingHeader("Animator", &delete_component, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(std::to_string(mAnimator.m_CurrentTime).c_str());	ImGui::SameLine();
		ImGui::Text(" / ");												ImGui::SameLine();
		ImGui::Text(std::to_string(mAnimator.m_CurrentAnimation->m_Duration).c_str());
		
		ImGui::Checkbox("Pause Animaton", &mAnimator.mIsPaused);
	}

	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<MeshRenderer>();
}

void MeshRenderer::Inspect() {
	bool delete_component{ true };
	if (ImGui::CollapsingHeader("MeshRenderer", &delete_component,ImGuiTreeNodeFlags_DefaultOpen)) {
		
		int st = mMeshPath.find_last_of("/");
		int ed = mMeshPath.find_last_of(".");
		std::string tempPath = mMeshPath.substr(st + 1, ed - (st + 1));

		ImGui::Text("Mesh");

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_GEOM")) {

				const char* data = (const char*)payload->Data;
				std::string data_str = std::string(data);
				mMeshPath = data_str;

				uid temp(mMeshPath);
				mMeshRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(temp.id));

				GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mMeshRef);

				Entity entins(Hierarchy::selectedId);
				if (entins.HasComponent<Animator>() && meshinst->mHasAnimation)
				{
					// if the new entity has both animations and the animator component, update the animator to use the new animation
					entins.GetComponent<Animator>().mAnimator.SetAnimation(&meshinst->mAnimation[0]);
				}
				else if (entins.HasComponent<Animator>() && !meshinst->mHasAnimation)
				{
					// if the new entity's mesh has no animation, but the entity still has the animator component
					entins.GetComponent<Animator>().mAnimator.m_CurrentAnimation = nullptr;
				}
			}


			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(tempPath.c_str()).x
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::Text(tempPath.c_str());

		

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		std::string textures[4] = { "DIFFUSE","NORMAL", "EMISSION","SPECULAR"};

		for (int i{ 0 }; i <4; i++) {
			if (mMaterialInstancePath[i] != "") {
				ImGui::Selectable(textures[i].c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_TEXT")) {

						const char* data = (const char*)payload->Data;
						std::string data_str = std::string(data);
						mMaterialInstancePath[i] = data_str;

						uid temp(mMaterialInstancePath[i]);
						mTextureRef[i] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(temp.id));
					}
					ImGui::EndDragDropTarget();
				}
				int posstart = mMaterialInstancePath[i].find_last_of("/");
				int posend = mMaterialInstancePath[i].find_last_of(".");

				std::string newpath = mMaterialInstancePath[i].substr(posstart+1, posend-(posstart+1));

				

				ImGui::SameLine();

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(newpath.c_str()).x
					- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);

				ImGui::Selectable(newpath.c_str());

				ImGui::Dummy(ImVec2(0.0f, 10.0f));
			}
			else {
				ImGui::Selectable(" ");

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_TEXT")) {

						const char* data = (const char*)payload->Data;
						std::string data_str = std::string(data);
						mMaterialInstancePath[i] = data_str;

						uid temp(mMaterialInstancePath[i]);
						mTextureRef[i] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(temp.id));
					}
					ImGui::EndDragDropTarget();
				}
					//if (ImGui::BeginDragDropTarget())
					//{
					//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_TEXT")) {

					//		const char* data = (const char*)payload->Data;
					//		std::string data_str = std::string(data);
					//		mMaterialInstancePath[i] = data_str;
					//		uid temp(mMaterialInstancePath[i]);
					//		mTextureRef[i] = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(temp.id));

					//	}
					//	ImGui::EndDragDropTarget();
					//}
				
			}
		}



	}
	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<MeshRenderer>();
}


void RigidBody::Inspect() {
	bool delete_component{ true };

	if (ImGui::CollapsingHeader("RigidBody", &delete_component, ImGuiTreeNodeFlags_DefaultOpen))
	{


		ImGui::DragFloat("##Density", (float*)&mDensity);

		ImGui::SameLine();
		ImGui::Text("Density");
		ImGui::Separator();


		const char* materials[] = { "RUBBER", "WOOD", "METAL", "ICE","CONCRETE","GLASS" };
		const char* motions[] = { "STATIC", "DYNAMIC" };


		if (ImGui::BeginCombo("Material", (materials[mMat]))) {

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

	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<RigidBody>();
}

void BoxCollider::Inspect() {
	bool delete_component{ true };
	if (ImGui::CollapsingHeader("BoxCollider", &delete_component, ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::DragFloat3("Boxcollider Scale", (float*)&mScaleOffset, 0.1f);
		ImGui::SameLine();
		ImGui::Text("Scale");
		ImGui::Separator();

		ImGui::DragFloat3("Boxcollider Translate", (float*)&mTranslateOffset);
		ImGui::SameLine();
		ImGui::Text("Translate");
		ImGui::Separator();
	}
	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<BoxCollider>();
}


void SphereCollider::Inspect() {
	bool delete_component{ true };
	if (ImGui::CollapsingHeader("SphereCollider", &delete_component, ImGuiTreeNodeFlags_DefaultOpen)) {
		
		ImGui::DragFloat("##Scale", (float*)&mScaleOffset);
		ImGui::SameLine();
		ImGui::Text("Scale");
		ImGui::Separator();

		ImGui::DragFloat3("##Translate", (float*)&mTranslateOffset);
		ImGui::SameLine();
		ImGui::Text("Translate");
		ImGui::Separator();
	}
	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<SphereCollider>();
}


void PlaneCollider::Inspect() {
	bool delete_component{ true };
	if (ImGui::CollapsingHeader("PlaneCollider",&delete_component, ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat3("##Scale", (float*)&mNormal);
		ImGui::SameLine();
		ImGui::Text("Scale");
		ImGui::Separator();

		ImGui::DragFloat("##Translate", (float*)&mTranslateOffset);
		ImGui::SameLine();
		ImGui::Text("Translate");
		ImGui::Separator();

	}
	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<PlaneCollider>();
}