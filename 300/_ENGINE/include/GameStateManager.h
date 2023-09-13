/*!*************************************************************************
****
\file GameStateManager.h
\author Lee Kwok Yew Glenn
\par DP email: kwokyewglenn.lee\@digipen.edu
\date 22-9-2022
\brief
This file contains functions that manager the different states of the game.

The functions include:
- GameStateManager
This function creates a default constructor and stores each level's states
into their appropriate array of function pointers.

- Init
This function creates a camera object and initializes the first level's init
state.

- Update
This function calls the current level's update state. When pause, the
update state will not run. When going into the next level, it will call the
free function and initializes the next level.

- Free
This function calls the current level's free state.

- FreeCamera
This function deletes the camera object.

- Save
This function calls the current level's save state.

- Exit
This function exits the application.
****************************************************************************
***/

#pragma once

class GameStateManager
{
public:
/***************************************************************************/
/*!
\brief
This function creates a default constructor and stores each level's states
into their appropriate array of function pointers.
*/
/***************************************************************************/
	GameStateManager();

/***************************************************************************/
/*!
\brief
This function creates a camera object and initializes the first level's init
state.
*/
/***************************************************************************/
	void Init();

/***************************************************************************/
/*!
\brief
This function calls the current level's update state. When pause, the
update state will not run. When going into the next level, it will call the
free function and initializes the next level.
*/
/***************************************************************************/
	void Update();

/***************************************************************************/
/*!
\brief
This function calls the current level's free state.
*/
/***************************************************************************/
	void Free();

/***************************************************************************/
/*!
\brief
This function deletes the camera object.
*/
/***************************************************************************/
	void FreeCamera();

/***************************************************************************/
/*!
\brief
This function calls the current level's save state.
*/
/***************************************************************************/
	void Save();

/***************************************************************************/
/*!
\brief
This function exits the game
*/
/***************************************************************************/
	void Exit();

	bool isPlaying{};
	bool isPaused{};
	std::string sceneFileName{};

private:

};
