#pragma once
#include <pch.h>
class ScriptingSystem;

class SystemManager
{
public:
	std::unique_ptr<ScriptingSystem> scriptingSystem;
	SystemManager();
	~SystemManager() = default;
	void Init();
	void Update(float dt);
	void Exit();
};

extern SystemManager* systemManager;