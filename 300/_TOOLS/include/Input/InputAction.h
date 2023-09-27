#pragma once
#include "../pch.h"
#include "Input/Input.h"
#include "Input/Subscriber.h" // Subscriber Model

// Similar to Action Callbacks (WIP) - 9/24/2023 
enum MAP_TO_AXIS
{
	X_POSITIVE, 
	Y_POSITIVE, 
	X_NEGATIVE,
	Y_NEGATIVE, 
	EMPTY
};

enum KEY_BIND
{
	KEY_UP,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_TOTAL
};

struct InputBinding
{
	E_STATE     key_state   = E_STATE::NOTPRESS;
	E_KEY       key_binding = E_KEY::UNKNOWN;
	
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

	void AddKeyBinding(KEY_BIND key_bind, E_STATE key_state, E_KEY key_binding);
	void RemoveKeyBinding(KEY_BIND key_bind);
	
	std::unordered_map<KEY_BIND, InputBinding>& GetKeyBindings();					// Get the entire (Key-Binding) container
	
	glm::vec2 ReadValue(E_KEY key_pressed) const;									// Might Template this (can return gamestates?)

private:
	std::unordered_map<KEY_BIND, InputBinding>       mKeyBindingsNew;					// Will only have the [Base 4 Directions Map] - Defined in (Init)
	std::unordered_map<std::string, InputBinding>    mKeyBindings;						// [Limit to 2 bindings] (for now)
	//INPUT_STATE									     mCurrentState;						
	bool										     isEnable = false;
	//std::vector<Subscriber>						  mSubscribers;
};