#ifndef BUTTON_SYSTEM_H
#define BUTTON_SYSTEM_H

#include <pch.h>
#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>

class ButtonSystem
{
public:
	
	void Init();
	void Update();
	void Reset();

	float imguiButtonX{};
	float imguiButtonY{};

private:
	GFX::Window* window;

	bool CheckHover(Entity e, vec2 mousePos);
	bool CheckClick(Entity e, vec2 mousePos);
	bool CheckActivate(Entity e, vec2 mousePos);
};

#endif // !BUTTON_SYSTEM_H
