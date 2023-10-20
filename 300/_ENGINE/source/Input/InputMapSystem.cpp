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

const std::array<std::pair<std::string, E_KEY>, 129> InputMapSystem::mE_KEYMap{ EKeyMappingInit() };

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
	for (auto [action, ekey] : tempMap) {
		auto correctEkey = std::find_if(mE_KEYMap.begin(), mE_KEYMap.end(), [ekey] (std::pair<std::string, E_KEY> item)->bool { return item.first == ekey; });
		if (correctEkey == mE_KEYMap.end()) {
			PWARNING("InputSystem Error: unable to load %s as it does not exist. Check the name in E_KEY list again!", ekey.c_str());
			continue;
		}

		mKeybindMap.insert(std::pair<std::string, E_KEY>(action, correctEkey->second));
	}

}

void InputMapSystem::AddKeybind(std::string const& actionName, E_KEY key) {
	mKeybindMap[actionName] = key;
	SaveKeybind();
}

void InputMapSystem::RemoveKeybind(std::string const& actionName) {
	mKeybindMap.erase(actionName);
	SaveKeybind();
}

void InputMapSystem::SaveKeybind() {
	std::fstream file;
	file.open(ConfigManager::GetValue("KeybindPath") + "keybind.txt", std::ios_base::out);
	if (!file.is_open()) {
		PASSERTMSG(false, std::string("File " + ConfigManager::GetValue("KeybindPath") + " not found.\n").c_str());
		return;
	}

	for (auto [actionName, ekey] : mKeybindMap) {
		file << actionName << ": " << GetEKeyName(ekey) << '\n';
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




