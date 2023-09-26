#pragma once
#include "Input/Input.h"		// Input Keys
#include "InputActionMap.h"		// A vector to store all the input actions mapped to their context.
#include "InputAction.h"		// Contains [Actions], [InputMapping] -> Keys Mapped to [Actions]

class InputActionMap;
class InputAction;

class InputMapSystem
{
public:
	InputMapSystem() = default;

public:
	void Init();
	void Update();
	void Exit();
	
	void AddActionMap(InputActionMap& action_map);
	bool CheckActionInput(InputAction action, E_KEY key_pressed);	// Used in Scripting Side

private:
	std::vector<InputActionMap> mActionMaps; // A collection of [Action Maps] -> [ActionMap] contains 1 or more [Actions]
};