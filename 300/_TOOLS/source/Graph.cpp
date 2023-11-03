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

Graph::Graph(unsigned size) {
  for (unsigned i{ 0 }; i < size; ++i)
    mAdjList.emplace_back(ALIST::value_type{});
}

void Graph::AddDEdge(unsigned source, unsigned destination, unsigned weight) {
  mAdjList[source - 1].emplace_back(AdjInfo{ destination, weight });
  std::sort(mAdjList[source - 1].begin(), mAdjList[source - 1].end());
}

void Graph::AddUEdge(unsigned node1, unsigned node2, unsigned weight) {
  AddDEdge(node1, node2, weight);
  AddDEdge(node2, node1, weight);
}

std::vector<DijkstraInfo> Graph::Dijkstra(unsigned start_node) const {
  auto cmp{ [](const GNode* lhs, const GNode* rhs) -> bool {return *lhs < *rhs; } };

  std::priority_queue<GNode*, std::vector<GNode*>, decltype(cmp)> pq(cmp);
  std::vector<DijkstraInfo> resultPath{};
  std::vector<GNode> nodes{};

  resultPath.reserve(mAdjList.size());
  nodes.reserve(mAdjList.size());

  // Initialize nodes in graph
  for (unsigned i{ 1 }; i <= this->mAdjList.size(); ++i) {
    GNode node{};
    if (i == start_node) {
      node.visited = true;
      node.info.cost = 0;
      node.info.path.push_back(start_node);
    } else {
      node.visited = false;
      node.info.cost = std::numeric_limits<unsigned>::max();
    }

    node.id = i;
    nodes.emplace_back(node);
  }

  // Push nodes into priority queue with updated costs
  // Only known costs are the adjacent nodes of the start node
  for (unsigned i{ 0 }; i < this->mAdjList[start_node - 1].size(); ++i) {
    const AdjInfo& nodeInfo{ mAdjList[start_node - 1][i] };
    GNode& node{ nodes[nodeInfo.id - 1] };
    node.info.cost = nodeInfo.weight;

    pq.push(&node);
    node.info.path.push_back(start_node);
  }

  // Run Dijkstra
  while (!pq.empty()) {
    // Extract min heap
    GNode* currNode{ pq.top() };
    pq.pop();

    currNode->visited = true;
    currNode->info.path.push_back(currNode->id);

    // Loop through adjacent nodes
    for (unsigned i{ 0 }; i < this->mAdjList[currNode->id - 1].size(); ++i) {
      const AdjInfo& nodeInfo{ mAdjList[currNode->id-1][i] };
      GNode& adjNode{ nodes[nodeInfo.id - 1] };

      unsigned newCost{ nodeInfo.weight + currNode->info.cost };
      if (newCost < adjNode.info.cost) {
        adjNode.info.cost = newCost;
        adjNode.info.path = currNode->info.path;

        if (!currNode->visited)
          adjNode.info.path.push_back(currNode->id);

        pq.push(&adjNode);
      }
    }
  }

  // Get and return the paths
  for (unsigned i{ 0 }; i < nodes.size(); ++i)
    resultPath.push_back(nodes[i].info);

  return resultPath;
}


//---------------------------------------------
// Graph Data
//---------------------------------------------
void GraphData::AddDEdge(glm::vec3 src, glm::vec3 dst) {
  auto it = std::find_if(mData.begin(), mData.end(),
                           [dst] (std::pair<glm::vec3, std::vector<glm::vec3>> const& pointNedges) { return dst == pointNedges.first; });

  if (it == mData.end()) AddPoint(dst);
  
  GetPointEdges(src).push_back(dst);
}

// Add undirected edge
void GraphData::AddUEdge(glm::vec3 p0, glm::vec3 p1) {
  GetPointEdges(p0).push_back(p1); // make point if can't be found
  GetPointEdges(p1).push_back(p0); // make point if can't be found
}

void GraphData::AddPoint(glm::vec3 point) {
  mData.push_back({ point, std::vector<glm::vec3>() });
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
