#pragma once
#pragma once
/*!*****************************************************************************
\file main.cpp
\author Kew Yu Jun
\par DP email: k.yujun@digipen.edu
\par Group: Memory Leak Studios
\date 15/05/2023
\brief
Main application that gets called in the main loop. It handles the creation and
start up of window and game system, also runs their update functions.
*******************************************************************************/
#include "pch.h"
#include "ECS/ECS_Systems.h"
#include "Window.hpp"
#include "Camera.hpp"
#include "DebugRenderer.hpp"
#include "Editor/Editor.h"


class EditorApplication {
public:
	/*!*****************************************************************************
	 Initialise the application and it's systems such as window, texture resources
	 and game engine systems
	*******************************************************************************/
	static void Init();

	/*!*****************************************************************************
	 Start up the vital components for application. Should be called in Application
	 Init()
	*******************************************************************************/
	static void StartUp();

	/*!*****************************************************************************
	 initialise Game Engine ECS's systems
	*******************************************************************************/
	static void SystemInit();

	// Main update that runs everythign in a loop
	static void MainUpdate();

	/*!*****************************************************************************
	 Initialisation before start of each game loop.

	 \return bool
	- True if ran successfully, otherwise false
	*******************************************************************************/
	static void FirstUpdate();

	/*!*****************************************************************************
	 Update Game Engine ECS's systems, should be called every game loop
	*******************************************************************************/
	static void SystemUpdate();

	/*!*****************************************************************************
	 Clean up after end of each game loop.
	*******************************************************************************/
	static void SecondUpdate();

	/*!*****************************************************************************
	 Unload and clean up when exiting the application
	*******************************************************************************/
	static void Exit();

private:
	static std::string title;
	static GFX::Window mWindow;
	static GFX::DebugRenderer* mRenderer;
	static Editor mMaineditor;
	GFX::Camera mCamera;
};