@echo OFF

set BATCHFILE_PATH="%cd%"

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

:: change the working directory to the geomcompiler DEBUG filepath
cd %cd%/x64/Debug		
_GEOM_COMPILER.exe

if %ERRORLEVEL% GEQ 1 goto :PAUSE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White Done Compiling Geom
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

:: change the batchfile working directory back to the original filepath
cd /d %BATCHFILE_PATH%	
if %ERRORLEVEL% GEQ 1 goto :PAUSE
:: //////////////////////////////////////////////////////////GEOM COMPILER////////////////////////////////////////////////////////////////

:: finished the batch file.
exit	

:ERROR
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------
powershell write-host -fore Red ERROR OCCURED
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------