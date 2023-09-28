#include "Input/InputAction.h"
#include "Input/KeyBind.h"

InputAction::InputAction() : mActionName("New Action")
{
	// Initialize the [mKeyBindings] with the basic [4 directional keys]
	for (int i = 0; i < KEY_TOTAL; i++)
	{
		KEY_BIND key = static_cast<KEY_BIND>(i);
		mKeyBindingsNew[key] = InputBinding();

		switch (key)
		{
		case KEY_UP:
			mKeyBindingsNew[key].direction_map = Y_POSITIVE;
			break;
		case KEY_LEFT:
			mKeyBindingsNew[key].direction_map = X_NEGATIVE;
			break;
		case KEY_RIGHT:
			mKeyBindingsNew[key].direction_map = X_POSITIVE;
			break;
		case KEY_DOWN:
			mKeyBindingsNew[key].direction_map = Y_NEGATIVE;
			break;
		default:
			mKeyBindingsNew[key].direction_map = EMPTY;
			break;
		}
	}
}


InputAction::InputAction(std::string action_name) : mActionName(action_name)
{
	for (int i = 0; i < KEY_TOTAL; i++)
	{
		KEY_BIND key = static_cast<KEY_BIND>(i);
		mKeyBindingsNew[key] = InputBinding();

		switch (key)
		{
		case KEY_UP:
			mKeyBindingsNew[key].direction_map = Y_POSITIVE;
			break;
		case KEY_LEFT:
			mKeyBindingsNew[key].direction_map = X_NEGATIVE;
			break;
		case KEY_RIGHT:
			mKeyBindingsNew[key].direction_map = X_POSITIVE;
			break;
		case KEY_DOWN:
			mKeyBindingsNew[key].direction_map = Y_NEGATIVE;
			break;
		default:
			mKeyBindingsNew[key].direction_map = EMPTY;
			break;
		}
	}
}
//InputAction::InputAction(std::string action_name, E_STATE key_state, E_KEY key_binding) : mActionName(action_name) 
//{
//	InputBinding new_binding = InputBinding(key_state, key_binding); 
//	mKeyBindingsNew.insert(std::make_pair(action_name, new_binding));
//}

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

void InputAction::AddKeyBinding(KEY_BIND key_bind, E_STATE key_state, E_KEY key_binding)
{
	InputBinding new_binding = InputBinding({ key_state, key_binding });

	mKeyBindingsNew[key_bind].key_binding = key_binding;
	mKeyBindingsNew[key_bind].key_state = key_state;
}

void InputAction::RemoveKeyBinding(KEY_BIND key_bind)
{
	mKeyBindingsNew[key_bind].key_binding = UNKNOWN;
	mKeyBindingsNew[key_bind].key_state = NOTPRESS;
}

std::unordered_map<KEY_BIND, InputBinding>& InputAction::GetKeyBindings()
{
	return mKeyBindingsNew;
}

glm::vec2 InputAction::ReadValue(E_KEY key_pressed) const
{
	glm::vec2 vec;

	for (const auto& input_binding : mKeyBindingsNew)
	{
		if (input_binding.second.key_binding == key_pressed)
		{
			switch (input_binding.second.direction_map)
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
	}

	vec = { 0.0f, 0.0f };
	return vec;
}


