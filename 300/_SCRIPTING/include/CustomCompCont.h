#pragma once
#include "ECS/ECS_Components.h"

class CustomCompCont
{
private:
	using CompContType = decltype(systemManager->ecs->registry.view<General>());
	CompContType container;

public:
	//void PrintCompType();
	using it = decltype(container)::iterator;

	it begin()
	{
		return it(container.begin());
	}
	it end()
	{
		return it(container.end());
	}
	size_t size()
	{
		return container.size();
	}
};
