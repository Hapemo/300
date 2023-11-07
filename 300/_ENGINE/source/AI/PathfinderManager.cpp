#include "pch.h"
#include "AI/PathfinderManager.h"
#include "ConfigManager.h"
#include "Graph.h"

PathfinderManager::PathfinderManager() : mActiveGraph(-1), mDrawGraph(true), mGraphDataNameList(), mGraphDataList(), mGraphDataEntities(), mALGraphList() {}

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











