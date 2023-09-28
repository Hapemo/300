#include "Input/InputActionMap.h"

InputActionMap::InputActionMap() : mActionMap_name("Action Map")
{

}

InputActionMap::InputActionMap(std::string action_map_name) : mActionMap_name(action_map_name)
{

}

void InputActionMap::RenameActionMap(std::string rename)
{
	mActionMap_name = rename;
}

void InputActionMap::Enable()
{
	isEnable = true;
}

void InputActionMap::Disable()
{
	isEnable = false;
}

bool InputActionMap::isEnabled()
{
	return isEnable;
}

InputAction& InputActionMap::FindAction(std::string action_name)
{
	auto action_it = mActions.find(action_name);

	if (action_it != mActions.end()) // Action Found.
	{
		return action_it->second;
	}
	/*else
	{
		PERROR("ACTION NOT FOUND.");
		return;
	}*/
}

void InputActionMap::AddAction(std::string action_name)
{
	InputAction new_action = InputAction(action_name);
	mActions.insert(std::make_pair(action_name, new_action));
}

std::unordered_map<std::string, InputAction>& InputActionMap::GetActions()
{
	return mActions;
}

std::string InputActionMap::GetActionMapName()
{
	return mActionMap_name;
}

