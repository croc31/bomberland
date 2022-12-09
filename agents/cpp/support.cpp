#include "support.hpp"

int Node::calculateHeuristic(Node target) {
  Support support;
  return support.manhattanDistance(this->coordinates, target.coordinates);
}

void Node::addNeighbors(int maxX, int maxY, json map) {
  Support support;
  Node north;
  north.coordinates.x = this->coordinates.x;
  north.coordinates.y = this->coordinates.y + 1;
  Node south;
  south.coordinates.x = this->coordinates.x;
  south.coordinates.y = this->coordinates.y - 1;
  Node east;
  east.coordinates.x = this->coordinates.x + 1;
  east.coordinates.y = this->coordinates.y;
  Node west;
  west.coordinates.x = this->coordinates.x - 1;
  west.coordinates.y = this->coordinates.y;

  vector<Node> neighborsVector{north, south, west, east};

  for (int i = 0; i < neighborsVector.size(); i++) {
    if (!support.isInBounds(neighborsVector[i].coordinates, maxX, maxY)) {
      neighborsVector.erase(neighborsVector.begin() + i);
    }
  }

  this->neighbors = removeUnavailableNeighbors(map, neighborsVector);
};

vector<Node> Node::removeUnavailableNeighbors(json map, vector<Node> neighbors) {
  Support support;
  vector<Node> available;
  for (Node node : neighbors) {
    if (!support.isOccupied(map, node.coordinates)) {
      available.push_back(node);
    }
  }

  return available;
}

bool Coordinates::isEqual(Coordinates point) {
  return (this->x == point.x) && (this->y == point.y);
}

int Support::manhattanDistance(Coordinates start, Coordinates target) {
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

bool Support::isInBounds(Coordinates point, int maxX, int maxY) {
  if (point.x >= maxX || point.x <= 0 || point.y >= maxY || point.y < 0)
    return false;
  return true;
}

bool Support::isOccupied(json entitiesJson, Coordinates point) {
  return entityAt(entitiesJson, point).size() != 0;
}

json Support::entityAt(json entitiesJson, Coordinates point) {
  json entityAtPoint;
  for (auto it = entitiesJson.begin(); it != entitiesJson.end(); ++it) {
    if (((*it)["x"].get<int>() == point.x && (*it)["y"].get<int>() == point.y) && (*it)["type"].get<string>() != "a") {
      entityAtPoint = (*it);
      break;
    }
  }
  return entityAtPoint;
}