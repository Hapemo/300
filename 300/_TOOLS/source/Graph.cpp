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
  
  if (CheckForEdge(src, dst))
    std::cout << "Warning: Attempting to add edge that was already added\n";
  else GetPointEdges(src).push_back(dst);
}

// Add undirected edge
void GraphData::AddUEdge(glm::vec3 p0, glm::vec3 p1) {
  if (CheckForEdge(p0, p1))
    std::cout << "Warning: Attempting to add edge that was already added\n";
  else GetPointEdges(p0).push_back(p1); // make point if can't be found
  
  if (CheckForEdge(p1, p0))
    std::cout << "Warning: Attempting to add edge that was already added\n";
  else GetPointEdges(p1).push_back(p0); // make point if can't be found
}

void GraphData::AddPoint(glm::vec3 point) {
  mData.push_back({ point, std::vector<glm::vec3>() });
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
      std::cout << " (" << edge.weight << ")";
#endif
    }
    std::cout << "\n";
  }
  std::cout << "---Printing ALGraph finish---\n";
}


