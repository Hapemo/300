/*!*************************************************************************
****
\file ContentBrowser.h
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 28-09-2023
\brief
This file contains ContentBrowser window functions that display
the relative folder and files to a directory stated, (currently hardcoded)
allows user to broswse through the files and drag (.png) files into texture
component (must open) to change texture of object during runtime.

The functions
- update
Contains main loop for the logic of contentbrowsing + Drag and drop functionality


****************************************************************************
***/
#pragma once

#include "Editor.h"
#include <filesystem>
#include "EditorWindow.h"

/*******************************/
/*!
	ContentBrowser Class
*/
/*******************************/
class ContentBrowser : public EditorWindow 
{
public:
/***************************************************************************/
/*!
\brief
Constructor

*/
/***************************************************************************/
	ContentBrowser(); // constructor

/***************************************************************************/
/*!
\brief
Destructor

*/
/***************************************************************************/
	virtual ~ContentBrowser() = default;// destructor
/***************************************************************************/
/*!
\brief
Contains main loop for the logic of contentbrowsing + Drag and drop functionality

*/
/***************************************************************************/
	void init();
/***************************************************************************/
/*!
\brief
	Empty virtual inherit

*/
/***************************************************************************/
	void update();

	//Helper variable for UR
	static bool dragCheck;

private:

	std::string mFilter;
	bool content2{ true };
	std::filesystem::path current_Directory;
};
