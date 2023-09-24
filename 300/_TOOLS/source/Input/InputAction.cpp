#include "Input/InputAction.h"

InputAction::InputAction() : mActionName("New Action")
{}

InputAction::InputAction(std::string action_name) : mActionName(action_name)
{}

InputAction::InputAction(std::string action_name, E_STATE key_state, E_KEY key_binding) : mActionName(action_name)
{
	InputBinding new_binding = InputBinding(key_state, key_binding); 
	mKeyBindings.insert(std::make_pair(action_name, new_binding));
}

void InputAction::Enable()
{
	isEnable = true;
}

void InputAction::Disable()
{
	isEnable = false;
}

bool InputAction::isEnabled()
{
	return isEnable;
}

void InputAction::AddKeyBinding(std::string binding_name, E_STATE key_state, E_KEY key_binding)
{
	InputBinding new_binding = InputBinding(key_state, key_binding);
	mKeyBindings.insert(std::make_pair(binding_name, new_binding));
}

std::unordered_map<std::string, InputBinding> InputAction::GetKeyBindings() const
{
	return mKeyBindings;
}

InputBinding& InputAction::GetKeyBinding(std::string binding_name)
{
	auto binding_it = mKeyBindings.find(binding_name);

	if (binding_it != mKeyBindings.end())
	{
		return mKeyBindings.find(binding_name)->second;
	}

	InputBinding empty;

	return empty;
}

glm::vec2 InputAction::ReadValue() const
{
	glm::vec2 vec;

	for (const auto& input_binding : mKeyBindings)
	{
		MAP_TO_AXIS axis_mapping = input_binding.second.direction_map; 
	

		switch (axis_mapping)
		{
			case X_POSITIVE:
				vec = { 1.0f, 0.0f };
				return vec;
			case X_NEGATIVE:
				vec = { -1.0f, 0.0f };
				return vec;
			case Y_POSITIVE:
				vec = { 0.0f , 1.0f };
				return vec;
			case Y_NEGATIVE:
				vec = { 0.0f , -1.0f };
				return vec;
		}
	}

	vec = { 0.0f, 0.0f };
	return vec;
}


