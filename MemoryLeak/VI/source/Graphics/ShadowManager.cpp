/*!*****************************************************************************
\file ShadowManager.cpp
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-10-2022
\brief
This file contains a system to create shadows. Currently, it can only cast
rectangular shadows.
*******************************************************************************/
#include <ShadowManager.h>
#include <Logger.h>
#include <ECSManager.h>
#include <GameStateManager.h>
#include <RenderProps.h>

/*!*****************************************************************************
\brief
Default constructor for ShadowManager.
*******************************************************************************/
ShadowManager::ShadowManager() : mLightsource(0), mCastShadows(false) {}
/*!*****************************************************************************
\brief
Updates the ShadowManager.
*******************************************************************************/
void ShadowManager::Update()
{
	ClearVectors();

	mLightsource = renderManager->GetLightSource();

	if (!mLightsource.id)
	{
		mCastShadows = false;
		return;
	}
	if (!mLightsource.HasComponent<LightSource>()) return;

	mCastShadows = true;

	CreateFOVVertices();

	for (Entity e : mEntities)
	{
		if (!e.ShouldRun()) continue;
		if (!e.HasComponent<ShadowCaster>()) continue;
		CreateObjectVertices(e);
	}
	CreateRays();
	RayCast();
}
/*!*****************************************************************************
\brief
Raycasting to determine ray endpoints.
*******************************************************************************/
void ShadowManager::RayCast()
{
	Math::Vec2 lightPos = GetLightPos();

	float T1, T2;
	float smallestT1{};
	bool foundIntersection;

	for (const Edge& ray : mRayDirection)
	{
		smallestT1 = 1.f;
		foundIntersection = false;
		for (const Edge& segment : mObjectEdges)
		{
			T2 = (ray.dir.x * (segment.pos.y - ray.pos.y) + ray.dir.y * (ray.pos.x - segment.pos.x)) 
				/ (segment.dir.x * ray.dir.y - segment.dir.y * ray.dir.x);
			T1 = (segment.pos.x + segment.dir.x * T2 - ray.pos.x) / ray.dir.x;

			if (!(T1 > 0 && T2 > 0 && T2 < 1)) // no intersection
				continue;
			if (T1 >= smallestT1 - 0.0001f) // intersection is further than the closest
				continue;
			foundIntersection = true;
			smallestT1 = T1;
		}
		Math::Vec2 _ray = ray.pos + smallestT1 * ray.dir;
		mRayEndPoints.push_back(_ray);

		if (!foundIntersection) // intersection is at a point
		{
			float theta = 0.001f;
			Math::Vec2 posDir{ ray.dir.x * cosf(theta) - ray.dir.y * sinf(theta), ray.dir.x * sinf(theta) + ray.dir.y * cosf(theta) };
			Math::Vec2 negDir{ ray.dir.x * cosf(-theta) - ray.dir.y * sinf(-theta), ray.dir.x * sinf(-theta) + ray.dir.y * cosf(-theta) };
			mExtendedRayDirection.push_back({ ray.pos, posDir });
			mExtendedRayDirection.push_back({ ray.pos, negDir });
		}
	}

	for (const Edge& ray : mExtendedRayDirection)
	{
		smallestT1 = std::numeric_limits<float>::max();
		for (const Edge& segment : mObjectEdges)
		{
			T2 = (ray.dir.x * (segment.pos.y - ray.pos.y) + ray.dir.y * (ray.pos.x - segment.pos.x))
				/ (segment.dir.x * ray.dir.y - segment.dir.y * ray.dir.x);
			T1 = (segment.pos.x + segment.dir.x * T2 - ray.pos.x) / ray.dir.x;

			if (!(T1 > 0 && T2 > 0 && T2 < 1)) // no intersection
				continue;
			if (T1 >= smallestT1 - 0.0001f) // intersection is further than the closest
				continue;
			smallestT1 = T1;
		}
		Math::Vec2 _ray = ray.pos + smallestT1 * ray.dir;
		mRayEndPoints.push_back(_ray);
	}
	std::sort(mRayEndPoints.begin(), mRayEndPoints.end(), CompareAngle);
}
/*!*****************************************************************************
\brief
Create rays based on the object vertices.
*******************************************************************************/
void ShadowManager::CreateRays()
{
	Math::Vec2 lightPos = GetLightPos();
	for (size_t k = 0; k < mObjectEdges.size(); ++k)
		mRayDirection.push_back({ lightPos, mObjectEdges[k].pos - lightPos });
}
/*!*****************************************************************************
\brief
Returns the position of the lightsoruce.
\return
The position of the lightsource.
*******************************************************************************/
Math::Vec2 ShadowManager::GetLightPos()
{
	if (!mLightsource.id) return Math::Vec2();
	return mLightsource.GetComponent<Transform>().translation
		+ mLightsource.GetComponent<LightSource>().centerOffset;;
}
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
bool CompareAngle(const Math::Vec2& endPt1, const Math::Vec2& endPt2)
{
	return GetAngle(endPt1 - shadowManager->GetLightPos()) < GetAngle(endPt2 - shadowManager->GetLightPos());
}
/*!*****************************************************************************
\brief
Returns the angle of a vector.

\param Math::Vec2
The vector to be checked.

\return
The angle of the vector.
*******************************************************************************/
float GetAngle(const Math::Vec2& endPt)
{
	if (endPt.y != 0.f && endPt.x >= 0.f)
	{
		float angle = atan2f(endPt.y, endPt.x);
		if (angle <= 0)
			angle = (float)Math::PI - abs(angle) + (float)Math::PI;
		return  angle;
	}
	if (endPt.y == 0.f && endPt.x > 0.f)
		return 0.f;
	if (endPt.y != 0.f && endPt.x < 0.f)
	{
		float angle = atan2f(endPt.y, endPt.x);
		if (angle <= 0)
			angle = (float)Math::PI - abs(angle) + (float)Math::PI;
		return  angle;
	}
	return (float)Math::PI;
}
/*!*****************************************************************************
\brief
Create vertices for the field of view (lightsource radius)
*******************************************************************************/
void ShadowManager::CreateObjectVertices(Entity e)
{
	std::vector<Math::Vec2> objV = e.GetComponent<ShadowCaster>().centerOffset;
	Math::Vec2 pos = e.GetComponent<Transform>().translation;

	float radius = mLightsource.GetComponent<LightSource>().radius;
	Math::Vec2 lightPos = GetLightPos();

	if (e.HasComponent<Sprite>())
	{
		Transform xform = e.GetComponent<Transform>();
		if (e.HasComponent<RectCollider>())
			xform.translation += e.GetComponent<RectCollider>().centerOffset;
		if (powf(xform.translation.x - lightPos.x, 2.f) + powf(xform.translation.y - lightPos.y, 2.f)
		> powf(mLightsource.GetComponent<LightSource>().radius, 2.f))
			e.GetComponent<Sprite>().color = { 80, 80, 80, e.GetComponent<Sprite>().color.a };
		else if (powf(xform.translation.x - lightPos.x, 2.f) + powf(xform.translation.y - lightPos.y, 2.f)
			< powf(mLightsource.GetComponent<LightSource>().radius * 0.5f, 2.f))
			e.GetComponent<Sprite>().color = { 255, 255, 255, e.GetComponent<Sprite>().color.a };
		else
		{
			int color = static_cast<int>(255 - 175 * (Math::Distance(xform.translation, lightPos) - 0.5f * radius) / (radius * 0.5f));
			if (color < 80) color = 80;
			e.GetComponent<Sprite>().color = { (unsigned char)color, (unsigned char)color, (unsigned char)color, e.GetComponent<Sprite>().color.a };
		}
	}

	if (objV.size() < 2) return;

	for (size_t i = 0; i < objV.size() - 1; ++i)
	{
		Math::Vec2 curr = objV[i] + pos;
		Math::Vec2 next = objV[i + 1] + pos;
		if (powf(curr.x - lightPos.x, 2.f) + powf(curr.y - lightPos.y, 2.f)
			< powf(radius, 2.f) ||
			powf(next.x - lightPos.x, 2.f) + powf(next.y - lightPos.y, 2.f)
			< powf(radius, 2.f))
			mObjectEdges.push_back({ curr, next - curr });
	}
}


/*!*****************************************************************************
\brief
Clear vectors.
*******************************************************************************/
void ShadowManager::ClearVectors()
{
	mObjectEdges.clear();
	mRayDirection.clear();
	mExtendedRayDirection.clear();
	mRayEndPoints.clear();
}
/*!*****************************************************************************
\brief
Create vertices for the field of view (lightsource radius)
*******************************************************************************/
void ShadowManager::CreateFOVVertices()
{
	std::vector<Math::Vec2> circlePts;
	Math::Vec2 center = GetLightPos();
	Math::Vec2 vec = Math::Vec2(mLightsource.GetComponent<LightSource>().radius, 0.f);
	float theta = 2.f / CIRCLE_SLICES * static_cast<float>(Math::PI);


	for (int i = 0; i < CIRCLE_SLICES; ++i)
		circlePts.push_back(center + Math::Vec2(cosf(theta * i) * vec.x, sinf(theta * i) * vec.x));
	
	for (int i = 0; i < CIRCLE_SLICES - 1; ++i)
		mObjectEdges.push_back({ circlePts[i], circlePts[i + 1] - circlePts[i] });
	mObjectEdges.push_back({ circlePts.back(), circlePts[0] - circlePts.back() });
}
