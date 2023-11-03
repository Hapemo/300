/*!*****************************************************************************
\file Graph.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\date 25-10-2023
This file contains the declaration of graph, made using the adjacent list
method, and dijkstra algorithm to find a path.
*******************************************************************************/
#pragma once
#include "pch.h"
#include <glm/glm.hpp>

struct DijkstraInfo {
  unsigned cost;
  std::vector<unsigned> path;
};

struct AdjInfo {
  unsigned id;
  unsigned weight;

  /*!*****************************************************************************
  AdjacencyInfo operator< overload which is used for sorting/comparing

  \param const AdjacencyInfo&
  A reference to read-only data to compare with

  \return bool
  Result of the evaluation
  *******************************************************************************/
  bool operator<(const AdjInfo& rhs) const {
    if (this->weight == rhs.weight)
      return this->id < rhs.id;
    return this->weight < rhs.weight;
  }
};


class Graph {
  using ALIST = std::vector<std::vector<AdjInfo>>;
public:
  /*!*****************************************************************************
  Graph default constructor that initializes the stored list of adjacent nodes

  \param unsigned
  Number of nodes in the graph
  *******************************************************************************/
  Graph(unsigned size);

  /*!*****************************************************************************
  Graph AddDEdge function that adds a directed edge to two given nodes

  \param unsigned
  ID of the source node

  \param unsigned
  ID of the destination node

  \param unsigned
  Cost of travelling from source to dest
  *******************************************************************************/
  void AddDEdge(unsigned source, unsigned destination, unsigned weight);

  /*!*****************************************************************************
  Graph AddUEdge function that adds a bi-directional edge to two given nodes

  \param unsigned
  ID of the source node

  \param unsigned
  ID of the destination node

  \param unsigned
  Cost of travelling inbetween the two nodes
  *******************************************************************************/
  void AddUEdge(unsigned node1, unsigned node2, unsigned weight);

  /*!*****************************************************************************
  Graph Dijkstra function that runs the Dijkstra algorithm to find the shortest
  path to every node in the graph given the start node

  \param unsigned
  ID of the start node

  \return std::vector<DijkstraInfo>
  Container storing the shortest path to each node
  *******************************************************************************/
  std::vector<DijkstraInfo> Dijkstra(unsigned start_node) const;

  /*!*****************************************************************************
  Graph GetAList function that gets and returns the stored adjacency list of
  the graph

  \return ALIST
  The stored adjacency list of the graph
  *******************************************************************************/
  ALIST GetAList() const { return this->mAdjList; }

private:

  struct GNode {
    unsigned id;
    bool visited;
    DijkstraInfo info;

    /*!*****************************************************************************
    GNode operator< overload which is used for sorting/comparing

    \param const GNode&
    A reference to read-only data to compare with

    \return bool
    Result of the evaluation
    *******************************************************************************/
    bool operator<(const GNode& _rhs) const {
      return this->info.cost < _rhs.info.cost;
    }
  };

  // Other private fields and methods
  ALIST mAdjList;
};

//template<int size>
class ALGraph;

class GraphData {
public:
  void AddDEdge(glm::vec3 src, glm::vec3 dst);
  void AddUEdge(glm::vec3 p0, glm::vec3 p1);
  void AddPoint(glm::vec3 point);
  // Get the point's edges. If can't find point, make a new point
  std::vector<glm::vec3>& GetPointEdges(glm::vec3 point);

  // Convert this graph data into a ALGraph
  //template<int size>
  ALGraph MakeALGraph(); // Make a non-dynamic container for efficiency

  std::vector<std::pair<glm::vec3, std::vector<glm::vec3>>> mData;
};

//template<int size>
class ALGraph {
public:
  static const int size = 10;
  struct AdjList;

  struct Edge {
    AdjList* vertex;    // Vertex id in the data 
    float weight;       // Distance 
  };

  struct AdjList {
    glm::vec3 point;
    std::vector<Edge> edges; // Vertices that this vertex is pointing to
  };

  std::array<AdjList, size> mData;
private:
};

//template<int size>
ALGraph GraphData::MakeALGraph() {
  ALGraph graph; // ALGraph<edges.size()>

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

