#include "Input/InputMapSystem.h"


void InputMapSystem::Init()
{
	InputActionMap general_movement = InputActionMap("General Movement"); 
	general_movement.Enable();
	general_movement.AddAction("move");

	InputAction& move_action = general_movement.FindAction("move");
	move_action.Enable();
	move_action.AddKeyBinding(KEY_BIND::KEY_UP, E_STATE::PRESS, E_KEY::UP);
	
	AddActionMap(general_movement);

	//CheckActionInput(move_action, E_KEY::UP);

	glm::vec2 vec_response = move_action.ReadValue(E_KEY::UP);

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

