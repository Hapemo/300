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

std::unordered_map<std::string, int> InputMapSystem::e_key_mapping;


/******************************************************************************/
/*!
	[Default Constructor] - InputMapSystem()
	- Calls upon [Helper Function - EKeyMappingInit()] 
		- This helps with initializing the keys that we have registered in GLFW.
		- Since the GLFW is not contiguous inside the enumeration.
		- Used to Cross-Reference the custom-bindings the user chooses.
 */
 /******************************************************************************/
InputMapSystem::InputMapSystem() { EKeyMappingInit(); }

/******************************************************************************/
/*!
	 Init()
 */
 /******************************************************************************/
void InputMapSystem::Init() {}


/******************************************************************************/
/*!
	 Update()
 */
 /******************************************************************************/
void InputMapSystem::Update()
{
	auto ActionMapEntities = systemManager->ecs->GetEntitiesWith<InputActionMapEditor>();
	int size = ActionMapEntities.size();

	for (Entity entity : ActionMapEntities)
	{
		InputActionMapEditor& editor_component = entity.GetComponent<InputActionMapEditor>();

		for (auto& action : editor_component.mActionMap)
		{
			if (action.second.isEnabled)
			{
				for (auto& binding_pair : action.second.mDefinedKeyMap)
				{
					if (binding_pair.second == E_KEY::UNKNOWN)
					{
						break;
					}

					else if (Input::CheckKey(PRESS, binding_pair.second))
					{
						std::cout << "Action Map Triggered!!! : " << editor_component.mSelectedMapName << std::endl;
						PINFO("Acion Map Triggered!!!: %s", editor_component.mSelectedMapName.c_str());

						glm::vec2 mapped_vector = action.second.ReadValue(binding_pair.second);
						PINFO("Vector Response: (%lf,%lf)", mapped_vector.x, mapped_vector.y);
					}	
				}
			}
		}

	}
}

/******************************************************************************/
/*!
	 Exit()
 */
 /******************************************************************************/
void InputMapSystem::Exit()
{

}

/******************************************************************************/
/*!
	 CheckEKeyMap()
	 - Checks the (enum - int) equivalent of the requested "key string"
 */
 /******************************************************************************/
int InputMapSystem::CheckEKeyMap(std::string key_string)
{
	return e_key_mapping[key_string];
}

/******************************************************************************/
/*!
	 EKeyMappingInit()
	 - Custom Maps out existing GLFW bindings to "std::string" equivalents that 
	 works in tandem with the editor side.
 */
 /******************************************************************************/
void InputMapSystem::EKeyMappingInit() {
	e_key_mapping.reserve(230);
	e_key_mapping["Unknown"] = -1;
	e_key_mapping["Space_"] = 32;
	e_key_mapping["'"] = 39;
	e_key_mapping[","] = 44;
	e_key_mapping["-"] = 45;
	e_key_mapping["."] = 46;
	e_key_mapping["/"] = 47;
	e_key_mapping["0"] = 48;
	e_key_mapping["1"] = 49;
	e_key_mapping["2"] = 50;
	e_key_mapping["3"] = 51;
	e_key_mapping["4"] = 52;
	e_key_mapping["5"] = 53;
	e_key_mapping["6"] = 54;
	e_key_mapping["7"] = 55;
	e_key_mapping["8"] = 56;
	e_key_mapping["9"] = 57;
	e_key_mapping[":"] = 59;
	e_key_mapping["="] = 61;
	e_key_mapping["A"] = 65;
	e_key_mapping["B"] = 66;
	e_key_mapping["C"] = 67;
	e_key_mapping["D"] = 68;
	e_key_mapping["E"] = 69;
	e_key_mapping["F"] = 70;
	e_key_mapping["G"] = 71;
	e_key_mapping["H"] = 72;
	e_key_mapping["I"] = 73;
	e_key_mapping["J"] = 74;
	e_key_mapping["K"] = 75;
	e_key_mapping["L"] = 76;
	e_key_mapping["M"] = 77;
	e_key_mapping["N"] = 78;
	e_key_mapping["O"] = 79;
	e_key_mapping["P"] = 80;
	e_key_mapping["Q"] = 81;
	e_key_mapping["R"] = 82;
	e_key_mapping["S"] = 83;
	e_key_mapping["T"] = 84;
	e_key_mapping["U"] = 85;
	e_key_mapping["V"] = 86;
	e_key_mapping["W"] = 87;
	e_key_mapping["X"] = 88;
	e_key_mapping["Y"] = 89;
	e_key_mapping["Z"] = 90;
	e_key_mapping["["] = 91;
	e_key_mapping["\\"] = 92;
	e_key_mapping["]"] = 93;
	e_key_mapping["`"] = 96;
	e_key_mapping["WORLD_1"] = 161;
	e_key_mapping["WORLD_2"] = 162;
	e_key_mapping["ESC"] = 256;
	e_key_mapping["ENTER"] = 257;
	e_key_mapping["TAB"] = 258;
	e_key_mapping["BACKSPACE"] = 259;
	e_key_mapping["INSERT"] = 260;
	e_key_mapping["DELETE"] = 261;
	e_key_mapping["RIGHT"] = 262;
	e_key_mapping["LEFT"] = 263;
	e_key_mapping["DOWN"] = 264;
	e_key_mapping["UP"] = 265;
	e_key_mapping["PAGE_UP"] = 266;
	e_key_mapping["PAGE_DOWN"] = 267;
	e_key_mapping["HOME"] = 268;
	e_key_mapping["END"] = 269;
	e_key_mapping["CAPS_LOCK"] = 280;
	e_key_mapping["SCROLL_LOCK"] = 281;
	e_key_mapping["NUM_LOCK"] = 282;
	e_key_mapping["PRINT_SCREEN"] = 283;
	e_key_mapping["PAUSE"] = 284;
	e_key_mapping["F1"] = 290;
	e_key_mapping["F2"] = 291;
	e_key_mapping["F3"] = 292;
	e_key_mapping["F4"] = 293;
	e_key_mapping["F5"] = 294;
	e_key_mapping["F6"] = 295;
	e_key_mapping["F7"] = 296;
	e_key_mapping["F8"] = 297;
	e_key_mapping["F9"] = 298;
	e_key_mapping["F10"] = 299;
	e_key_mapping["F11"] = 300;
	e_key_mapping["F12"] = 301;
	e_key_mapping["F13"] = 302;
	e_key_mapping["F14"] = 303;
	e_key_mapping["F15"] = 304;
	e_key_mapping["F16"] = 305;
	e_key_mapping["F17"] = 306;
	e_key_mapping["F18"] = 307;
	e_key_mapping["F19"] = 308;
	e_key_mapping["F20"] = 309;
	e_key_mapping["F21"] = 310;
	e_key_mapping["F22"] = 311;
	e_key_mapping["F23"] = 312;
	e_key_mapping["F24"] = 313;
	e_key_mapping["F25"] = 314;
	e_key_mapping["PAD_0"] = 320;
	e_key_mapping["PAD_1"] = 321;
	e_key_mapping["PAD_2"] = 322;
	e_key_mapping["PAD_3"] = 323;
	e_key_mapping["PAD_4"] = 324;
	e_key_mapping["PAD_5"] = 325;
	e_key_mapping["PAD_6"] = 326;
	e_key_mapping["PAD_7"] = 327;
	e_key_mapping["PAD_8"] = 328;
	e_key_mapping["PAD_9"] = 329;
	e_key_mapping["PAD_DECIMAL"] = 330;
	e_key_mapping["PAD_DIVIDE"] = 331;
	e_key_mapping["PAD_MULTIPLY"] = 332;
	e_key_mapping["PAD_SUBTRACT"] = 333;
	e_key_mapping["PAD_ADD"] = 334;
	e_key_mapping["PAD_ENTER"] = 335;
	e_key_mapping["PAD_EQUAL"] = 336;
	e_key_mapping["LEFT_SHIFT"] = 340;
	e_key_mapping["LEFT_CONTROL"] = 341;
	e_key_mapping["LEFT_ALT"] = 342;
	e_key_mapping["LEFT_SUPER"] = 343;
	e_key_mapping["RIGHT_SHIFT"] = 344;
	e_key_mapping["RIGHT_CONTROL"] = 345;
	e_key_mapping["RIGHT_ALT"] = 346;
	e_key_mapping["RIGHT_SUPER"] = 347;
	e_key_mapping["MENU"] = 348;
	e_key_mapping["M_BUTTON_L"] = 349;
	e_key_mapping["M_BUTTON_R"] = 350;
	e_key_mapping["M_BUTTON_M"] = 351;
	e_key_mapping["M_BUTTON_4"] = 352;
	e_key_mapping["M_BUTTON_5"] = 353;
	e_key_mapping["M_BUTTON_6"] = 354;
	e_key_mapping["M_BUTTON_7"] = 355;
	e_key_mapping["M_BUTTON_8"] = 356;
}