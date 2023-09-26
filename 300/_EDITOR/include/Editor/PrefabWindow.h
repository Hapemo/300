#pragma once
#include "EditorWindow.h"
#include "ECS.h"


class PrefabWindow : public EditorWindow
{
public:

	void init();

	void update();



	static entt::entity prefabObj;
};