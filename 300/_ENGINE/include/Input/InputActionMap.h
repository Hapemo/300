#pragma once
#include "InputAction.h"
#include "../../../_ENGINE/include/Debug/Logger.h"

class InputActionMap
{
public:
	InputActionMap();
	InputActionMap(std::string action_name); // Add Actions separately.

	void RenameActionMap(std::string action_name);

	// [Activate Action Group] 
	void Enable();
	void Disable();
	bool isEnabled();

	InputAction& FindAction(std::string action_name);
	void AddAction(std::string action_name);
	std::unordered_map<std::string, InputAction>& GetActions();

	std::string GetActionMapName();

private:
	std::string									  mActionMap_name;		// [Group Name]		 - Action Map Name (Gameplay / Main Menu)
	std::unordered_map<std::string, InputAction>  mActions;				/* [List of Actions] - (1) Action Name(Move / Jump / Crouch / Use)
																							   (2) InputAction : contains (KEY BINDINGS) + (LOGIC?) */
	bool isEnable = false;												// Works like isActive() in unity.													
};