/*!*****************************************************************************
\file Misc.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Pepe Production
\date 24-09-2023
\brief
Miscellaneous helper functions that is useful across the game engine
*******************************************************************************/
#include "Misc.h"
#include "pch.h"
#include "crtdbg.h"

void Misc::MemLeakCheck(int breakAlloc) {
    //Set the leak checking flag
    int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);

    //If a valid break alloc provided set the breakAlloc
    if (breakAlloc != -1) _CrtSetBreakAlloc(breakAlloc);
}

std::string Misc::GetFileName(std::string const& _filepath) {
  size_t lastSep = _filepath.find_last_of("/\\");
  size_t lastStop = _filepath.find_last_of(".");

  if (lastSep != std::string::npos && lastStop != std::string::npos && lastStop > lastSep)
    return _filepath.substr(lastSep + 1, lastStop - lastSep - 1);

  return "";
}