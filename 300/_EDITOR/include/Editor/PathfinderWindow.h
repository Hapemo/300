/**-------------------------------------------------------------------------------------/
 * @file
 *  PrefabWindow.h
 * @author
 *  Han Cheng Wen, c,han@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *	Prefab Scene declaration for editing prefabs
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/
#pragma once
#include "EditorWindow.h"
#include "ECS/ECS.h"


class PathfinderWindow : public EditorWindow
{
public:
	/*!*****************************************************************************
	* /brief
		Initialize funtion for imguiWindow
	*******************************************************************************/
	void init();
	/*!*****************************************************************************
	* /brief
		Update funtion for imguiWindow
	*******************************************************************************/
	void update();


	static bool openWindow;
	Entity mLatestPoint;
	Entity mLatestEdge;
	std::queue<Entity> mEdgePoints; // Should have maximum 2 at a time
	//std::array<Entity, 2> mEdgeHistory;
	//std::queue<Entity, std::list<Entity>> mSelectedEntityHistory;
};