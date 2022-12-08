#include "aStar.hpp"
using namespace std;

vector<Node> AStar::aStar(Node start, Node target) {
  Support support;

  priority_queue<Node, vector<Node>, Compare> openList;
  vector<Node> closedList;

  start.h = start.calculateHeuristic(target);
  start.f = start.g + start.h;

  openList.push(start);

  while (!openList.empty()) {
    Node node = openList.top();
    openList.pop();

    if (start.coordinates.isEqual(target.coordinates)) {
      return vector<Node>{};
    }
    // TO DO - adicionar os vizinhos (implementar metodo addNeighbors() verificando quais vizinhos disponiveis e adicionar) 
    node.addNeighbors();

    for (auto neighbor : node.neighbors) {
      Node* parent = new Node(node.h, node.g, node.f, node.coordinates, node.parent);

      Node node;
      node.parent = parent;
      node.coordinates = neighbor.coordinates;
      node.g = parent->g + 1;
      node.h = node.calculateHeuristic(target);
      node.f = node.g + node.h;

      if (!support.containsInPriorityQueue(node, openList)) {
        openList.push(node);
      }

      closedList.push_back(node);
      
    }
  }

  return vector<Node>{};
}