/*!*****************************************************************************
\file Logger.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 26-09-2022
\brief
This file contains the function definitions of the class Logger.
The Logger class logs messages and errors of different levels and
also handles the assertion.
*******************************************************************************/

#pragma once
#include "Logger.h"

std::shared_ptr<Logger> Logger::mInstance = nullptr;

/*!*****************************************************************************
\brief
Creates a new log type.

\param std::string _newType
The name of the new log type.
*******************************************************************************/
void Logger::CreateNew(std::string _newType) {
    std::string newType = Util::UpperString(_newType);
    mLogNames.push_back(newType);

    mLogTypesVec.push_back({ newType, Util::LowerString(newType) + ".log", (int)mLogNames.size() });
    mLogFilesVec.resize(mLogTypesVec.size());

    size_t newIndex = mLogTypesVec.size() - 1;
    mLogFilesVec[newIndex].open(mFilepath + mLogTypesVec[newIndex].filename.c_str(), std::ios_base::in);
    CreateTempFile(mLogFilesVec[newIndex], mLogTypesVec[newIndex].filename); // Copy current data to temp file
    mLogFilesVec[newIndex].close();
    mLogFilesVec[newIndex].open(mFilepath + mLogTypesVec[newIndex].filename.c_str(), std::ios_base::out); // reopen log file as out stream
    mLogFilesVec[newIndex] << "------ NEW RUN ------\n";
}

/*!*****************************************************************************
\brief
Default constructor for the Logger class.
*******************************************************************************/
Logger::Logger() {
    if (!std::filesystem::exists(mFilepath))
        std::filesystem::create_directory(mFilepath);

    // CREATE VECTOR WITH LOG TYPE DETAILS
    for (size_t i = 0; i < mLogNames.size(); ++i)
        mLogTypesVec.push_back({ Util::UpperString(mLogNames[i]), Util::LowerString(mLogNames[i]) + ".log", (int)i });

    // opening specific log file types
    mLogFilesVec.resize(mLogTypesVec.size());
    for (size_t i = 0; i < mLogNames.size(); ++i) {
        mLogFilesVec[i].open(mFilepath + mLogTypesVec[i].filename.c_str(), std::ios_base::in);
        CreateTempFile(mLogFilesVec[i], mLogTypesVec[i].filename); // Copy current data to temp file
        mLogFilesVec[i].close();
        mLogFilesVec[i].open(mFilepath + mLogTypesVec[i].filename.c_str(), std::ios_base::out); // reopen log file as out stream
        mLogFilesVec[i] << "------ NEW RUN ------\n";
    }

    // opening human readable log file
    mLogFile.open(mFilepath + mLogFilename, std::ios_base::in);
    CreateTempFile(mLogFile, mLogFilename); // Copy current data to temp file
    mLogFile.close();
    mLogFile.open(mFilepath + mLogFilename, std::ios_base::out); // reopen log file as out stream
    mLogFile << "------ NEW RUN ------\n";

    // opening full log file with file name and line number
    mFullLogFile.open(mFilepath + mFullLogFilename, std::ios_base::in);
    CreateTempFile(mFullLogFile, mFullLogFilename); // Copy current data to temp file
    mFullLogFile.close();
    mFullLogFile.open(mFilepath + mFullLogFilename, std::ios_base::out); // reopen log file as out stream
    mFullLogFile << "------ NEW RUN ------\n";
}

/*!*****************************************************************************
\brief
End the current run for the logger.
*******************************************************************************/
void Logger::EndRun() {
    mLogFile << "------ RUN ENDED ------\n";
    DeleteTempFile(mLogFile, mLogFilename);
    mLogFile.close();

    mFullLogFile << "------ RUN ENDED ------\n";
    DeleteTempFile(mFullLogFile, mFullLogFilename);
    mFullLogFile.close();

    for (size_t i = 0; i < mLogNames.size(); ++i) {
        mLogFilesVec[i] << "------ RUN ENDED ------\n";
        DeleteTempFile(mLogFilesVec[i], mLogTypesVec[i].filename);
        mLogFilesVec[i].close();
    }
}

/*!*****************************************************************************
\brief
Destructor for the Logger class.
*******************************************************************************/
Logger::~Logger() {
    EndRun();
}

/*!*****************************************************************************
\brief
Creates a temporary log file.
*******************************************************************************/
void Logger::CreateTempFile(std::fstream& _logfile, std::string _logFilename) {
    std::string tempFilename = mTempFilename + _logFilename;
    mTempFile.open(mFilepath + tempFilename, std::ios_base::out);
    std::string line;
    if (_logfile && mTempFile) {
        while (getline(_logfile, line))
            mTempFile << line << "\n";
    } else {
#ifdef NDEBUG
#else
        std::cout << "Failed to create temp log file: " << tempFilename << "\n";
#endif
    }
    mTempFile.close();
}

/*!*****************************************************************************
\brief
Delete the temporary log file.
*******************************************************************************/
void Logger::DeleteTempFile(std::fstream& _logfile, std::string _logFilename) {
    const std::string tempFilename = mTempFilename + _logFilename;
    mTempFile.open(mFilepath + tempFilename, std::ios_base::in);
    size_t maxline = 0;
    std::string line;
    if (_logfile && mTempFile) {
        while (getline(mTempFile, line)) {
            if (maxline < MAX_LOG_HISTORY) { // checks for log history length
                // replace NEW RUN header with PREV RUN
                size_t index;
                std::string prevRun = "------ PREV RUN ------";
                while ((index = line.find("------ NEW RUN ------")) != std::string::npos)
                    line.replace(index, prevRun.length(), prevRun);
                _logfile << line << "\n";
                ++maxline;
            }
            else {
#ifdef NDEBUG
#else
                std::cout << "Older log data has been cleared. Log file longer than maximum allowed history count: " << tempFilename << "\n";
#endif
                break;
            }
        }
    }
    else {
#ifdef NDEBUG
#else
        std::cout << "Temp log file doesn't exist: " << tempFilename << "\n";
#endif
    }
    mTempFile.close();
    if (!std::filesystem::remove((mFilepath + tempFilename).c_str())) {
#ifdef NDEBUG
#else
        std::cout << "Error removing temp log file: " << tempFilename << "\n";
#endif
    }
}
