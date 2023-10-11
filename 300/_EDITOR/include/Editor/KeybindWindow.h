/**-------------------------------------------------------------------------------------/
 * @file
 *  PrefabWindow.h
 * @author
 *  Han Cheng Wen, c,han@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *	Prefab Scene declaration for editing prefabs
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/
#pragma once
#include "EditorWindow.h"
#include "ECS/ECS.h"


class KeybindWindow : public EditorWindow
{
public:
	/*!*****************************************************************************
	* /brief
		Initialize funtion for imguiWindow
	*******************************************************************************/
	void init();
	/*!*****************************************************************************
	* /brief
		Update funtion for imguiWindow
	*******************************************************************************/
	void update();


	static bool openWindow;

};