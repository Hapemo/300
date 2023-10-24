#include "ECS/ECS_Components.h"

/*!*************************************************************************
****
\file Inspect.cpp
\author Han ChengWen, Charissa Yip , Cheong Ming Lun
\par DP email: c.han@\digipen.edu, charissa.yip\@digipen.edu, m.cheong\@digipen.edu
\date 28-09-2023
\brief
This header file contains the inspector window and relative functions to run
the system logic.

The functions

- init
Initialize related data needed for Inspector window to draw

- update
Run Inspector logic that checks for component and render related data which
provides user to edit in run time


- Add_component
Add component button and logic implementation that allows user to add
components

- Transform::Inspect
Inspect display for Transform component
Included ImGui custom dragfloat3 function for data required in command system.

-PlaneCollider::Inspect
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

- Audio::Inspect
Inspect display for Audio components
****************************************************************************
***/

#include "Inspect.h"
#include "Editor.h"
#include "ECS/ECS_Components.h"
#include "ECS/ECS_Systems.h"
#include "Hierarchy.h"
#include <math.h>
#include "imgui_stdlib.h"
#include "ScriptingSystem.h"

#include "ResourceManagerTy.h"
#include "Debug/Logger.h"
#include "Audio/AudioSystem.h"
#include "Input/InputMapSystem.h"
#include <TextureCompressor.h>
#include "Script/Script.h"

#include <descriptor.h>
#include <string>

/***************************************************************************/
/*!
\brief
	Init function for Inspector Window
*/
/**************************************************************************/
void Inspect::init() {
//empty
}
/***************************************************************************/
/*!
\brief
	Update function for Inspector Window
*/
/**************************************************************************/
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
		if (ent.HasComponent<PointLight>()) {
			PointLight& pointLight = ent.GetComponent<PointLight>();
			pointLight.Inspect();
		}

		if (ent.HasComponent<Camera>()) {
			Camera& cam = ent.GetComponent<Camera>();
			cam.Inspect();
		}

		if (ent.HasComponent<Audio>()) {
			Audio& audio = ent.GetComponent<Audio>();
			audio.Inspect();
		}

		if (ent.HasComponent<AudioListener>()) {
			AudioListener& audio_listener = ent.GetComponent<AudioListener>();
			audio_listener.Inspect();
		}

		if (ent.HasComponent<UIrenderer>()) {
			UIrenderer& render = ent.GetComponent<UIrenderer>();
			render.Inspect();
		}

		if (ent.HasComponent<VFX>()) {
			VFX& vfx = ent.GetComponent<VFX>();
			vfx.Inspect();
		}
		//if (ent.HasComponent<InputActionMapEditor>()) {
		//	InputActionMapEditor& inputAction = ent.GetComponent<InputActionMapEditor>();
		//	inputAction.Inspect();
		//}
		//--------------------------------------------// must be at the LAST OF THIS LOOP
		Add_component(); 
	}

}
/***************************************************************************/
/*!
\brief
	Add Component function for Inspector Window
*/
/**************************************************************************/
void Inspect::Add_component() {


	auto initialCursorPos = ImGui::GetCursorPos();
	auto windowSize = ImGui::GetWindowSize();
	float centralizedCursorpos = (windowSize.x - 200) * 0.5f;
	ImGui::SetCursorPosX(centralizedCursorpos);


	ImGui::SetCursorPosX((ImGui::GetWindowSize().x )/2 - ImGui::CalcTextSize("      ComponentList ").x);

	if (ImGui::BeginCombo("##add","ComponentList"))
	{


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

		if (ImGui::Selectable("Animator")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<Animator>())
				Entity(Hierarchy::selectedId).AddComponent<Animator>();
		}

		if (ImGui::Selectable("PointLight"))
		{
			if (!Entity(Hierarchy::selectedId).HasComponent<PointLight>())
				Entity(Hierarchy::selectedId).AddComponent<PointLight>();
		}
		if (ImGui::Selectable("Audio")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<Audio>())
				Entity(Hierarchy::selectedId).AddComponent<Audio>();
		}	

		if (ImGui::Selectable("AudioListener")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<AudioListener>())
				Entity(Hierarchy::selectedId).AddComponent<AudioListener>();				
		}

		if (ImGui::Selectable("VFX")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<VFX>())
				Entity(Hierarchy::selectedId).AddComponent<VFX>();
		}
		if (ImGui::Selectable("MeshRenderer")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<MeshRenderer>()) {
				Entity(Hierarchy::selectedId).AddComponent<MeshRenderer>();

				Entity meshobj(Hierarchy::selectedId);

			}
		}
		if (ImGui::Selectable("UIrenderer")) {
			if (!Entity(Hierarchy::selectedId).HasComponent<UIrenderer>())
				Entity(Hierarchy::selectedId).AddComponent<UIrenderer>();
		}

		ImGui::EndCombo();


	}

}
/***************************************************************************/
/*!
\brief
	Inspector functionality for General
*/
/**************************************************************************/
void General::Inspect() {


	ImGui::Text("Name");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	ImGui::InputText("##naming",&name);


	//ImGui::Text("Tag");

	//ImGui::SameLine();
	//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
	//	- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);

	//if (ImGui::BeginCombo("##Tag", tag[tagid].c_str())) {

	//	for (int i = 0; i < 5; i++) {
	//		if (ImGui::Selectable(tag[i].c_str())) {
	//			tagid = i;
	//		}
	//	}
	//	ImGui::EndCombo();
	//}


	ImGui::Text("Tag");

	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);

	if (ImGui::BeginCombo("##Tag", enum_tag::ToString(tagid))) {

		for (int i = 0; i < enum_tag::COUNT; i++) {
			enum_tag::enum_tag temp = static_cast<enum_tag::enum_tag>(i);
			if (ImGui::Selectable(enum_tag::ToString(temp))) {
				tagid = static_cast<enum_tag::enum_tag>(i);
			}
		}
		ImGui::EndCombo();
	}
}

/***************************************************************************/
/*!
\brief
	Inspector functionality for Transform
*/
/**************************************************************************/
void Transform::Inspect() {
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {


		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat3("##Position", (float*)&mTranslate);

		ImGui::Separator();


		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat3("##Scale", (float*)&mScale, 1);

		ImGui::Separator();


		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat3("##Rotation", (float*)&mRotate, 1);


		
	}

}
/***************************************************************************/
/*!
\brief
	Inspector functionality for Camera
*/
/**************************************************************************/
void Camera::Inspect()
{

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);

		ImGui::Separator();

		vec3 temp = mCamera.mPosition;
		ImGui::DragFloat3("Camera Position", (float*)&temp);

		mCamera.mTarget += temp - mCamera.mPosition;
		mCamera.mPosition = temp;

		ImGui::DragFloat3("Camera Target", (float*)&mCamera.mTarget);

		ImGui::Text("Aspect Ratio");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat("##Aspect", &mCamera.mAspectRatio);

		ImGui::Text("FOV");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat("##FOV", &mCamera.mFovDegree, 0.2f, GFX::CameraConstants::minFOV, GFX::CameraConstants::maxFOV);

		ImGui::Text("Far Plane");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat("##FP", &mCamera.mFar);

		ImGui::Text("Near plane");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat("##NP", &mCamera.mNear);

		//ImGui::Text("Orthographic");
		//ImGui::SameLine();
		//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
		//	- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		//ImGui::Checkbox("##Orth", &Orthographic);
	}

}

void PointLight::Inspect()
{
	if (ImGui::CollapsingHeader("PointLight", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat3("##Light Color", (float*)&mLightColor, 0.01f);


		ImGui::Text("Linear Falloff");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat("##Linear Falloff", (float*)&mLinearFalloff, 0.1f);

		ImGui::Text("Quadratic Falloff");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat("##Quadratic Falloff", (float*)&mQuadraticFalloff, 0.1f);

		ImGui::Text("Intensity");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcItemWidth()
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::DragFloat("##Intensity", (float*)&mIntensity, 0.1f);


	}
}
/***************************************************************************/
/*!
\brief
	Inspector functionality for Script
*/
/**************************************************************************/
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
					script.Load(Hierarchy::selectedId);
					scripts.scriptsContainer.push_back(script);
					//std::cout << "Script " << script.scriptFile << " added to entity " << std::to_string((int)Hierarchy::selectedId) << std::endl;
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
						
						script.Load(Hierarchy::selectedId);

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
			ImGui::SetNextItemOpen(true);
			if (ImGui::TreeNode(elem.scriptFile.c_str())) {
				if (ImGui::IsItemClicked(1)) {
					open_popup = true;
					deleteScript = elem.scriptFile;
				}
				InspectScript(elem);
				ImGui::TreePop();
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
/***************************************************************************/
/*!
\brief
	Inspector functionality for Animator
*/
/**************************************************************************/
void Animator::Inspect()
{
	bool delete_component{ true };
	if (ImGui::CollapsingHeader("Animator", &delete_component, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (mAnimator.m_CurrentAnimation == nullptr)
		{
			ImGui::Text("Mesh is not capable of animation");
		}

		else
		{
			ImGui::Text(std::to_string(mAnimator.m_CurrentTime).c_str());	ImGui::SameLine();
			ImGui::Text(" / ");												ImGui::SameLine();
			ImGui::Text(std::to_string(mAnimator.m_CurrentAnimation->m_Duration).c_str());
		
			ImGui::Checkbox("Pause Animaton", &mAnimator.mIsPaused);
		}
	}

	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<MeshRenderer>();
}


/***************************************************************************/
/*!
\brief
	Inspector functionality for MeshRenderer
*/
/***************************************************************************/
void MeshRenderer::Inspect() 
{
	//!< Shader Helper 
	auto getShaderName = [](std::string shaderpath) -> std::string
	{
		// returns pointLight_vert.glsl
		return shaderpath.substr(shaderpath.find_last_of("/") + 1);
	};

	//!< Shader Helper 
	auto getShaderExtension = [](std::string shaderpath) -> std::string
	{
		// returns vert.glsl or frag.glsl
		return shaderpath.substr(shaderpath.find_last_of("_"));
	};

	//!< Helper
	auto getFilename = [](std::string filepath) -> std::string
	{
		// returns AT-AT
		std::string ret_str = filepath.substr(filepath.find_last_of("/") + 1);
		ret_str = ret_str.substr(0, ret_str.find_first_of("."));
		return ret_str;
	};


	bool delete_component{ true };
	if (ImGui::CollapsingHeader("MeshRenderer", &delete_component,ImGuiTreeNodeFlags_DefaultOpen)) 
	{
		
		int st = static_cast<int>(mMeshPath.find_last_of("/"));
		int ed = static_cast<int>(mMeshPath.find_last_of("."));
		std::string tempPath = mMeshPath.substr(st + 1, ed - (st + 1));

		Entity entins(Hierarchy::selectedId);

		// == >> Shaders << == //
		std::string shaderstr{" "};

		if (systemManager->mResourceTySystem->m_Shaders.find(mShaderRef.data_uid) != systemManager->mResourceTySystem->m_Shaders.end())
			shaderstr = systemManager->mResourceTySystem->m_Shaders[mShaderRef.data_uid].first;
		
		if (ImGui::BeginCombo("##Shaders", shaderstr.c_str())) {

			for (auto& data : systemManager->mResourceTySystem->m_Shaders) {

				if (ImGui::Selectable(data.second.first.c_str())) {
					mShaderRef.data_uid = data.first;
				}
			}

			ImGui::EndCombo();

		}
		ImGui::Separator();

		// == >> Mesh << == //
		ImGui::Text("Mesh");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_GEOM")) 
			{

				const char* data = (const char*)payload->Data;
				std::string data_str = std::string(data);
				mMeshPath = data_str;

				std::string descfilepath = data_str + ".desc";
				unsigned guid = _GEOM::GetGUID(descfilepath);
				mMeshRef.data = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(guid));
				GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mMeshRef.data);


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


		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_FBX")) 
			{
				const char* data = (const char*)payload->Data;
				std::string data_str = std::string(data);
				mMeshPath = systemManager->mResourceTySystem->compiled_geom_path + getFilename(data_str) + ".geom";
				std::string GEOM_Descriptor_Filepath;
				unsigned guid;

				bool descFilePresent = _GEOM::CheckAndCreateDescriptorFileMESH(data_str, GEOM_Descriptor_Filepath);
				std::string descfilepath = data_str + ".desc";
				guid = _GEOM::GetGUID(descfilepath);

				// If the descriptor file is not present, then load it
				if (!descFilePresent)
				{
					//!>> Calling the GEOM Compiler to load 
					std::string command = "..\\_GEOM_COMPILER\\_GEOM_COMPILER.exe ";
					command += GEOM_Descriptor_Filepath;
					int result = system(command.c_str());

					std::string geompath = GEOM_Descriptor_Filepath.substr(0, GEOM_Descriptor_Filepath.find_last_of("."));

					systemManager->mResourceTySystem->mesh_Load(geompath, guid);
				}


				mMeshRef.data = reinterpret_cast<void*>(systemManager->mResourceTySystem->get_mesh(guid));
				GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mMeshRef.data);

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


		// == >> Textures << == //
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		std::string textures[4] = { "DIFFUSE","NORMAL", "EMISSION","SPECULAR"};

		for (int i{ 0 }; i <4; i++) 
		{
			if (mMaterialInstancePath[i] != "") 
			{
				ImGui::Text(textures[i].c_str());
				if (ImGui::BeginDragDropTarget())
				{
					// loading compressed texture
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_TEXT")) 
					{
						const char* data = (const char*)payload->Data;
						std::string data_str = std::string(data);

						mMaterialInstancePath[i] = data_str;

						uid temp(mMaterialInstancePath[i]);
						mTextureRef[i].data = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(temp.id));
					}

					// file uncompressed texture for objects
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_TEXT_UNCOMPRESS")) 
					{
						const char* data = (const char*)payload->Data;
						std::string data_str = std::string(data);

						std::string texturestr =systemManager->mResourceTySystem->compressed_texture_path + getFilename(data_str) + ".ctexture";
						mMaterialInstancePath[i] = texturestr;

						std::string TEXTURE_Descriptor_Filepath;
						unsigned guid;

						// check and ensures that the descriptor file for the materials are created
						bool descFilePresent = _GEOM::CheckAndCreateDescriptorFileTEXTURE(data_str, TEXTURE_Descriptor_Filepath, texturestr);
						std::string descfilepath = data_str + ".desc";
						guid = _GEOM::GetGUID(descfilepath);	// gets the guid from the png desc

						// If the descriptor file is not present, then load it
						if (!descFilePresent)
						{
							CompressImageFile(data_str.c_str(), systemManager->mResourceTySystem->compressed_texture_path.c_str());

							// Load the textures into the list of usable textures within the engine
							systemManager->mResourceTySystem->texture_Load(getFilename(data_str), guid);
							mTextureRef[i].data = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(guid));
							mTextureCont[i] = true;
						}

						uid temp(mMaterialInstancePath[i]);
						mTextureRef[i].data = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(temp.id));
						mTextureCont[i] = true;
					}

					ImGui::EndDragDropTarget();
				}

				int posstart = static_cast<int>(mMaterialInstancePath[i].find_last_of("/"));
				int posend = static_cast<int>(mMaterialInstancePath[i].find_last_of("."));

				std::string newpath = mMaterialInstancePath[i].substr(posstart+1, posend-(posstart+1));

				ImGui::SameLine();

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(newpath.c_str()).x
					- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);

				ImGui::Selectable(newpath.c_str());

				// The Descriptor data for the selected texture
				if (ImGui::TreeNode("DescriptorFile"))
				{
					ImGui::Text(getFilename(mMaterialInstancePath[i]).c_str());
					ImGui::TreePop();
				}

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
						mTextureRef[i].data = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(temp.id));

					}
					ImGui::EndDragDropTarget();
				}
			}
		}

		ImGui::ColorPicker4("MeshColor", (float*)&mInstanceColor);
	}

	// == >> Mesh Renderer GEOM Descriptor File << == //
	//ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("GEOM DescirptorFile"))
	{
		GFX::Mesh* meshinst = reinterpret_cast<GFX::Mesh*>(mMeshRef.data);

		// sanity check
		if (meshinst != nullptr)
		{
			ImGui::InputText("Desc Filepath", const_cast<char*>(meshinst->mMeshDescriptorPath.c_str()), meshinst->mMeshDescriptorPath.length() + 1);
			ImGui::InputInt("GUID", reinterpret_cast<int*>(&meshinst->mMeshDescriptorData.m_GUID));

			// Vert shader selection
			int selectedFBX{};
			_GEOM::DescriptorData& descInst = meshinst->mMeshDescriptorData;

			if (ImGui::BeginCombo("FBX Filepaths", descInst.m_Filepaths[selectedFBX].data(), 0))
			{
				for (int i{}; i < descInst.m_Filepaths.size(); ++i)
				{
					bool isItemSelected = (selectedFBX == i);
					if (ImGui::Selectable(descInst.m_Filepaths[i].data(), isItemSelected))
						selectedFBX = i;

					if (isItemSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::DragFloat3("Pre-Transforms", (float*)&descInst.m_Translate, 0.1f);
			ImGui::DragFloat3("Pre-Scale", (float*)&descInst.m_Scale, 0.1f);
			ImGui::DragFloat3("Pre-Rotate", (float*)&descInst.m_Rotate, 0.1f);

			if (ImGui::Button("Save Descriptor File"))
			{
				std::cout << "Saving Descriptor File to: " << meshinst->mMeshDescriptorPath << std::endl;
				_GEOM::DescriptorData::SerializeGEOM_DescriptorDataToFile(meshinst->mMeshDescriptorPath, descInst);
			}
		}
		ImGui::TreePop();
	}

	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<MeshRenderer>();
}

/***************************************************************************/
/*!
\brief
	Inspector functionality for Rigidbody
*/
/***************************************************************************/
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
/***************************************************************************/
/*!
\brief
	Inspector functionality for BoxCollider
*/
/***************************************************************************/
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

/***************************************************************************/
/*!
\brief
	Inspector functionality for SphereCollider
*/
/***************************************************************************/
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

/***************************************************************************/
/*!
\brief
	Inspector functionality for Audio
*/
/***************************************************************************/
void Audio::Inspect() {
	bool delete_component = true;
	auto audioEntities = systemManager->ecs->GetEntitiesWith<Audio>();
	std::string full_file_path;  // With the Audio File Name  e.g "../assets\\Audio\\farm_ambience.wav"
	std::string file_path;		 // Only Audio Directory	  e.g "../assets\\Audio"
	std::string audio_name;      // Audio Name only.		  e.g "farm_ambience.wav"

	float vol_changed = false; 

	const char* audio_type[] = { "SFX" , "BGM" };

	// Audio Component (Bar)
	if (ImGui::CollapsingHeader("Audio", &delete_component, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_AUDIO"))
			{
				full_file_path = std::string((const char*)payload->Data);

				size_t audio_dir_pos = full_file_path.find_first_of("Audio");

				if (audio_dir_pos != std::string::npos) {
					// Use substr to extract the substring up to the found position
					file_path = full_file_path.substr(0, audio_dir_pos + 5); // Account for "Audio"
					//std::cout << result << std::endl;
				}

				size_t audio_name_start = full_file_path.find_last_of("\\");
				std::string audio_name;

				if (audio_name_start != std::string::npos) {
					audio_name = full_file_path.substr(audio_name_start + 1);
				}

				// Must be outside (what if i remove and add an already loaded audio)
				mFilePath = file_path;
				mFileName = audio_name;
				mFullPath = file_path + "/" + audio_name;

				// Check if the [Audio file] has been uploaded into the database...
				if (systemManager->mAudioSystem.get()->CheckAudioExist(audio_name)) // Exists ... 
				{
					// For Debugging Purposes
					PINFO("[Loaded] Audio is already in database.");
					// Assign the [Sound*] to this component. 
					mSound = systemManager->mAudioSystem.get()->FindSound(audio_name);
					Entity(Hierarchy::selectedId).GetComponent<Audio>().mIsEmpty = false; // Component is populated with info
					Audio& audioent = Entity(Hierarchy::selectedId).GetComponent<Audio>();
					return;
				}

				else // Does not exist...
				{
					// Load the Audio File + Check (load status)
					systemManager->mAudioSystem.get()->UpdateLoadAudio(Entity(Hierarchy::selectedId));
					Entity(Hierarchy::selectedId).GetComponent<Audio>().mIsEmpty = false; // must be here (editor specific) -> to trigger the other options to appear.
					/*Audio& audioent = Entity(Hierarchy::selectedId).GetComponent<Audio>();
					int i = 0;*/
				}

			}

			ImGui::EndDragDropTarget();
		}

	}

	ImGui::Text("Drag drop 'Audio' files to header above 'Audio'");
	ImGui::Text("Audio File Selected: ");
	ImGui::Text(Entity(Hierarchy::selectedId).GetComponent<Audio>().mFullPath.c_str());
	Audio& audio = Entity(Hierarchy::selectedId).GetComponent<Audio>();

	static bool remove_audio_bool = false;
	if (!Entity(Hierarchy::selectedId).GetComponent<Audio>().mIsEmpty)
	{
		ImGui::Checkbox("Remove Audio File.", &remove_audio_bool);
	}

	if (!mIsEmpty && remove_audio_bool) // if not empty
	{
		Entity(Hierarchy::selectedId).GetComponent<Audio>().ClearAudioComponent();
		remove_audio_bool = false;
		PINFO("Successfully Removed Audio.");
	}

	static float f1 = 0.123f;
	if (!mIsEmpty)
	{
		//ImGui::Checkbox("Play This (start the scene first)", &mIsPlay);
		//ImGui::Checkbox("IsPlaying", &mIsPlaying);
		ImGui::Checkbox("3D Audio", &m3DAudio);
		ImGui::Checkbox("Play on Awake", &mPlayonAwake);
		ImGui::Checkbox("Is Looping", &mIsLooping);
		ImGui::SliderFloat("Volume", &mVolume, 0.0f, 1.0f, "volume = %.3f");

		if (ImGui::IsItemEdited())
		{
			mChannel->setVolume(mVolume);
		}

		ImGui::SliderFloat("Fade Speed", &mFadeSpeedModifier, 0.0f, 1.0f, "fade = %.3f");

		if (m3DAudio)
		{
			ImGui::SliderFloat("Min Distance", &mMinDistance, 0.0f, 3000.0f, "%.3f");
			ImGui::SliderFloat("Max Distance", &mMaxDistance, 0.0f, 3000.0f, "%.3f");

			//systemManager->mAudioSystem.get()->Update3DChannelSettings(Entity(Hierarchy::selectedId)); 
		}

	}

	// AudioType Selector 
	if (ImGui::BeginCombo("Audio Type", audio_type[mAudio]))
	{
		for (unsigned char i{ 0 }; i < 2; i++) {
			if (ImGui::Selectable(audio_type[i])) {
				mAudio = i;
				switch (mAudio)
				{
				case 0:
					mAudioType = AUDIO_SFX;
					mTypeChanged = true;
					systemManager->mAudioSystem.get()->UpdateChannelReference(Entity(Hierarchy::selectedId));
					break;
				case 1:
					mAudioType = AUDIO_BGM;
					mTypeChanged = true;
					systemManager->mAudioSystem.get()->UpdateChannelReference(Entity(Hierarchy::selectedId));
					break;
				}
			}
		}

		ImGui::EndCombo();
	}

	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<Audio>();
}


/***************************************************************************/
/*!
\brief
	Inspector functionality for AudioListener
*/
/***************************************************************************/
void AudioListener::Inspect() {
	bool delete_component = true;

	// Audio Component (Bar)
	if (ImGui::CollapsingHeader("Audio Listener", &delete_component, ImGuiTreeNodeFlags_DefaultOpen))
	{

	}

	
	if (delete_component == false)
		Entity(Hierarchy::selectedId).RemoveComponent<AudioListener>();
}
/***************************************************************************/
/*!
\brief
	Inspector functionality for Input action
*/
/***************************************************************************/
//void InputActionMapEditor::Inspect()
//{
//	bool delete_component = true;
//
//	const char* action_maps[] = { "PlayerMovement", "MenuControls" };
//	static std::string newActionMapName;
//
//	//std::string selected_map {};
//
//	if (ImGui::CollapsingHeader("InputActionMapEditor", &delete_component, ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		auto ActionMapEntities = systemManager->ecs->GetEntitiesWith<InputActionMapEditor>();
//		//int size = ActionMapEntities.size();
//		InputActionMapEditor& editor_component = ActionMapEntities.get<InputActionMapEditor>(Hierarchy::selectedId);
//
//		// Create New [InputActionMap]
//		ImGui::Text("Create new InputActionMap");
//		ImGui::InputText(".", &newActionMapName);
//		if (ImGui::Button("Add Action Map"))
//		{
//			// Creates a new [ActionMap] - component side.
//			Entity(Hierarchy::selectedId).GetComponent<InputActionMapEditor>().AddActionMap(newActionMapName);
//		}
//
//		// [InputActionMap] selected
//		ImGui::Text("Select Action Map (to edit): ");
//		if (ImGui::BeginCombo("Selected Action Map", mSelectedMapName.c_str()))
//		{
//
//			for (auto& action_pair : editor_component.mActionMap)
//			{
//				if (ImGui::Selectable(action_pair.first.c_str()))
//				{
//					mSelectedMapName = action_pair.first.c_str();
//
//					selected = true;
//				}
//			}
//			ImGui::EndCombo();
//		}
//
//
//		PseudoInputAction& selected_action = GetAction(mSelectedMapName);
//
//
//		auto& e_key_map = systemManager->mInputActionSystem->e_key_mapping;
//
//		if (selected)
//		{
//			if (mSelectedMapName != " ")
//			{
//				if (ImGui::BeginCombo("Movement (UP)", selected_action.mSelectedBindingUP.c_str()))
//				{
//					// Iterate through the [Key Map]
//					for (auto& e_keypair : e_key_map)
//					{
//						std::string key_name = e_keypair.first;
//						if (ImGui::Selectable(key_name.c_str()))
//						{
//							selected_action.mKeyBindUp = (int)(e_key_map[key_name]);
//							selected_action.LinkKeyBinding(KEY_UP, (E_KEY)selected_action.mKeyBindUp);
//							selected_action.mSelectedBindingUP = e_keypair.first;
//						}
//					}
//					ImGui::EndCombo();
//				}
//
//				if (ImGui::BeginCombo("Movement (DOWN)", selected_action.mSelectedBindingDOWN.c_str()))
//				{
//					// Iterate through the [Key Map]
//					for (auto& e_keypair : e_key_map)
//					{
//						std::string key_name = e_keypair.first;
//						if (ImGui::Selectable(key_name.c_str()))
//						{
//							selected_action.mKeyBindDown = (int)(e_key_map[key_name]);
//							selected_action.LinkKeyBinding(KEY_DOWN, (E_KEY)selected_action.mKeyBindDown);
//							selected_action.mSelectedBindingDOWN = e_keypair.first;
//						}
//					}
//
//					ImGui::EndCombo();
//				}
//
//				if (ImGui::BeginCombo("Movement (LEFT)", selected_action.mSelectedBindingLEFT.c_str()))
//				{
//					// Iterate through the [Key Map]
//					for (auto& e_keypair : e_key_map)
//					{
//						std::string key_name = e_keypair.first;
//						if (ImGui::Selectable(key_name.c_str()))
//						{
//							selected_action.mKeyBindLeft = (int)(e_key_map[key_name]);
//							selected_action.LinkKeyBinding(KEY_LEFT, (E_KEY)selected_action.mKeyBindLeft);
//							selected_action.mSelectedBindingLEFT = e_keypair.first;
//						}
//					}
//
//					ImGui::EndCombo();
//				}
//
//				if (ImGui::BeginCombo("Movement (RIGHT)", selected_action.mSelectedBindingRIGHT.c_str()))
//				{
//					// Iterate through the [Key Map]
//					for (auto& e_keypair : e_key_map)
//					{
//						std::string key_name = e_keypair.first;
//						if (ImGui::Selectable(key_name.c_str()))
//						{
//							selected_action.mKeyBindRight = (int)(e_key_map[key_name]);
//							selected_action.LinkKeyBinding(KEY_RIGHT, (E_KEY)selected_action.mKeyBindRight);
//							selected_action.mSelectedBindingRIGHT = e_keypair.first;
//						}
//					}
//
//					ImGui::EndCombo();
//				}
//			}
//		}
//	}
//}


void UIrenderer::Inspect() {
	bool delete_component = true;

	if (ImGui::CollapsingHeader("UIrenderer", &delete_component, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Selectable(" ");

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_TEXT")) {

				const char* data = (const char*)payload->Data;
				std::string data_str = std::string(data);
				mTexPath = data_str;

				uid temp(mTexPath);
				mTextureRef = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(temp.id));
			}
			ImGui::EndDragDropTarget();
		}
	}
}

void VFX::Inspect() {
	bool delete_component = true;
	if (ImGui::CollapsingHeader("VFX", &delete_component, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored({ 0.f,1.f, 1.f, 1.f }, "Bloom Variables");
		ImGui::DragFloat3("Entity Bloom Threshold", (float*)&mBloomThreshold, 0.01f, 0.f, 1.f);
	}
}
