/*!*************************************************************************
****
\file					 InputMapSystem.cpp
\author(s)	   Cheong Ming Lun, Jazz Teoh Yu Jue
\par DP email: m.cheong@digipen.edu, j.teoh@digipen.edu
\date		   16-8-2023
\brief

This file contains the base InputMapSystem.h that supports mapping of inputs through
the editor to physical keys.
****************************************************************************/
#include "Input/InputMapSystem.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Components.h"
#include "ConfigManager.h"
#include "Misc.h"
#include "Debug/Logger.h"
#include "SparseSet.h"

//const std::array<std::pair<std::string, E_KEY>, 129> InputMapSystem::mE_KEYMap{ EKeyMappingInit() };
const std::array<std::string, E_KEY::KEY_TOTAL> InputMapSystem::mE_KEYArray{ EKeyArrayInit() };

InputMapSystem::InputMapSystem() {}

void InputMapSystem::Init() {
	std::fstream file;
	file.open(ConfigManager::GetValue("KeybindPath") + "keybind.txt", std::ios_base::in);
	if (!file.is_open()) {
		PASSERTMSG(false, std::string("File " + ConfigManager::GetValue("KeybindPath") + " not found.\n").c_str());
		return;
	}

	SparseSet<int, 100> sparseSet{};

	auto tempMap = Misc::TextFileToMap(file);
	for (auto [action, ekeyName] : tempMap) {

		auto correctEkey = GetEKeyFromName(ekeyName);
		mKeybindMapString.insert({ action, ekeyName });
		mKeybindMap.insert(std::pair<std::string, E_KEY>(action, correctEkey));
		if (correctEkey == E_KEY::ERROR_EKEY)
			PWARNING("InputSystem Error: unable to load %s as it does not exist. Check the name in E_KEY list again!", ekeyName.c_str());
	}

}

void InputMapSystem::AddKeybind(std::string const& actionName, E_KEY key) {
	mKeybindMap[actionName] = key;
	mKeybindMapString[actionName] = GetEKeyName(key);
}

void InputMapSystem::AddEmptyKeybind(std::string const& actionName) {
	mKeybindMap[actionName] = E_KEY::ERROR_EKEY;
	mKeybindMapString[actionName] = "";
}

void InputMapSystem::RemoveKeybind(std::string const& actionName) {
	mKeybindMap.erase(actionName);
	mKeybindMapString.erase(actionName);
}

void InputMapSystem::SaveKeybind() {
	std::fstream file;
	file.open(ConfigManager::GetValue("KeybindPath") + "keybind.txt", std::ios_base::out);
	if (!file.is_open()) {
		PASSERTMSG(false, std::string("File " + ConfigManager::GetValue("KeybindPath") + " not found.\n").c_str());
		return;
	}

	for (auto [actionName, ekeyName] : mKeybindMapString) {
		file << actionName << ": " << ekeyName << '\n';
		mKeybindMap[actionName] = GetEKeyFromName(ekeyName);
	}

	file.close();
}


bool InputMapSystem::CheckButton(std::string const& actionName, E_STATE state) {
	E_KEY ekey{};
	try {
		ekey = mKeybindMap.at(actionName);
	} catch (const std::out_of_range& oor) {
		PWARNING("Attempted to check for non-existant action \"%s\". Insert the action in setting > Edit Keybind before using it!");
	}
	return Input::CheckKey(state, ekey);
}

bool InputMapSystem::GetButton(std::string const& actionName) {
	return CheckButton(actionName, HOLD);
}

bool InputMapSystem::GetButtonUp(std::string const& actionName) {
	return CheckButton(actionName, RELEASE);
}

bool InputMapSystem::GetButtonDown(std::string const& actionName) {
	return CheckButton(actionName, PRESS);
}

bool InputMapSystem::GetKey(E_KEY ekey) {
	return Input::CheckKey(HOLD, ekey);
}

bool InputMapSystem::GetKeyUp(E_KEY ekey) {
	return Input::CheckKey(RELEASE, ekey);
}

bool InputMapSystem::GetKeyDown(E_KEY ekey) {
	return Input::CheckKey(PRESS, ekey);
}

std::string InputMapSystem::GetEKeyName(E_KEY ekey) { 
	return mE_KEYArray[ekey];
	//return std::find_if(mE_KEYMap.begin(), mE_KEYMap.end(), [ekey] (std::pair<std::string, E_KEY> item)->bool { return item.second == ekey; })->first; 
}

E_KEY InputMapSystem::GetEKeyFromName(std::string const& ekeyName) { 
	for (int i = 0; i < mE_KEYArray.size(); ++i)
		if (mE_KEYArray[i] == ekeyName)
			return static_cast<E_KEY>(i);
	
	return E_KEY::ERROR_EKEY;
}

std::string& InputMapSystem::GetActionEKeyName(std::string const& action) { 
	return mKeybindMapString[action];
}



