/*!*****************************************************************************
\file Graph.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\date 25-10-2023
This file contains the definition of graph, made using the adjacent list method,
and dijkstra algorithm to find a path.
*******************************************************************************/

#include "Graph.h"
#include <algorithm>
#include <queue>
//#include "Physics/PhysicsSystem.h"
#include <functional>

//---------------------------------------------
// Graph Data
//---------------------------------------------
GraphData::GraphData(std::string const& _filePath) {
  std::ifstream file(_filePath);

  if (!file.is_open()) {
    std::cout << "Unable to open graph data file to load: " << _filePath << '\n';
    file.close();
    return;
  }

  std::string line{};
  while (std::getline(file, line)) {
    if (!line.size()) continue;

    size_t startPos{ 0 };
    size_t endPos{ line.find(')')+1 };
    glm::vec3 point = StrToVec3(line.substr(startPos, endPos));
    AddPoint(point);

    startPos = endPos;
    while (startPos < line.length()) {
      endPos = line.find(')', startPos) + 1;
      AddDEdgeSafe(point, StrToVec3(line.substr(startPos, endPos)));
      startPos = endPos;
    }
  }

  file.close();
}

void GraphData::SaveGraph(std::string const& _filePath) {
  // Format to save is 
  // (point1)(edge1)(edge2)...
  // (point2)...
  // (point3)...
  // ...

  std::ofstream file(_filePath);

  if (!file.is_open()) {
    std::cout << "Unable to open graph data file to save: " << _filePath << '\n';
    file.close();
    return;
  }

  for (auto& [point, edges] : mData) {
    file << Vec3ToStr(point);
    for (glm::vec3 const& edge : edges)
      file << Vec3ToStr(edge);
    file << '\n';
  }

  file.close();
}

glm::vec3 GraphData::StrToVec3(std::string const& str) {
  glm::vec3 v{};

  size_t startPos{ 1 };
  size_t endPos{ str.find(',') };
  v.x = std::stof(str.substr(startPos, endPos));

  startPos = endPos + 1;
  endPos = str.find(',', endPos + 1);
  v.y = std::stof(str.substr(startPos, endPos));

  startPos = endPos + 1;
  endPos = str.length()-1;
  v.z = std::stof(str.substr(startPos, endPos));

  return v;
}

std::string GraphData::Vec3ToStr(glm::vec3 const& v) {
  return std::string("(" + std::to_string(v.x) + "," + 
                           std::to_string(v.y) + "," + 
                           std::to_string(v.z) + ")");
}

void GraphData::AddDEdge(glm::vec3 src, glm::vec3 dst) {
  auto it = std::find_if(mData.begin(), mData.end(),
                           [dst] (std::pair<glm::vec3, std::vector<glm::vec3>> const& pointNedges) { return dst == pointNedges.first; });

  if (it == mData.end()) AddPoint(dst);
  
  if (CheckForEdge(src, dst))
    std::cout << "Warning: Attempting to add edge that was already added\n";
  else GetPointEdges(src).push_back(dst);
}

void GraphData::AddDEdgeSafe(glm::vec3 src, glm::vec3 dst) {
  if (CheckForEdge(src, dst))
    std::cout << "Warning: Attempting to add edge that was already added\n";
  else GetPointEdges(src).push_back(dst);
}

// Add undirected edge
void GraphData::AddUEdge(glm::vec3 p0, glm::vec3 p1) {
  AddDEdgeSafe(p0, p1);
  AddDEdgeSafe(p1, p0);
}

void GraphData::AddPoint(glm::vec3 point) {
  mData.push_back({ point, std::vector<glm::vec3>() });
}

void GraphData::DeletePoint(glm::vec3 _point) {
  auto it = std::find_if(mData.begin(), mData.end(), [_point] (std::pair<glm::vec3, std::vector<glm::vec3>> const& pointnedge) { return _point == pointnedge.first; });
  if (it == mData.end()) {
    std::cout << "Unable to delete non-existant point in graph data\n";
    return;
  }
  mData.erase(it);

  for (auto& [point, edges] : mData) {
    auto it = std::find(edges.begin(), edges.end(), _point);
    if (it == edges.end()) continue;
    edges.erase(it);
  }
}

void GraphData::DeleteUEdge(glm::vec3 p0, glm::vec3 p1) {
  auto p0it = std::find_if(mData.begin(), mData.end(), [p0] (std::pair<glm::vec3, std::vector<glm::vec3>> const& pointnedge) { return p0 == pointnedge.first; });
  auto p1it = std::find_if(mData.begin(), mData.end(), [p1] (std::pair<glm::vec3, std::vector<glm::vec3>> const& pointnedge) { return p1 == pointnedge.first; });

  if (p0it != mData.end()) {
    auto it = std::find(p0it->second.begin(), p0it->second.end(), p1);
    if (it != p0it->second.end()) p0it->second.erase(it);
  }

  if (p1it != mData.end()) {
    auto it = std::find(p1it->second.begin(), p1it->second.end(), p0);
    if (it != p1it->second.end()) p1it->second.erase(it);
  }
}

void GraphData::DeleteDEdge(glm::vec3 src, glm::vec3 dst) {
  auto it = std::find_if(mData.begin(), mData.end(), [src] (std::pair<glm::vec3, std::vector<glm::vec3>> const& pointnedge) { return src == pointnedge.first; });
  if (it != mData.end()) {
    auto itedges = std::find(it->second.begin(), it->second.end(), dst);
    if (itedges != it->second.end()) it->second.erase(itedges);
  }
}


bool GraphData::CheckForEdge(glm::vec3 src, glm::vec3 dst) {
  auto const& edges = GetPointEdges(src);
  auto it = std::find_if(edges.begin(), edges.end(), [dst] (glm::vec3 const& vec) { return dst == vec; });
  return it != edges.end();
}

std::vector<glm::vec3>& GraphData::GetPointEdges(glm::vec3 point) {
  auto it = std::find_if(mData.begin(), mData.end(),
                         [point] (std::pair<glm::vec3, std::vector<glm::vec3>> const& pointNedges) { return point == pointNedges.first; });
  if (it == mData.end()) {
    AddPoint(point);
    return mData.back().second;
  } else {
    return it->second;
  }
}

ALGraph GraphData::MakeALGraph() {
  ALGraph graph(mData.size());

  // Adding all the points
  int i{};
  for (auto& [point, edges] : mData) {
    graph.mData[i++].point = point;
  }

  // Add edges
  // Find all the same vector and make a new edge, push back into the edges container
  i = 0;
  for (auto& [point, edges] : mData) { // In all the graphdata's points
    ALGraph::AdjList& currPoint = graph.mData[i++]; // Get the corresponding points in ALGraph
    for (glm::vec3 const& neighbor : edges) { // In the current point, get all connected points in graphdata
      auto it = std::find_if(graph.mData.begin(), graph.mData.end(), // Find them in ALGraph so that you can point towards them and calculate the weight
                             [neighbor] (ALGraph::AdjList& adjList) { return adjList.point == neighbor; });
      assert((it != graph.mData.end()) && "Unable to add edge to AdjList since the other connection point does not exist in ALGraph's mData");

      // calculate the weight and get pointer, then add it into the edge list
      float weight = abs(glm::length(currPoint.point - neighbor));
      currPoint.edges.emplace_back(ALGraph::Edge(&*it, weight));
    }
  }

  return graph;
}

void TestGraph() {
  std::cout << "---graph test start---\n";

  glm::vec3 p1{ 1,2,3 };
  glm::vec3 p2{ 2,3,4 };
  glm::vec3 p3{ 3,4,5 };
  glm::vec3 p4{ 4,5,6 };
  glm::vec3 p5{ 5,6,7 };
  glm::vec3 p6{ 6,7,8 };
  glm::vec3 p7{ 7,8,9 };
  glm::vec3 p8{ 8,9,10 };
  glm::vec3 p9{ 9,10,11 };
  glm::vec3 p10{ 10,11,12 };

  std::string str = GraphData::Vec3ToStr(p1);
  GraphData::StrToVec3(str);

  GraphData graphData;
  graphData.AddUEdge(p1, p2);
  graphData.AddUEdge(p2, p3);
  graphData.AddUEdge(p3, p4);
  graphData.AddUEdge(p4, p5);
  graphData.AddUEdge(p5, p6);
  graphData.AddUEdge(p6, p7);
  graphData.AddUEdge(p7, p8);
  graphData.AddUEdge(p8, p9);
  graphData.AddUEdge(p9, p10);
  graphData.AddUEdge(p10, p1);

  graphData.DeleteUEdge(p1, p2);
  graphData.DeleteUEdge(p1, p10);

  std::string filePath{ "../assets/GraphData/graphData1.txt" };
  //graphData.SaveGraph(filePath);
  GraphData graphData1(filePath);


  ALGraph alGraph = graphData.MakeALGraph();
  alGraph.Print();

  std::cout << "---graph test finish---\n";
}

void ALGraph::Print() {
  std::cout << "---Printing ALGraph start---\n";
  for (AdjList& adjList : mData) {
    PrintVec(adjList.point);

    for (auto& edge : adjList.edges) {
      std::cout << " -> ";
      PrintVec(edge.vertex->point);

#if 1 // Print weight
      std::cout << " (" << edge.distance << ")";
#endif
    }
    std::cout << "\n";
  }
  std::cout << "---Printing ALGraph finish---\n";
}

float ALGraph::CalcHCost(glm::vec3 const& p0, glm::vec3 const& p1) {
  return abs(glm::length(p0 - p1));
}

void ALGraph::AStarInit(glm::vec3 const& start, glm::vec3 const& end) {
  // Add start and end nodes
  mData.push_back(decltype(mData)::value_type());
  mData.back().point = start;
  mData.push_back(decltype(mData)::value_type());
  mData.back().point = end;

  // Initialise HCost and reset all other values
  for (AdjList& node : mData) {
    node.hCost = CalcHCost(node.point, end);
    node.gCost = 0;
    node.state = NODE_STATE::NONE;
    node.parent = nullptr;
  }

}

std::vector<glm::vec3> ALGraph::AStarPath(glm::vec3 const& start, glm::vec3 const& end) {
  // Initialise containers (open list, close list)
  // Put starting node on the open list
  OpenList openList;

  AStarInit(start, end);
  openList.Insert(&*mData.end()-2); // Add start node to openList
  AdjList* endNode{ &*mData.end()-1 };

  while (!openList.IsEmpty()) {
    AdjList* currNode = openList.PopLowestCostNode();

    if (currNode == endNode) {
      return MasterPath(currNode);
    }

    currNode->state = NODE_STATE::CLOSE;

    for (Edge& edge : currNode->edges) {
      AdjList* neighbor = edge.vertex;
      float newGCost = currNode->gCost + edge.distance;
      float fCost = newGCost + neighbor->hCost;

      if (neighbor->state == NODE_STATE::NONE) {
        neighbor->parent = currNode;
        neighbor->gCost = newGCost;
        openList.Insert(neighbor);
      } else if (neighbor->gCost + neighbor->hCost > fCost) {
        neighbor->parent = currNode;
        neighbor->gCost = newGCost;
        if (neighbor->state == NODE_STATE::CLOSE)
          openList.Insert(neighbor);
      }
    }
  }

  AStarExit();
  return std::vector<glm::vec3>();
}

//void ALGraph::ConnectStartAndEnd() {
//  // TODO WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING. I set the max height the start and end node can connect to to be 10
//  // Make those points their neighbor
//
//  bool(*fptr)(glm::vec3, glm::vec3);
//  
//  std::function
//
//  // Identify the start end node
//  AdjList* startNode{ &*mData.end()-2 };
//  AdjList* endNode{ &*mData.end()-1 };
//
//  // For each of them, identify all the nodes that they can see
//  for (auto& adjList : mData) {
//    // If out of elevation range, don't connect
//    if (abs(glm::length(adjList.point - startNode->point)) > mSetting.elevation) continue;
//
//    // Check line of sight
//    //if ()
//
//
//  }
//}

void ALGraph::AStarExit() {
  // Pop the start and end nodes
  mData.pop_back();
  mData.pop_back();
}

std::vector<glm::vec3> ALGraph::MasterPath(AdjList* tailNode) {
  std::vector<glm::vec3> path;
  path.push_back(tailNode->point);
  while (tailNode->parent != nullptr) {
    tailNode = tailNode->parent;
    path.push_back(tailNode->point);
  }

  AStarExit();
  return path;
}


//------------------------------------
// Open List Functionalities
//------------------------------------
void ALGraph::OpenList::Insert(AdjList* node) {
  mData.push_back(node);
}

ALGraph::AdjList* ALGraph::OpenList::PopLowestCostNode() {
  if (IsEmpty()) return nullptr;

  AdjList** cheapest = &*mData.begin();
  float cheapestFCost = (*cheapest)->gCost + (*cheapest)->hCost;
  for (auto it = mData.begin(); it != mData.end(); ++it) {
    float newFCost = (*it)->gCost + (*it)->hCost;
    if (newFCost < cheapestFCost) {
      cheapestFCost = newFCost;
      cheapest = &*it;
    }
  }

  // Swap back to return val's location
  AdjList* returnVal = *cheapest;
  *cheapest = std::move(mData.back());
  mData.pop_back();
  return returnVal;
}

bool ALGraph::OpenList::IsEmpty() {
  return mData.empty();
}

void ALGraph::OpenList::Sort() {
  std::sort(mData.begin(), mData.end(), [] (AdjList* a, AdjList* b) { return (a->gCost + a->hCost) > (b->gCost + b->hCost); });
}