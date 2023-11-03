/*!*************************************************************************
****
\file Script.cpp
\author(s) Lor Xaun Yun Michelle
\par DP email:
xaunyunmichelle.lor\@digipen.edu
\date 3-11-2023
\brief
This file contains a function InspectScript that will be called under
ImGui's inspect.
- InspectScript
Aid in rendering the lua reflection variables in the ImGui.
****************************************************************************
***/
#include "Script.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "sol/sol.hpp"

namespace ImGuiHelper {
	void Render(const char* name, bool* value) {
		ImGui::Checkbox(name, value);
	}
	void Render(const char* name, float* value) {
		ImGui::DragFloat(name, value);
	}
	void Render(const char* name, std::string* value) {
		ImGui::InputText(name, value);
	}
	void Render(const char* name, glm::vec3* value) {
		ImGui::DragFloat3(name, &value->x);
	}
}

void InspectScript(Script* script)
{
	if (ImGui::TreeNode("Variables")) {

		for (auto& e : script->variables) {
			std::visit([&](auto&& v) {
				//v = script.env[e.first].get<std::remove_reference<decltype(v)>::type>();
				v = script->env[e.first].get<decltype(v)>();
				ImGuiHelper::Render(e.first.c_str(), &v);
				script->env[e.first] = v;
				}, e.second);
		}

		ImGui::TreePop();
	}
}