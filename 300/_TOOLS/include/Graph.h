/*!*****************************************************************************
\file Graph.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\date 25-10-2023
This file contains the declaration of graph, made using the adjacent list
method, and dijkstra algorithm to find a path.

To use the AStar pathfinding, user must first create the graph using GraphData.
Then generate ALGraph using the GraphData created, thereafter use the AStar 
pathfinding algorithm in ALGraph.
*******************************************************************************/
#pragma once
#include "pch.h"
#include <glm/glm.hpp>

void TestGraph();


class ALGraph;

class GraphData {
public:
  void AddDEdge(glm::vec3 src, glm::vec3 dst);
  void AddUEdge(glm::vec3 p0, glm::vec3 p1);
  void AddPoint(glm::vec3 point);
  void DeletePoint(glm::vec3 _point);
  void DeleteEdge(glm::vec3 p0, glm::vec3 p1);
  bool CheckForEdge(glm::vec3 src, glm::vec3 dst);
  // Get the point's edges. If can't find point, make a new point
  std::vector<glm::vec3>& GetPointEdges(glm::vec3 point);

  // Convert this graph data into a ALGraph
  ALGraph MakeALGraph(); // Make a non-dynamic container for efficiency

  std::vector<std::pair<glm::vec3, std::vector<glm::vec3>>> mData;
};

class ALGraph {
public:
  ALGraph(int _size) { mData.resize(_size); }

  enum NODE_STATE {
    NONE,
    OPEN,
    CLOSE
  };

  struct AdjList;

  struct Edge {
    AdjList* vertex;    // Vertex id in the data 
    float distance;     // Distance from owner of edge to the vertex in the edge
  };

  // This is like a node
  struct AdjList {
    NODE_STATE state = NONE;
    glm::vec3 point;
    std::vector<Edge> edges; // Vertices that this vertex is pointing to
    AdjList* parent;
    float gCost;
    float hCost;
  };

  void Print();
  static void PrintVec(glm::vec3 v) { std::cout << v.x << "|" << v.y << "|" << v.z; }
  std::vector<AdjList> mData;
public:
//------------------------------
// AStar pathfinding portion
//------------------------------
  
  class OpenList {
  public:
    void Insert(AdjList* node);   // O(1)
    AdjList* PopLowestCostNode(); // O(n)
    bool IsEmpty();
  private:
    // Sort the container, put largest fCost infront and smallest fCost behind
    void Sort();
    std::vector<AdjList*> mData;
    std::vector<float> fCostHistory;
  };

  // Put the G cost and H cost in the AdjList (G cost is accumulated from path, H cost is cost to end point)

  // Add starting and ending adjList into mData at the back, so startnode = end-2, endnode = end-1
  // Do line of sight check with all the nodes and generate the edges for start and end node
  // Calculate G and H cost for every nodes
  std::vector<glm::vec3> AStarPath(glm::vec3 const& start, glm::vec3 const& end); // Generates the AStar path for the graph
private:
  void AStarInit(glm::vec3 const& start, glm::vec3 const& end);
  void AStarExit();
  float CalcHCost(glm::vec3 const& p0, glm::vec3 const& p1);    // TODO find out the most suitable heuristic cost function
  std::vector<glm::vec3> MasterPath(AdjList* tailNode);
};


