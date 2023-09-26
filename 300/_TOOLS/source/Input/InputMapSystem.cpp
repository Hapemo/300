#include "Input/InputMapSystem.h"


void InputMapSystem::Init()
{
	InputActionMap general_movement = InputActionMap("General Movement"); 
	general_movement.Enable();
	general_movement.AddAction("move");

	InputAction& move_action = general_movement.FindAction("move");
	move_action.Enable();
	move_action.AddKeyBinding("Up", E_STATE::PRESS, E_KEY::UP);
	
	AddActionMap(general_movement);

	//CheckActionInput(move_action, E_KEY::UP);

	glm::vec2 vec_response = move_action.ReadValue();

}

void InputMapSystem::Update()
{	

	// [9/26] - Functionality Moved to [CheckActionInput()] : Check when needed (used in the script)
	//for (auto& action_map : mActionMaps)
	//{
	//	if (action_map.isEnabled()) // This [Action Map] is currently set to be used...
	//	{
	//		for (auto& action_pair : action_map.GetActions()) // Go through [Action Map]'s list of [Actions]
	//		{
	//			auto& action = action_pair.second; // Retrieve each [Action] 

	//			for (auto& binding_pair : action.GetKeyBindings()) // Iterate through [Key Bindings]
	//			{
	//				E_KEY   key_bind = binding_pair.second.key_binding;
	//				E_STATE key_state = binding_pair.second.key_state;

	//				if (Input::CheckKey(key_state, key_bind))
	//				{
	//					action.UpdateState(STARTED);
	//					std::cout << "Action Map Triggered!!! : " << action_map.GetActionMapName() << std::endl;
	//				}
	//			}
	//		}
	//	}
	//}
}

void InputMapSystem::Exit()
{

}

void InputMapSystem::AddActionMap(InputActionMap& action_map)
{
	mActionMaps.push_back(action_map);
}

// Used in Update() Loop
bool InputMapSystem::CheckActionInput(InputAction action, E_KEY key_pressed)
{
	// Run through all the actions that are enabled.
	for (auto& action_map : mActionMaps)
	{	
		auto& actions = action_map.GetActions();

		// Check if the action exists in the action map
		auto action_it = actions.find(action.mActionName);

		if (action_it != actions.end())
		{
			InputAction& found_action = action_it->second;

			for (auto& binding : found_action.GetKeyBindings())
			{
				E_KEY key_bind = binding.second.key_binding;

				if (Input::CheckKey(PRESS, key_bind))
				{
					std::cout << "KEY PRESSED." << std::endl;
					return true;
				}
			}
		}
	}

	return false; // Couldn't Find Input
}





//InputActionMap& InputMapSystem::GetActionMap(std::string action_map_name)
//{
//
//}
