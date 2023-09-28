/*!*****************************************************************************
\file Misc.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Pepe Production
\date 24-09-2023
\brief
Miscellaneous helper functions that is useful across the game engine
*******************************************************************************/
#pragma once
#include <string>

struct Misc {
  /*!*****************************************************************************
  Check if there is memory leak
  *******************************************************************************/
	static void MemLeakCheck(int breakAlloc = -1);

  /*!*****************************************************************************
  Get file path and return file name, without directory and extension

  \param _filepath
  - File path in string format, with extension and directory

  \return std::string
  - File name in string format
  *******************************************************************************/
  static std::string GetFileName(std::string const& _filepath);

};