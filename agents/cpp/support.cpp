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

Coordinates Support::getValidCoordinates(Coordinates start, int maxX, int maxY){
  Coordinates retorno = start;
  if(start.x < 0 ){
    retorno.x = 0;
  }else if(start.x > maxX){ 
    retorno.x = maxX;
  }

  if(start.y < 0 ){
    retorno.y = 0;
  }else if(start.y > maxY){ 
    retorno.y = maxY;
  }

  return retorno;
}

json Support::entityAt(json entitiesJson, Coordinates point) {
  json entityAtPoint;
  for (auto it = entitiesJson.begin(); it != entitiesJson.end(); ++it) {
    if (((*it)["x"].get<int>() == point.x && (*it)["y"].get<int>() == point.y) && !((*it)["type"].get<string>() == "m" || (*it)["type"].get<string>() == "x")) {
      entityAtPoint = (*it);
      break;
    }
  }
  return entityAtPoint;
}

Coordinates Support::getFreeCoordinates(Coordinates start, int maxX, int maxY, json entitiesJson) {
  if (isInBounds(start, maxX, maxY) && !isOccupied(entitiesJson, start)) {
    return start;
  } else {
    Coordinates random = getRandomMove(start);
    random.x = random.x % maxX; 
    random.y = random.y % maxY; 
    return random;
    
  }
}

Coordinates Support::getRandomMove(Coordinates start) {
  Coordinates north;
  north.x = start.x;
  north.y = start.y + 1;
  Coordinates south;
  south.x = start.x;
  south.y = start.y - 1;
  Coordinates east;
  east.x = start.x + 1;
  east.y = start.y;
  Coordinates west;
  west.x = start.x - 1;
  west.y = start.y;
  const vector<Coordinates> actions = {north, south, east, west};
  return actions[rand() % actions.size()];
}
vector<Coordinates> Support::getBombs(json entities){
    vector<Coordinates> bombs;
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        if ((*it)["type"].get<string>() == "b") {
            bombs.push_back(Coordinates{ (*it)["x"], (*it)["y"] });
        }
    }
    return bombs;
}
Coordinates Support::getFirstBomb(vector<Coordinates> bombs, Coordinates point){
  Coordinates bomb;
    for(int i=0; i<bombs.size(); i++){
      if(manhattanDistance(point, bombs[i]) < manhattanDistance(point, bomb)){
        bomb = bombs[i];
      }
    }
    return bomb;
}