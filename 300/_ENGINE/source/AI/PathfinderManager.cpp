#include "pch.h"
#include "AI/PathfinderManager.h"
#include "ConfigManager.h"
#include "Graph.h"
#include "ECS/ECS_Components.h"

#define POINTNAME "GraphPoint"
#define EDGENAME "GraphEdge"


void TestPathfinderManager() {
	//systemManager->mPathfinderSystem.get()->ReloadALGraph("graphData2");

	if (Input::CheckKey(PRESS, _1)) {
		std::vector<glm::vec3> points{ { 10, 0, 0 }, {50, 50, 50},{300, 50, 300} ,{10, 10, 0} ,{4000, 500, 0} ,{-300, -750, 30} };

		for (int i{}; i < points.size(); ++i) {
			systemManager->mPathfinderSystem.get()->AddPoint(points[i]);
			for (int j{}; j < points.size(); ++j) {
				if (i == j) continue;
				systemManager->mPathfinderSystem.get()->AddDirectedEdge(points[i], points[j]);
			}
		}
	}

	if (Input::CheckKey(PRESS, _2))
		systemManager->mPathfinderSystem.get()->DeleteGraphEntities();
}

PathfinderManager::PathfinderManager() : mActiveGraph(-1), mDrawGraph(true), mGraphDataNameList(), mGraphDataList(), mGraphDataEntities(), mALGraphList() {}

//------------------
// Game Mode
//------------------ 
void PathfinderManager::Init() {
	LoadAllGraphData();
}

void PathfinderManager::LoadAllGraphData() {
	std::filesystem::path folder{ ConfigManager::GetValue("GraphDataPath") };

	for (std::filesystem::path const& entry : std::filesystem::directory_iterator(folder)) {
		LoadGraphData(entry);
	}
}

void PathfinderManager::LoadGraphData(std::filesystem::path const& _path) {
	// Get name
	std::string graphDataName(_path.filename().string());
	graphDataName = graphDataName.substr(0, graphDataName.length() - 4); // Get the ".txt" out
	mGraphDataNameList.push_back(graphDataName);
	
	// Get graph data
	mGraphDataList.emplace_back(GraphData(_path.string()));

	// Get ALGraph
	mALGraphList.emplace_back(mGraphDataList.back().MakeALGraph());
}



//------------------
// Editor Mode
//------------------ 
void PathfinderManager::ReloadALGraph(std::string const& _graphDataName) {
	for (int i{}; i < mGraphDataNameList.size(); ++i) {
		if (mGraphDataNameList[i] != _graphDataName) continue;
		mALGraphList[i] = mGraphDataList[i].MakeALGraph();
		return;
	}
	PWARNING("Unable to find %s to update it's ALGraph", _graphDataName);
}

GraphData* PathfinderManager::GetActiveGraphData() {
	if (mActiveGraph == -1) return nullptr;
	return &mGraphDataList[mActiveGraph];
}

void PathfinderManager::SetActiveGraphData(std::string const& _graphDataName) {
	DeleteGraphEntities();

	for (int i{}; i < mGraphDataNameList.size(); ++i) {
		if (mGraphDataNameList[i] != _graphDataName) continue;
		mActiveGraph = i;
		AddGraphEntities();
		break;
	}

	mActiveGraph = -1;
	PWARNING("Unable to find %s to change active graph data to it, setting graph data to none", _graphDataName);
}

void PathfinderManager::NewGraphData(std::string const& _filename) {
	mGraphDataNameList.push_back(_filename);
	mGraphDataList.push_back(GraphData());
	mALGraphList.push_back(mGraphDataList.back().MakeALGraph());
}

void PathfinderManager::SaveActiveGraphData() {
	if (mActiveGraph == -1) {
		PWARNING("Unable to save invalid graph data, active graph data is -1");
		return;
	}

	std::string filePath = ConfigManager::GetValue("GraphDataPath") + mGraphDataNameList[mActiveGraph] + ".txt";
	mGraphDataList[mActiveGraph].SaveGraph(filePath);
}

void PathfinderManager::DeleteActiveGraphData() {
	std::string filePath = ConfigManager::GetValue("GraphDataPath") + mGraphDataNameList[mActiveGraph] + ".txt";
	remove(filePath.c_str());
	
	mGraphDataNameList.erase(mGraphDataNameList.begin() + mActiveGraph);
	mGraphDataList.erase(mGraphDataList.begin() + mActiveGraph);
	mALGraphList.erase(mALGraphList.begin() + mActiveGraph);

	DeleteGraphEntities();
	mActiveGraph = -1;
}

void PathfinderManager::ToggleDrawGraph() {
	mDrawGraph = !mDrawGraph;

	for (auto& [e, positions] : mGraphDataEntities)
		Entity(e).GetComponent<General>().isActive = mDrawGraph;
}



// Editing waypoint entities
//--------------------------
// Add a bunch of entities from active graph data
void PathfinderManager::AddGraphEntities() {
	GraphData& currGD{ mGraphDataList[mActiveGraph] };
	for (auto& [point, edges] : currGD.mData) {
	// Add points
		AddPoint(point);
	// Add directed edges
		for (glm::vec3 const& edge : edges) {
			AddDirectedEdge(point, edge);
		}
	}
}

// Remove current active waypoint entities
void PathfinderManager::DeleteGraphEntities() {
	for (auto& [e, positions] : mGraphDataEntities)
		systemManager->ecs->registry.destroy(e.id);
	mGraphDataEntities.clear();
}

void PathfinderManager::DeleteEntity(Entity e) {
	// Delete from graph data
	std::vector<glm::vec3> const& endpoints{ FindGraphEntity(e)->second };
	if (endpoints.size() == 1)			// Deleting point from graph
		mGraphDataList[mActiveGraph].DeletePoint(endpoints.front());
	else if (endpoints.size() == 2) // Deleting edge from graph
		mGraphDataList[mActiveGraph].DeleteDEdge(endpoints.front(), endpoints.back());

	// Delete from ecs container
	auto it = std::find_if(mGraphDataEntities.begin(), mGraphDataEntities.end(), [e] (std::pair<Entity, std::vector<glm::vec3>> entityData) { return entityData.first == e; });
	mGraphDataEntities.erase(it);

	// Delete from ecs
	systemManager->ecs->registry.destroy(e.id);
}



// Editing active graph
// -------------
void PathfinderManager::AddPoint(glm::vec3 pos) {
	// Add graph point entity
	Entity e = systemManager->ecs->NewEntityPrefabForGraph(POINTNAME, pos);

	// Add the entity in to pathfinder manager's entity list
	mGraphDataEntities.push_back({ e, {pos} });
}

void PathfinderManager::AddDirectedEdge(glm::vec3 start, glm::vec3 end) {
	glm::vec3 direction = end - start;
	Entity e = systemManager->ecs->NewEntityPrefabForGraph(EDGENAME, start + (direction)/2.f - direction*(0.15f)); // Make arrow entity at mid point
	
	Transform& eTrans = e.GetComponent<Transform>();

	eTrans.mScale.z = glm::length(end - start)-5.f;
	eTrans.mRotate = RotationalVectorToEulerAngle(glm::normalize(end - start)) * 180.f / 3.1415925359f;
	eTrans.mRotate.y += 180;

	mGraphDataEntities.push_back({ e, {start, end} });
}

void PathfinderManager::AddDirectedEdge(Entity _src, Entity _dst) {
	AddDirectedEdge(_src.GetComponent<Transform>().mTranslate, _dst.GetComponent<Transform>().mTranslate);
}

void PathfinderManager::AddUndirectedEdge(Entity _e0, Entity _e1) {
	AddDirectedEdge(_e0, _e1);
	AddDirectedEdge(_e1, _e0);
}
//
//void PathfinderManager::DeletePoint(Entity _e) {
//	// Delete from graph data
//	mGraphDataList[mActiveGraph].DeletePoint(_e.GetComponent<Transform>().mTranslate);
//
//	// Delete entity
//	DeleteEntity(_e);
//}
//
//void PathfinderManager::DeleteEdge(Entity _e0, Entity _e1) {
//
//}



// Helper function
std::pair<Entity, std::vector<glm::vec3>>* PathfinderManager::FindGraphEntity(Entity _e) {
	return &*std::find_if(mGraphDataEntities.begin(), mGraphDataEntities.end(), [_e] (std::pair<Entity, std::vector<glm::vec3>> entityData) { return entityData.first == _e; });
}

glm::vec3 PathfinderManager::RotationalVectorToEulerAngle(glm::vec3 direction) {
	glm::vec3 angles;

	angles.x = asin(direction.y);
	angles.y = glm::atan(direction.x, direction.z); 
	angles.z = 0.0f; // Assuming no roll

	return angles;
}

