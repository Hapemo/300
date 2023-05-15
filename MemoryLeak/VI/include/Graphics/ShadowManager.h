/*!*****************************************************************************
\file ShadowManager.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains a system to create shadows. Currently, it can only cast 
rectangular shadows.
*******************************************************************************/
#pragma once
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"
#include <RenderManager.h>

/*!*****************************************************************************
\brief
Function for comparing angles between 2 vectors.

\param const Math::Vec2
The first vector.

\param const Math::Vec2
The second vector.

\return
true if the angle of endPt1 is less than endPt2.
*******************************************************************************/
bool CompareAngle(const Math::Vec2& endPt1, const Math::Vec2& endPt2);

/*!*****************************************************************************
\brief
Returns the angle of a vector.

\param Math::Vec2
The vector to be checked.

\return
The angle of the vector.
*******************************************************************************/
float GetAngle(const Math::Vec2& p0);

/*!*****************************************************************************
\brief
Edge struct that encapsulates position and direction.
*******************************************************************************/
struct Edge
{
	Math::Vec2 pos;
	Math::Vec2 dir;
};

/*!*****************************************************************************
\brief
ShadowManager Class that handles raycasting from entities with LightSource 
componenet onto entities with ShadowCaster component.
*******************************************************************************/
class ShadowManager : public System
{
public:
	/*!*****************************************************************************
	\brief
	Default constructor for ShadowManager.
	*******************************************************************************/
	ShadowManager();
	/*!*****************************************************************************
	\brief
	Updates the ShadowManager.
	*******************************************************************************/
	void Update();

	/*!*****************************************************************************
	\brief
	Returns the position of the lightsoruce.
	\return
	The position of the lightsource.
	*******************************************************************************/
	Math::Vec2 GetLightPos();
	/*!*****************************************************************************
	\brief
	Returns true if shadows are to be casted, false otherwise.
	\return
	Returns true if shadows are to be casted, false otherwise.
	*******************************************************************************/
	bool CastShadows() { return mCastShadows; }
	/*!*****************************************************************************
	\brief
	Returns the endpoints of the rays.
	\return
	Returns the endpoints of the rays.
	*******************************************************************************/
	const std::vector <Math::Vec2>& GetRayEndPoints() { return mRayEndPoints; }
private:
	Entity mLightsource;
	bool mCastShadows;
	std::vector <Edge> mObjectEdges;
	std::vector <Edge> mRayDirection;
	std::vector <Edge> mExtendedRayDirection;
	std::vector <Math::Vec2> mRayEndPoints;
	Transform mCamera;
	/*!*****************************************************************************
	\brief
	Raycasting to determine ray endpoints.
	*******************************************************************************/
	void RayCast();
	/*!*****************************************************************************
	\brief
	Create rays based on the object vertices.
	*******************************************************************************/
	void CreateRays();
	/*!*****************************************************************************
	\brief
	Clear vectors.
	*******************************************************************************/
	void ClearVectors();
	/*!*****************************************************************************
	\brief
	Create vertices for the field of view (lightsource radius)
	*******************************************************************************/
	void CreateFOVVertices();
	/*!*****************************************************************************
	\brief
	Create vertices for the field of view (lightsource radius)
	*******************************************************************************/
	void CreateObjectVertices(Entity e);
};