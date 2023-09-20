#pragma once

class InputActionMap
{
public:
	InputActionMap() = default;
	void Enable();
	bool isEnabled();
	
private:
	std::string mActionMap_name;
	bool isEnable;
};