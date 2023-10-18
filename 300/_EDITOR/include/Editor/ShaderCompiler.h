/**-------------------------------------------------------------------------------------/
 * @file
 *  ShaderCompiler.h
 * @author
 *  Han Cheng Wen, c,han@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *	Setting Window declaration for editing editor variables such as camera speed etc
 *
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/
#pragma once
#include "EditorWindow.h"
#include "ECS/ECS.h"
#include "ECS/ECS_Systems.h"

class ShaderCompiler : public EditorWindow
{
public:
	/*!*****************************************************************************
	* /brief
		Init funtion for imguiWindow
	*******************************************************************************/
	void init();
	/*!*****************************************************************************
	* /brief
		Initialize funtion for imguiWindow
	*******************************************************************************/
	void update();

	std::string vertStr{};
	std::string fragStr{};
	std::string shaderName{};

	static bool openWindow;
};