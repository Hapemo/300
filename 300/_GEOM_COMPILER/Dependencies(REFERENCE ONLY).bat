@echo OFF
setlocal enabledelayedexpansion
set GEOM_COMPILER_PATH="%cd%"

rem --------------------------------------------------------------------------------------------------------
rem Set the color of the terminal to blue with yellow text
rem --------------------------------------------------------------------------------------------------------
COLOR 8E
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore Cyan Welcome I am your GEOM_COMPILER updater bot, let me get to work...
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
echo.

:DOWNLOAD_DEPENDENCIES
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DOWNLOADING DEPENDENCIES
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
echo.

rmdir "../dependencies" /S /Q

rem ------------------------------------------------------------
rem XCORE
rem ------------------------------------------------------------
:XCORE
rmdir "../dependencies/xcore" /S /Q
git clone https://gitlab.com/LIONant/xcore.git "../dependencies/xcore"
if %ERRORLEVEL% GEQ 1 goto :ERROR
cd ../dependencies/xcore/builds
if %ERRORLEVEL% GEQ 1 goto :ERROR
call UpdateDependencies.bat "return"
if %ERRORLEVEL% GEQ 1 goto :ERROR
cd /d %GEOM_COMPILER_PATH%
if %ERRORLEVEL% GEQ 1 goto :ERROR

rem ------------------------------------------------------------
rem ASSIMP
rem ------------------------------------------------------------
:ASSIMP
rmdir "../dependencies/assimp" /S /Q
git clone https://github.com/assimp/assimp.git "../dependencies/assimp"
if %ERRORLEVEL% GEQ 1 goto :ERROR
cd ../dependencies/assimp
if %ERRORLEVEL% GEQ 1 goto :ERROR
cmake CMakeLists.txt -G "Visual Studio 16 2019" -S . -B ./BINARIES/Win32
if %ERRORLEVEL% GEQ 1 goto :ERROR
cmake --build ./BINARIES/Win32 --config release
if %ERRORLEVEL% GEQ 1 goto :ERROR
cd /d %GEOM_COMPILER_PATH%
if %ERRORLEVEL% GEQ 1 goto :ERROR

rem ------------------------------------------------------------
rem MESHOPTIMIZER
rem ------------------------------------------------------------
:MESHOPTIMIZER
rmdir "../dependencies/meshoptimizer" /S /Q
git clone https://github.com/zeux/meshoptimizer.git "../dependencies/meshoptimizer"
if %ERRORLEVEL% GEQ 1 goto :ERROR
cd /d %GEOM_COMPILER_PATH%
if %ERRORLEVEL% GEQ 1 goto :ERROR

rem ------------------------------------------------------------
rem XGPU
rem ------------------------------------------------------------
:XGPU
rmdir "../dependencies/xgpu" /S /Q
git clone https://github.com/LIONant-depot/xGPU.git "../dependencies/xgpu"
if %ERRORLEVEL% GEQ 1 goto :ERROR
cd ../dependencies/xgpu/Build
if %ERRORLEVEL% GEQ 1 goto :ERROR
call updateExamplesDependencies.bat "return"
if %ERRORLEVEL% GEQ 1 goto :ERROR
cd /d %GEOM_COMPILER_PATH%
if %ERRORLEVEL% GEQ 1 goto :ERROR

rem ------------------------------------------------------------
rem XBMP_TOOLS
rem ------------------------------------------------------------
:XBMP_TOOLS
rmdir "../dependencies/xbmp_tools" /S /Q
git clone https://github.com/LIONant-depot/xbmp_tools.git "../dependencies/xbmp_tools"
if %ERRORLEVEL% GEQ 1 goto :ERROR
cd ../dependencies/xbmp_tools/build
if %ERRORLEVEL% GEQ 1 goto :ERROR
call UpdateDependencies.bat "return"
if %ERRORLEVEL% GEQ 1 goto :ERROR
cd /d %GEOM_COMPILER_PATH%
if %ERRORLEVEL% GEQ 1 goto :ERROR

:DONE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DONE!!
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
goto :PAUSE

:ERROR
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------
powershell write-host -fore Red DONE WITH ERRORS!!
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------

:PAUSE
rem if no one give us any parameters then we will pause it at the end, else we are assuming that another batch file called us
if %1.==. pause
