#pragma once
#include "Input/Input.h"		// Input Keys
#include "KeyBind.h"


class InputActionMap;
class InputAction;

class InputMapSystem
{
public:
	InputMapSystem();

public:
	void Init();
	void Update();
	void Exit();
	
	// Need this for Lua
	//void AddActionMap(InputActionMap& action_map);
	//std::vector<InputActionMap>& GetActionMaps();
	static std::unordered_map<std::string, int> e_key_mapping;
	
	int CheckEKeyMap(std::string); // Returns the (int equivalent of the enum for E_KEY) -> to be stored in the <InputActionMapEditor> component.

private:
	// Need to Register... (Put in InputMapSystem.Input())
	static void EKeyMappingInit();
};