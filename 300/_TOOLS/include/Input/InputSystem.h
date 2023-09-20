#pragma once
#include "Input/Input.h"		// Input Keys
#include "InputActionMap.h"		// A vector to store all the input actions mapped to their context.
#include "InputAction.h"		// Contains [Actions], [InputMapping] -> Keys Mapped to [Actions]


class InputAction;
class InputActionMap;

class InputSystem
{
public:
	InputSystem() = default;

public:
	void Init();
	void Update();
	void Exit();

private:
	std::unordered_map<std::string, InputActionMap> mActionMaps; // A collection of [Action Maps] -> [ActionMap] contains 1 or more [Actions]
};