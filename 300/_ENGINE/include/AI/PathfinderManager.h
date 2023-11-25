#pragma once
#include "pch.h"
#include "glm/glm.hpp"
#include "Graph.h"
#include "ECS/ECS.h"
// Pathfind manager will be in charge of editing of pathfinding graph and saving of it.
// It has to load in the pathfind graph every start of gamestate. 
// 
// Add in pathfind string in AISetting
// 
// When loading gamestate, look through enemies and load in the ALGraph needed for each entities. (Don't load if already loaded in)
// 
// 
// Good to have - AISetting keep the string of the file name. After loading in the file, save pointer to ALGraph in PathfinderManager to AISetting component of AI
// 

#define POINTNAME "GraphPoint"
#define EDGENAME "GraphEdge"

void TestPathfinderManager();

struct AStarSetting {
	//AStarSetting() : elevation(), ignoreTags() {}
	float elevation;								// acceptable range of elevation between start or end node with other node
	std::vector<std::string> ignoreTags;	// Tags of entity to ignore the line of sight test
};

class PathfinderManager {
public:
	PathfinderManager();
	//------------------
	// Game Mode
	//------------------ 
	void Init();

	// Looks through the asset folder and load all the graph data
	void LoadAllGraphData();

	void LoadGraphData(std::filesystem::path const& _path);
	// Load Graph Data file in, convert it into ALGraph and return a shared pointer to it back to the entity.
	//ALGraph* LoadGraphData(std::string _filename);

	//Pathfinder functionalities. Pathfinding from one entity to another
	std::vector<glm::vec3> AStarPath(Entity _start, Entity _end, AStarSetting const& aStarSetting);

	// Checks for entities inbetween 2 entities
	bool CheckEntitiesInbetween(glm::vec3 const& _p0, glm::vec3 const& _p1, std::vector<Entity> _toIgnoreEntities, std::vector<std::string> _toIgnoreTags);

	// Get collider Position
	glm::vec3 GetColliderPos(Entity _e);
private:
	// Resetting all the nodes and connecting the start and end node to the alGraph
	void InitAStar(Entity _start, Entity _end, ALGraph& alGraph, AStarSetting const& aStarSetting);
	// Removing the last 2 nodes, the start and end points
	void EndAStar(Entity _start, Entity _end, ALGraph& alGraph);

	// Connect the src node with all other visible nodes in alGraph
	void ConnectVisibleNodes(Entity src_e, ALGraph::AdjList& src, ALGraph& alGraph, AStarSetting const& aStarSetting);
	// This version will take into account entity width, making sure most part of the entity can see the target
	bool CheckEntitiesInbetween(glm::vec3 const& _p0, glm::vec3 const& _p1, std::vector<Entity> _toIgnoreEntities, std::vector<std::string> _toIgnoreTags, glm::vec3 size);

	// Rubberband remove unnecessary nodes
	std::vector<glm::vec3> RubberbandPath(std::vector<glm::vec3> const& path, std::vector<Entity> toIgnoreEntities, std::vector<std::string> const& toIgnoreTags);

public:


	// AIManager can handle the pathfinding part by calling AISetting.ALGraphsharedpointer.AStarpathfind(), smth like tat

	//------------------
	// Editor Mode
	//------------------ 
	
	// Draws the pathfinder continuously 
	//void Update();

	// Editing entity
	// --------------
	// Update the ALGraph
	// Check the name of graphdata and make entity's AISetting component point to the respective ALGraph
	// If doesn't exist, attempt to load graph data
	// If unable to load, print warning message
	void ReloadALGraph(std::string const& _graphDataName);


	// Editing graph
	// -------------
	// In editor mode, only one graph can be active at a time
	GraphData* GetActiveGraphData();
	// Change active graph data. Will need to unload all entities in current graph data, and load in entities from new graph data
	void SetActiveGraphData(std::string const& _graphDataName);
	// Make new graph data
	void NewGraphData(std::string const& _filename);
	// Draws graph data
	//void DrawActiveGraphData();
	// Save graph data into another file
	void SaveActiveGraphData();
	// Delete the active graph data in system and in folder. Will need to loop through all the loaded entities in gamestate and ensure their AISetting graph data are pointed at nullptr
	void DeleteActiveGraphData();
	// Toggle graph drawing
	void ToggleDrawGraph();


	// Editing active graph
	// -------------
	void AddPoint(glm::vec3);
	void AddDirectedEdge(Entity _src, Entity _dst);
	void AddUndirectedEdge(Entity _e0, Entity _e1);
	//void DeletePoint(Entity _e);
	//void DeleteEdge(Entity _e0, Entity _e1);
	std::vector<glm::vec3> GetEdgeEnds(Entity _e);
	// Updates the active graph data with the current waypoint entities
	void UpdateGraphData();

//private:
	// Editing waypoint entities
	//--------------------------
	// Add a bunch of entities using active graph data
	void AddGraphEntities();
	// Remove all entities from current active graph data
	void DeleteGraphEntities();
	// Delete Entity, point or edge, changing the graph data.
	void DeleteEntity(Entity e);
	// Get the most updated entity version of the active graph data
	void RefreshGraphEntity();


	// Variables that only needed in editor mode
	int mActiveGraph;
	bool mDrawGraph;															// Toggle to draw the debug lines for active graph
	std::vector<std::string> mGraphDataNameList;
	std::vector<GraphData> mGraphDataList;				// TODO, not needed during game mode, consider removing it during publication
	std::vector<std::pair<Entity, std::vector<Entity>>> mGraphDataEntities;	// Contains all the entities that represent the point and edges of the active graph data. If it's point, vector will be empty
private:

	// Solution to graph data not updated when entities are added is, make a function that transforms mGraphDataEntities to graph data
	// When to transform?
	// 1. When saving graph data
	// 2. When changing graph data
	// 3. When updating ALGraph
	//------------------------------------------

private:
	std::vector<std::shared_ptr<ALGraph>> mALGraphList;

private: // Helper function
	std::pair<Entity, std::vector<Entity>>* FindGraphEntity(Entity _e);
	glm::vec3 RotationalVectorToEulerAngle(glm::vec3 direction);

};











