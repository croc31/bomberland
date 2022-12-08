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
    if (n.coordinates.x == node.coordinates.x && n.coordinates.y == node.coordinates.y) {
      return true;
    }
  }
  return false;
}