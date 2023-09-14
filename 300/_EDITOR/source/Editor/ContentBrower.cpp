/*!*************************************************************************
****
\file ContentBrowser.cpp
\author Han ChengWen
\par DP email: c.han@digipen.edu
\date 22-9-2022
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


#include "ContentBrower.h"
#include <stdlib.h>

static const std::filesystem::path s_asset_directory = "../Resources";
bool ContentBrowser::dragCheck = false;

ContentBrowser::ContentBrowser():current_Directory(s_asset_directory) {} // constructor to set base directory

void ContentBrowser::update() 
{


}


void ContentBrowser::init() {

	// empty
}

