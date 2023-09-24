#include "Input/InputMapSystem.h"


void InputMapSystem::Init()
{
	InputActionMap general_movement = InputActionMap("General Movement"); 
	general_movement.Enable();
	general_movement.AddAction("move");

	InputAction& move_action = general_movement.FindAction("move");
	move_action.Enable();
	move_action.AddKeyBinding("Up", E_STATE::PRESS, E_KEY::UP);

	glm::vec2 vec_response = move_action.ReadValue();

}

void InputMapSystem::Update()
{

}

void InputMapSystem::Exit()
{

}

void InputMapSystem::AddActionMap(std::string action_map_name)
{
	
}


//InputActionMap& InputMapSystem::GetActionMap(std::string action_map_name)
//{
//
//}
