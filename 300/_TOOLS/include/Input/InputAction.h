#pragma once

class InputAction
{
public:
	InputAction() = default;
	void Enable();
	bool isEnabled();
private:
	std::string mAction_name;
	bool isEnable;
};