@echo OFF

set BATCHFILE_PATH = "%cd%"
set GEOMCOMPILER_BUILD_PATH = %cd%/x64/Debug
set GEOMCOMPILER_PROJ_FILE = %cd%/_GEOM_COMPILER/_GEOM_COMPILER.vcxproj

COLOR 8E
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore Cyan Hi, Im the Compiler.bat that will compile everything for you. Let me get to work!
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
echo.

:: //////////////////////////////////////////////////////////GEOM COMPILER////////////////////////////////////////////////////////////////
:Compile_Geom
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White COMPILING GEOM_COMPILER.EXE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
echo.

:: build the project using MSBuild
%GEOMCOMPILER_BUILD_PATH% %GEOMCOMPILER_PROJ_FILE% /p:Configuration=Debug /p:Platform=x64
if %errorlevel% equ 0 (
	echo Build Succeeded.
) else (
	echo Build Failed.
)

:: change the working directory to the Texture Compressor Filepath
cd %cd%/_GEOM_COMPILER
_GEOM_COMPILER.exe

if %ERRORLEVEL% GEQ 1 goto :PAUSE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White Done Compiling Geom
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

:: change the batchfile working directory back to the original filepath
cd %BATCHFILE_PATH%
if %ERRORLEVEL% GEQ 1 goto :PAUSE
:: //////////////////////////////////////////////////////////GEOM COMPILER////////////////////////////////////////////////////////////////


:: //////////////////////////////////////////////////////////TEXTURE COMPILER////////////////////////////////////////////////////////////////
:Texture_Compressor
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White COMPILING TEXTURE_COMPRESSOR.EXE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
echo.

:: change the working directory to the Texture Compressor Filepath
cd ../_TEXTURE_COMPRESSOR
_TEXTURE_COMPRESSOR.exe

if %ERRORLEVEL% GEQ 1 goto :PAUSE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White Done Compiling Textures
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
:: //////////////////////////////////////////////////////////TEXTURE COMPILER////////////////////////////////////////////////////////////////

:: finished the batch file.
exit	

:ERROR
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------
powershell write-host -fore Red ERROR OCCURED
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------