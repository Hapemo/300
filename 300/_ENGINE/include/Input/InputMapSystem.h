#pragma once
/*!*************************************************************************
****
\file		   InputMapSystem.h
\author(s)	   Cheong Ming Lun
\par DP email: m.cheong@digipen.edu
\date		   16-8-2023
\brief

This file contains the base InputMapSystem.h that supports mapping of inputs through
the editor to physical keys.
****************************************************************************/
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