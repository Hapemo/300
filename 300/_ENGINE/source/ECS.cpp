#include "ECS.h"

Entity ECS::NewEntity()
{
	return registry.create();
}