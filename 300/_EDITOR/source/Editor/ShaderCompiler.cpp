#include "ShaderCompiler.h"
#include "ResourceManagerTy.h"
#include "imgui_stdlib.h"

bool ShaderCompiler::openWindow;

void ShaderCompiler::init() {
	openWindow = true;
}

void ShaderCompiler::update() {

	auto getShaderExtension = [](std::string shaderpath) -> std::string
	{
		// returns vert.glsl or frag.glsl
		return shaderpath.substr(shaderpath.find_last_of("_"));
	};


	ImGui::InputText("Name", &shaderName);

	if(ImGui::BeginCombo("##Vert", vertStr.c_str())) {
		for (const auto& entry : std::filesystem::directory_iterator(systemManager->mResourceTySystem->shader_path))
		{
			if (getShaderExtension(entry.path().string()) == "_vert.glsl")
				if (ImGui::Selectable(entry.path().filename().string().c_str())) {
					vertStr = entry.path().filename().string();
				}
		}
		ImGui::EndCombo();
	}
	
	if(ImGui::BeginCombo("##Frag", fragStr.c_str())) {
		for (const auto& entry : std::filesystem::directory_iterator(systemManager->mResourceTySystem->shader_path))
		{
			if (getShaderExtension(entry.path().string()) == "_frag.glsl")
				if (ImGui::Selectable(entry.path().filename().string().c_str())) {
					fragStr = entry.path().filename().string();
				}
		}
		ImGui::EndCombo();
	}

	if(ImGui::Button("Compile")) {

		systemManager->mResourceTySystem->create_Shader(shaderName, systemManager->mResourceTySystem->shader_path + vertStr,
			systemManager->mResourceTySystem->shader_path + fragStr);

		shaderName = "";
		fragStr = "";
		vertStr = "";	
	}

		//if (std::filesystem::is_regular_file(entry))
		//{
			//if(getShaderExtension(entry.path().string()) == "_vert.glsl")
			//	vertShaders.push_back(entry.path().string());

		//	else if (getShaderExtension(entry.path().string()) == "_frag.glsl")
		//		fragShaders.push_back(entry.path().string());
		//}


}