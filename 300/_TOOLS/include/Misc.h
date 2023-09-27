#pragma once
#include "pch.h"

struct Misc
{
	static void MemLeakCheck(int breakAlloc = -1);

    // Get file path and return file name, without directory and extension
    static std::string GetFileName(std::string const& _filepath) {
        size_t lastSep = _filepath.find_last_of("/\\");
        size_t lastStop = _filepath.find_last_of(".");

        if (lastSep != std::string::npos && lastStop != std::string::npos && lastStop > lastSep)
            return _filepath.substr(lastSep + 1, lastStop - lastSep - 1);

        return "";
    }

};