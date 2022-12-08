#include "support.hpp"

float Node::calculateHeuristic(Node target) {
  Support support;
  return support.manhattanDistance(this->coordinates, target.coordinates);
}

void Node::addNeighbors(){
    // TO DO
};

bool Coordinates::isEqual(Coordinates point) {
  return (this->x == point.x) && (this->y == point.y);
}

float Support::manhattanDistance(Coordinates start, Coordinates target) {
  return abs(start.x - target.x) + abs(start.y - target.y);
}

bool Support::containsInPriorityQueue(Node node, priority_queue<Node, vector<Node>, Compare> priorityQueue) {
  while (!priorityQueue.empty()) {
    Node n = priorityQueue.top();
    priorityQueue.pop();
    if (n.coordinates.isEqual(node.coordinates))
      return true;
    }
  return false;
}

bool Support::containsInVector(Node node, vector<Node> vector) {
  for (int i = 0; i < vector.size(); i++) {
    if (vector[i].coordinates.isEqual(node.coordinates))
      return true;
  }
  return false;
}