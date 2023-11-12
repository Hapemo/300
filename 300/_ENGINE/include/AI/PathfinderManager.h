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

void TestPathfinderManager();

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
	// Delete the active graph data. Will need to loop through all the loaded entities in gamestate and ensure their AISetting graph data are pointed at nullptr
	void DeleteActiveGraphData();
	// Toggle graph drawing
	void ToggleDrawGraph();


	// Editing active graph
	// -------------
	void AddPoint(glm::vec3);
	void AddDirectedEdge(glm::vec3 start, glm::vec3 end);
	void AddDirectedEdge(Entity _src, Entity _dst);
	void AddUndirectedEdge(Entity _e0, Entity _e1);
	//void DeletePoint(Entity _e);
	//void DeleteEdge(Entity _e0, Entity _e1);

//private:
	// Editing waypoint entities
	//--------------------------
	// Add a bunch of entities using active graph data
	void AddGraphEntities();
	// Remove all entities from current active graph data
	void DeleteGraphEntities();
	// Delete Entity, point or edge, changing the graph data.
	void DeleteEntity(Entity e);

private:
	// Variables that only needed in editor mode
	int mActiveGraph;
	bool mDrawGraph;															// Toggle to draw the debug lines for active graph
	std::vector<std::string> mGraphDataNameList;
	std::vector<GraphData> mGraphDataList;				// TODO, not needed during game mode, consider removing it during publication
	std::vector<std::pair<Entity, std::vector<glm::vec3>>> mGraphDataEntities;	// Contains all the entities that represent the point and edges of the active graph data


	//------------------------------------------

private:
	std::vector<ALGraph> mALGraphList;

private: // Helper function
	std::pair<Entity, std::vector<glm::vec3>>* FindGraphEntity(Entity _e);
	glm::vec3 RotationalVectorToEulerAngle(glm::vec3 direction);

};











