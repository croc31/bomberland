#include "aStar.hpp"

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

    for (Node neighbor : node.neighbors) {
      Node* parent = new Node(node.h, node.g, node.f, node.coordinates, node.parent);
      Node n = neighbor;
      if (!support.containsInPriorityQueue(n, openList) && !support.containsInVector(n, closedList)) {
        n.parent = parent;
        n.g = parent->g + 1;
        n.h = n.calculateHeuristic(target);
        n.f = n.g + n.h;
        openList.push(n);
      }
    }
    closedList.push_back(node);
  }

  return vector<Node>{};
}