#include "aStar.hpp"

vector<Node> AStar::aStar(Node start, Node target, int maxX, int maxY, json map) {
  Support support;

  priority_queue<Node, vector<Node>, Compare> openList;
  vector<Node> closedList;

  start.h = start.calculateHeuristic(target);
  start.f = start.g + start.h;

  openList.push(start);

  while (!openList.empty()) {
    Node node = openList.top();
    openList.pop();

    if (node.coordinates.isEqual(target.coordinates)) {
      return getPath(node);
    }

    node.addNeighbors(maxX, maxY, map);

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

vector<Node> AStar::getPath(Node node) {
  vector<Node> vectorPath;
  while (node.parent != nullptr) {
    vectorPath.push_back(node);
    node = *(node.parent);
  }
  return vectorPath;
}