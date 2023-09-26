#include "Input/InputMapSystem.h"


void InputMapSystem::Init()
{	
	InputActionMap general_movement = InputActionMap("General Movement");
	general_movement.Enable();
	general_movement.AddAction("move");
	// general_movement.AddAction("jump");

	InputAction& move_action = general_movement.FindAction("move");
	move_action.Enable();
	move_action.AddKeyBinding(KEY_BIND::KEY_UP, E_STATE::PRESS, E_KEY::UP);
	move_action.AddKeyBinding(KEY_BIND::KEY_DOWN, E_STATE::PRESS, E_KEY::DOWN);
	move_action.AddKeyBinding(KEY_BIND::KEY_LEFT, E_STATE::PRESS, E_KEY::LEFT);
	move_action.AddKeyBinding(KEY_BIND::KEY_RIGHT, E_STATE::PRESS, E_KEY::RIGHT);

	glm::vec2 vec_response_up = move_action.ReadValue(E_KEY::UP);
	glm::vec2 vec_response_down = move_action.ReadValue(E_KEY::DOWN);
	glm::vec2 read_vec;

	AddActionMap(general_movement);

	E_KEY key_pressed = E_KEY::DOWN;

	read_vec = move_action.ReadValue(key_pressed);
}

void InputMapSystem::Update()
{	
	// [9/26] - Functionality Moved to [CheckActionInput()] : Check when needed (used in the script)
	for (auto& action_map : mActionMaps)
	{
		if (action_map.isEnabled()) // In side [InputActionMap]
		{
			for (auto& action_pair : action_map.GetActions())
			{
				auto& action = action_pair.second;

				for (auto& binding_pair : action.GetKeyBindings())
				{
					if (binding_pair.second.key_binding == E_KEY::UNKNOWN)
					{
						break;
					}

					E_KEY   key_bind = binding_pair.second.key_binding;
					E_STATE key_state = binding_pair.second.key_state;

					if (Input::CheckKey(key_state, key_bind))
					{
						std::cout << "Action Map Triggered!!! : " << action_map.GetActionMapName() << std::endl;
						glm::vec2 vec_response = action_pair.second.ReadValue(key_bind);
						std::cout << "Vector Response: (" << vec_response.x << "," << vec_response.y << ")" << std::endl;
					}
				}
			}
		}
	}
}

void InputMapSystem::Exit()
{

}

void InputMapSystem::AddActionMap(InputActionMap& action_map)
{
	mActionMaps.push_back(action_map);
}

