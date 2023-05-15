/*!*****************************************************************************
\file MonoManager.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 19-10-2022
\brief
This file contains the function declarations of the class MonoManager.
The MonoManager class handles the C# scripting for the engine.
*******************************************************************************/

#pragma once
#include <mono/jit/jit.h>
#include "mono/metadata/object.h"
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include "Singleton.h"
#include "ScriptComponent.h"
//#include "MonoComponent.h"
#include "MonoMethods.h"

class MonoManager : public Singleton<MonoManager>
{
public:
	/*!*****************************************************************************
	\brief
	Default constructor and destructor.
	*******************************************************************************/
	MonoManager() = default;
	~MonoManager() = default;

	/*!*****************************************************************************
	\brief
	Delete copy constructor.
	*******************************************************************************/
	MonoManager(const MonoManager&) = delete;
	const MonoManager& operator=(const MonoManager&) = delete;

	/*!*****************************************************************************
	\brief
	Initialise Mono.
	*******************************************************************************/
	void InitMono();

	/*!*****************************************************************************
	\brief
	Close Mono and free & unload the domain.
	*******************************************************************************/
	void CloseMono();

	/*!*****************************************************************************
	\brief
	Set and return the mono directory path according to the projec configuration.
	*******************************************************************************/
	std::string MonoDirectoryPath();

	/*!*****************************************************************************
	\brief
	Initialise the Mono root domain, create the app domain, and load the mono
	assembly dll. Returns true if success, else returns false.
	*******************************************************************************/
	bool InitMonoDomain(const char* _root, const char* _appdomain, const std::string _directory, const std::string _dll);

	/*!*****************************************************************************
	\brief
	Retrieves the mono class from a mono image. Returns the mono class pointer if
	success, else returns a nullptr.
	*******************************************************************************/
	MonoClass* GetClassInAssembly(MonoAssembly* _assembly, const char* _namespace, const char* _class);

	/*!*****************************************************************************
	\brief
	Loads the mono class and allocate a class instance for it in the app domain.
	Returns the class instance if success, else returns a nullptr.
	*******************************************************************************/
	MonoObject* InstantiateClass(const char* _namespace, const char* _class);

	/*!*****************************************************************************
	\brief
	Calls a mono method by script name and function name.
	*******************************************************************************/
	void CallMethod(std::string _scriptName, const char* _function, int _paramCount, void** _params);

	/*!*****************************************************************************
	\brief
	Register the C# scripts to store internally for the logic system to use.
	*******************************************************************************/
	void RegisterMonoScript(std::string _namespace, std::string _class);

	/*!*****************************************************************************
	\brief
	Set the mono script component in the member map mMonoComponents.
	*******************************************************************************/
	void SetMonoComponent(std::string _class, MonoObject* _monoObject);

	/*!*****************************************************************************
	\brief
	Get the mono script component from the member map mMonoComponents by class name.
	*******************************************************************************/
	MonoObject* GetMonoComponent(std::string _class);

private:
	// Mono generic stuff
	static MonoDomain* mAppDomain;
	static MonoDomain* mRootDomain;
	static MonoAssembly* mAssembly;
	static std::vector<uint32_t> mMonoHandlers;

	// Storing all mono scripts
	std::map<std::string, MonoObject*> mMonoComponents;
};