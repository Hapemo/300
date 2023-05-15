/*!*****************************************************************************
\file SignalHandler.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 26-09-2022
\brief
This file contains the function HandleSignal which handles the different
signal sent by the program and prints into the crash log.
*******************************************************************************/
#pragma once
#include <csignal>
#include "Logger.h"

/*!*****************************************************************************
\brief
Handles the different signal sent by the program and prints into the crash log.

\param int _signalNumber
The signal received.
*******************************************************************************/
void HandleSignal(int _signalNumber)
{
    switch (_signalNumber) {
    case SIGABRT:
        LOG_CRASH("The program has called an abortion.");
        break;
    case SIGTERM:
        LOG_CRASH("The program has received a termination request.");
        break;
    case SIGINT:
        LOG_CRASH("The program has been interrupted externally.");
        break;
    case SIGSEGV:
        LOG_CRASH("The program has detected an invalid access to storage.");
        break;
    case SIGILL:
        LOG_CRASH("The program has detected an illegal instruction.");
        break;
    case SIGFPE:
        LOG_CRASH("The program has detected an erroneous arithmetic operation.");
        break;
    }
    Logger::GetInstance()->EndRun();
}

/*
SIGTERM	termination request, sent to the program
SIGSEGV	invalid memory access(segmentation fault)
SIGINT	external interrupt, usually initiated by the user
SIGILL	invalid program image, such as illegal instruction
SIGABRT	abnormal termination condition, as is e.g.initiated by std::abort()
SIGFPE	erroneous arithmetic operation such as divide by zero or an operation resulting in overflow
*/
