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

void TestGraph();

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
  bool CheckForEdge(glm::vec3 src, glm::vec3 dst);
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
  ALGraph(int _size) { mData.resize(_size); }

  void Print();

  struct AdjList;

  struct Edge {
    AdjList* vertex;    // Vertex id in the data 
    float weight;       // Distance 
  };

  struct AdjList {
    glm::vec3 point;
    std::vector<Edge> edges; // Vertices that this vertex is pointing to
  };

  std::vector<AdjList> mData;
private:

  void PrintVec(glm::vec3 v) {
    std::cout << v.x << "|" << v.y << "|" << v.z;
  }
};


