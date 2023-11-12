#include "pch.h"
#include "AI/PathfinderManager.h"
#include "ConfigManager.h"
#include "Graph.h"
#include "ECS/ECS_Components.h"
#include "Physics/PhysicsSystem.h"

void TestPathfinderManager() {
	//systemManager->mPathfinderSystem.get()->ReloadALGraph("graphData2");

	//if (Input::CheckKey(PRESS, _1)) {
	//	std::vector<glm::vec3> points{ { 10, 0, 0 }, {50, 50, 50},{300, 50, 300} ,{10, 10, 0} ,{4000, 500, 0} ,{-300, -750, 30} };

	//	for (int i{}; i < points.size(); ++i) {
	//		systemManager->mPathfinderSystem.get()->AddPoint(points[i]);
	//		for (int j{}; j < points.size(); ++j) {
	//			if (i == j) continue;
	//			systemManager->mPathfinderSystem.get()->AddDirectedEdge(points[i], points[j]);
	//		}
	//	}
	//}

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
// AStar Functionalitiies
//------------------ 

std::vector<glm::vec3> PathfinderManager::AStarPath(Entity _start, Entity _end, AStarSetting const& aStarSetting) {
	using NODE_STATE = ALGraph::NODE_STATE;
	using AdjList = ALGraph::AdjList;
	using Edge = ALGraph::Edge;
	// Initialise containers (open list, close list)
// Put starting node on the open list
	
	// Find ALGraph to do pathfinding
	ALGraph* alGraph{ nullptr };
	for (int i{}; i < mGraphDataNameList.size(); ++i) {
		if (mGraphDataNameList[i] != _start.GetComponent<AISetting>().mGraphDataName) continue;
		alGraph = &mALGraphList[i];
		break;
	}
	if (!alGraph) {
		PWARNING("Unable to find %s to perform pathfinding", _start.GetComponent<AISetting>().mGraphDataName);
		return std::vector<glm::vec3>();
	}

	// AStar pathfinding portion
	InitAStar(_start, _end, *alGraph, aStarSetting);
	std::vector<glm::vec3> returnVal{}; //= alGraph->AStarPath();
	EndAStar(*alGraph);

	return returnVal;
}

void PathfinderManager::InitAStar(Entity _start, Entity _end, ALGraph& alGraph, AStarSetting const& aStarSetting) {
	// Add start and end nodes
	auto& mData = alGraph.mData;

	glm::vec3 const& start = _start.GetComponent<Transform>().mTranslate;
	glm::vec3 const& end = _end.GetComponent<Transform>().mTranslate;
	
	mData.push_back(decltype(alGraph.mData)::value_type());
	mData.back().point = start;
	mData.push_back(decltype(alGraph.mData)::value_type());
	mData.back().point = end;

	ConnectVisibleNodes(_start, *(mData.end()-2), alGraph, aStarSetting);
	ConnectVisibleNodes(_end, *(mData.end()-1), alGraph, aStarSetting);

	// Initialise HCost and reset all other values
	for (ALGraph::AdjList& node : mData) {
		node.hCost = ALGraph::CalcHCost(node.point, end);
		node.gCost = 0;
		node.state = ALGraph::NODE_STATE::NONE;
		node.parent = nullptr;
	}
}

void PathfinderManager::EndAStar(ALGraph& alGraph) {
	alGraph.mData.pop_back();
	alGraph.mData.pop_back();
}

// TODO DEBUG THIS
// The start and end point is unable to connect with any points for some reason. Check what the ray collided with and print the entity's name.
// 
void PathfinderManager::ConnectVisibleNodes(Entity src_e, ALGraph::AdjList& src, ALGraph& alGraph, AStarSetting const& aStarSetting) {
	for (ALGraph::AdjList& node : alGraph.mData) {
		// In each node of the graph, if elevation difference is too much, ignore.
		if ((abs(abs(src.point.y) - abs(node.point.y))) > aStarSetting.elevation) continue;
		// If it's the same node, ignore
		if (src.point == node.point) continue;

		// Compare the node and src for line of sight test and get a list of entity hit
		std::vector<Entity> entitiesHit = systemManager->GetPhysicsPointer()->Visible(src.point, node.point, FLT_MAX);

		// Remove the start entity
		entitiesHit.erase(std::find(entitiesHit.begin(), entitiesHit.end(), src_e));
		std::cout << "When doing visibility test between " << src.point << " and " << node.point << ", these are the entities hit\n";
		// Check the remaining tag of entities to see which ones you wanna ignore
		for (int i{}; i < entitiesHit.size(); ++i) {
			std::cout << "Entity " << static_cast<int>(entitiesHit[i].id) << ": " << entitiesHit[i].GetComponent<General>().name << '\n';
			static int counter{};
			entitiesHit[i].GetComponent<General>().name = entitiesHit[i].GetComponent<General>().name + std::to_string(++counter);
			for (std::string const& tag : aStarSetting.ignoreTags) {
				if (entitiesHit[i].GetComponent<General>().GetTag() == tag) {
					entitiesHit.erase(entitiesHit.begin() + i);
					printf("erased\n");
					--i;
				}
			}
		}
		std::cout << '\n';
		//for (auto it{ entitiesHit.begin() }; it != entitiesHit.end(); ++it) {
		//}

		// After removing those to ignore, if there's still some remains, ignore this point
		if (entitiesHit.size()) continue;

		// If it passes all the test, add both to each other neighbor, UEdge
		float dist = ALGraph::CalcHCost(src.point, node.point);
		src.edges.push_back(ALGraph::Edge(&node, dist));
		node.edges.push_back(ALGraph::Edge(&src, dist));
	}
}


//------------------
// Editor Mode
//------------------ 
void PathfinderManager::ReloadALGraph(std::string const& _graphDataName) {
	for (int i{}; i < mGraphDataNameList.size(); ++i) {
		if (mGraphDataNameList[i] != _graphDataName) continue;
		UpdateGraphData();
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
	UpdateGraphData();
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
	UpdateGraphData();
	ReloadALGraph(mGraphDataNameList[mActiveGraph]);

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
	// Add points
	for (auto& [point, edges] : currGD.mData) {
		Entity e = systemManager->ecs->NewEntityPrefabForGraph(POINTNAME, point);

		// Add the entity in to pathfinder manager's entity list
		mGraphDataEntities.push_back({ e, {} });
	}

	// Add directed edges
	for (auto& [point, edges] : currGD.mData) {
		Entity Start{};
		// Get start entity
		for (auto [_e, ends] : mGraphDataEntities) {
			if (_e.GetComponent<Transform>().mTranslate == point) {
				Start = _e;
				break;
			}
		}

		for (glm::vec3 const& edge : edges) {
			Entity End{};
			// Get end entity
			for (auto [_e, ends] : mGraphDataEntities) {
				if (_e.GetComponent<Transform>().mTranslate == edge) {
					End = _e;
					break;
				}
			}
			glm::vec3 start = point;
			glm::vec3 end = edge;

			glm::vec3 direction = end - start;
			Entity e = systemManager->ecs->NewEntityPrefabForGraph(EDGENAME, start + (direction)/2.f - direction*(0.15f)); // Make arrow entity at mid point

			Transform& eTrans = e.GetComponent<Transform>();

			eTrans.mScale.z = glm::length(direction)-5.f;
			eTrans.mRotate = RotationalVectorToEulerAngle(glm::normalize(direction)) * 180.f / 3.1415925359f;
			eTrans.mRotate.y += 180;

			mGraphDataEntities.push_back({ e, {Start, End} });
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
	std::vector<Entity> const& endpoints{ FindGraphEntity(e)->second };
	if (endpoints.size() == 1)			// Deleting point from graph
		mGraphDataList[mActiveGraph].DeletePoint(e.GetComponent<Transform>().mTranslate);
	else if (endpoints.size() == 2) // Deleting edge from graph
		mGraphDataList[mActiveGraph].DeleteDEdge(endpoints.front().GetComponent<Transform>().mTranslate, endpoints.back().GetComponent<Transform>().mTranslate);

	// Delete from ecs container
	auto it = std::find_if(mGraphDataEntities.begin(), mGraphDataEntities.end(), [e] (std::pair<Entity, std::vector<Entity>> entityData) { return entityData.first == e; });
	mGraphDataEntities.erase(it);
	// If Point, delete all edges connecting to it TODO

	// Delete from ecs
	systemManager->ecs->registry.destroy(e.id);
}

void PathfinderManager::RefreshGraphEntity() {
	DeleteGraphEntities();
	AddGraphEntities();
}


// Editing active graph
// -------------
void PathfinderManager::AddPoint(glm::vec3 pos) {
	// Add graph point entity
	Entity e = systemManager->ecs->NewEntityPrefabForGraph(POINTNAME, pos);

	// Add point to graph data
	mGraphDataList[mActiveGraph].AddPoint(pos);

	// Add the entity in to pathfinder manager's entity list
	mGraphDataEntities.push_back({ e, {} });
}

//void PathfinderManager::AddDirectedEdge(glm::vec3 start, glm::vec3 end) {
//	glm::vec3 direction = end - start;
//	Entity e = systemManager->ecs->NewEntityPrefabForGraph(EDGENAME, start + (direction)/2.f - direction*(0.15f)); // Make arrow entity at mid point
//	
//	Transform& eTrans = e.GetComponent<Transform>();
//
//	eTrans.mScale.z = glm::length(end - start)-5.f;
//	eTrans.mRotate = RotationalVectorToEulerAngle(glm::normalize(end - start)) * 180.f / 3.1415925359f;
//	eTrans.mRotate.y += 180;
//
//	mGraphDataEntities.push_back({ e, {start, end} });
//}

void PathfinderManager::AddDirectedEdge(Entity _src, Entity _dst) {
	//AddDirectedEdge(_src.GetComponent<Transform>().mTranslate, _dst.GetComponent<Transform>().mTranslate);
	// Adding entity
	glm::vec3 start = _src.GetComponent<Transform>().mTranslate;
	glm::vec3 end = _dst.GetComponent<Transform>().mTranslate;

	glm::vec3 direction = end - start;
	Entity e = systemManager->ecs->NewEntityPrefabForGraph(EDGENAME, start + (direction)/2.f - direction*(0.15f)); // Make arrow entity at mid point

	Transform& eTrans = e.GetComponent<Transform>();

	eTrans.mScale.z = glm::length(direction)-5.f;
	eTrans.mRotate = RotationalVectorToEulerAngle(glm::normalize(direction)) * 180.f / 3.1415925359f;
	eTrans.mRotate.y += 180;

	mGraphDataEntities.push_back({ e, {_src, _dst} });

	// Adding to graph data
	mGraphDataList[mActiveGraph].AddDEdge(start, end);
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

std::vector<glm::vec3> PathfinderManager::GetEdgeEnds(Entity _e) {
	auto const& ends = FindGraphEntity(_e)->second;
	return { ends.front().GetComponent<Transform>().mTranslate, ends.back().GetComponent<Transform>().mTranslate };
}

void PathfinderManager::UpdateGraphData() {
	if (mActiveGraph == -1) return;
	GraphData updatedGD{};

	// Add points
	for (auto& [_e, ends] : mGraphDataEntities) {
		if (!ends.size()) {
			updatedGD.AddPoint(_e.GetComponent<Transform>().mTranslate);
			std::cout << "Entity " << static_cast<int>(_e.id) << ": point: " << _e.GetComponent<Transform>().mTranslate << '\n';
		}
	}

	// Add edges
	for (auto& [_e, ends] : mGraphDataEntities) {
		if (ends.size()) {
			updatedGD.AddDEdge(ends.front().GetComponent<Transform>().mTranslate, ends.back().GetComponent<Transform>().mTranslate);
			std::cout << "Entity " << static_cast<int>(_e.id) << ": edge start: " << ends.front().GetComponent<Transform>().mTranslate << " edge end: " << ends.back().GetComponent<Transform>().mTranslate << '\n';
		}
	}

	mGraphDataList[mActiveGraph] = updatedGD;
}


// Helper function
std::pair<Entity, std::vector<Entity>>* PathfinderManager::FindGraphEntity(Entity _e) {
	return &*std::find_if(mGraphDataEntities.begin(), mGraphDataEntities.end(), [_e] (std::pair<Entity, std::vector<Entity>> entityData) { return entityData.first == _e; });
}

glm::vec3 PathfinderManager::RotationalVectorToEulerAngle(glm::vec3 direction) {
	glm::vec3 angles;

	angles.x = asin(direction.y);
	angles.y = glm::atan(direction.x, direction.z); 
	angles.z = 0.0f; // Assuming no roll

	return angles;
}

