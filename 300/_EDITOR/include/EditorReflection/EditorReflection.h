#pragma once
#include "Reflection/Reflection.h"
#include "imgui.h"
#include "imgui_stdlib.h"

template <typename T>
void ReflectProperties(T& _type);
void SetCursor();


template <typename T>
void ReflectProperties(T& instance)
{
	meta::class_type class_type = meta::resolve_type(instance);
	for (auto member : class_type.get_members())
	{
		std::string name = member.get_name();
		ImGui::Text(name.c_str());
		ImGui::SameLine();
		SetCursor();
		meta::uvalue val = member.get(instance);

		if (member.get_type().get_value_type() == meta::resolve_type<bool>())
		{
			bool type = val.as<bool>();
			ImGui::Checkbox(("##" + name).c_str(), &type);
			member.set(instance, type);
		}
		else if (member.get_type().get_value_type() == meta::resolve_type<float>())
		{
			float type = val.as<float>();
			ImGui::DragFloat(("##" + name).c_str(), &type);
			member.set(instance, type);
		}
		else if (member.get_type().get_value_type() == meta::resolve_type<glm::vec3>())
		{
			glm::vec3 type = val.as<glm::vec3>();
			ImGui::DragFloat3(("##" + name).c_str(), (float*)&type, 0.1f);
			member.set(instance, type);
		}
		ImGui::Separator();
	}
}