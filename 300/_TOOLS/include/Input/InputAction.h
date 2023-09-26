#pragma once
#include "../pch.h"
#include "Input/Input.h"
#include "Input/Subscriber.h" // Subscriber Model

// Similar to Action Callbacks (WIP) - 9/24/2023 
enum INPUT_STATE
{
	DISABLED, 
	WAITING, 
	STARTED,
	PERFORMED,
	CANCELLED
};

enum MAP_TO_AXIS
{
	X_POSITIVE, 
	Y_POSITIVE, 
	X_NEGATIVE,
	Y_NEGATIVE
};

struct InputBinding
{
	E_STATE     key_state;
	E_KEY       key_binding;
	
	MAP_TO_AXIS direction_map; // For Movement? (first)

	void SetDirectionMap(MAP_TO_AXIS dir_map)
	{
		direction_map = dir_map;
	}
};

class InputAction
{
public:
	std::string mActionName;
public:
	InputAction();
	InputAction(std::string action_name);
	InputAction(std::string action_name, E_STATE key_state, E_KEY key_binding);

	void Enable();
	void Disable();
	bool isEnabled();

	void AddKeyBinding(std::string binding_name, E_STATE key_state, E_KEY key_binding);	// Add a (Key-Bindings) to InputAction
	// std::unordered_map<std::string, InputBinding> GetKeyBindings() const;			// Get all the (Key-Bindings) from database
	InputBinding& GetKeyBinding(std::string binding_name);								// Get a Singular (Key-Binding)
	std::unordered_map<std::string, InputBinding>& GetKeyBindings();					// Get the entire (Key-Binding) container
	
	glm::vec2 ReadValue() const;														// Might Template this (can return gamestates?)
	void      UpdateState(INPUT_STATE new_state);

private:

	std::unordered_map<std::string, InputBinding>    mKeyBindings;						// [Limit to 2 bindings] (for now)
	INPUT_STATE									     mCurrentState;						
	bool										     isEnable = false;
	//std::vector<Subscriber>						  mSubscribers;
};