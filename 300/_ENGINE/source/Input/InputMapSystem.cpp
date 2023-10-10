/*!*************************************************************************
****
\file		   InputMapSystem.cpp
\author(s)	   Cheong Ming Lun
\par DP email: m.cheong@digipen.edu
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

const std::array<std::pair<std::string, E_KEY>, 129> InputMapSystem::mE_KEYMap{ EKeyMappingInit() };
//std::unordered_map<std::string, E_KEY> InputMapSystem::mKeybindMap;

InputMapSystem::InputMapSystem() {}


void InputMapSystem::Init() {
	std::fstream file;
	file.open(ConfigManager::GetValue("KeybindPath") + "keybind.txt", std::ios_base::in);
	if (!file.is_open()) {
		PASSERTMSG(false, std::string("File " + ConfigManager::GetValue("KeybindPath") + " not found.\n").c_str());
		return;
	}

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
}

void InputMapSystem::RemoveKeybind(std::string const& actionName) {
	mKeybindMap.erase(actionName);
}

void InputMapSystem::SaveKeybind() {
	std::fstream file;
	file.open(ConfigManager::GetValue("KeybindPath") + "keybind.txt", std::ios_base::out);
	if (!file.is_open()) {
		PASSERTMSG(false, std::string("File " + ConfigManager::GetValue("KeybindPath") + " not found.\n").c_str());
		return;
	}

	for (auto [actionName, ekey] : mKeybindMap) {
		file << actionName << ": " << std::find_if(mE_KEYMap.begin(), mE_KEYMap.end(), [ekey] (std::pair<std::string, E_KEY> item)->bool { return item.second == ekey; })->first << '\n';
	}

	file.close();
}

int InputMapSystem::CheckEKeyMap(std::string key_string) {
	return 0;
	//return e_key_mapping[key_string];
}