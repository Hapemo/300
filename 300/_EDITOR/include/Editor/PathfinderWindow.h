/**-------------------------------------------------------------------------------------/
 * @file
 *  PathfinderWindow.h
 * @author
 *  Jazz Teoh Yu Jue, j.teoh@digipen.edu
 * @date
 *  14/11/2023
 * @brief
 *	Pathfinder graph editing
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