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
#pragma once
#include "pch.h"
#include "ECS/ECS_Systems.h"
#include "Window.hpp"
#include "Camera.hpp"
#include "DebugRenderer.hpp"

class Application {
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

	// Main update that runs everythign in a loop
	static void MainUpdate();

	/*!*****************************************************************************
	 Unload and clean up when exiting the application
	*******************************************************************************/
	static void Exit();

private:
	static std::string title;
	static GFX::Window mWindow;
    static GFX::DebugRenderer* mRenderer;
	static SystemManager systemManager;
    GFX::Camera mCamera;
	
	/*!*****************************************************************************
	 Callback function for GLFW errors

	 \param int _error
	 - Error code from GLFW

	 \param char const* _description
	 - Description of error
	*******************************************************************************/
	static void error_cb(int _error, char const* _description);

	/*!*****************************************************************************
	 Callback function for GLFW framebuffer

	 \param GLFWwindow* _ptrWin
	 - Pointer to window

	 \param _width
	 - Window width

	 \param _height
	 - Window height
	*******************************************************************************/
	static void fbsize_cb(GLFWwindow* _ptrWin, int _width, int _height);
};