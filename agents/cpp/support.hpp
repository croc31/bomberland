#ifndef SUPPORT
#define SUPPORT
#define MAX_INT 99999
#include <stdlib.h>

#include <cmath>
#include <queue>
#include <vector>
using namespace std;

struct Node {
 public:
  float g = 0;
  float f = MAX_INT;
  float h = MAX_INT;
  Coordinates coordinates;
  Node* parent = nullptr;
  vector<Node> neighbors;
  Node(){};
  Node(int h, int g, int f, Coordinates coordinates, Node* parent) {
    this->h = h;
    this->g = g;
    this->f = f;
    this->coordinates = coordinates;
    this->parent = parent;
  };
  float calculateHeuristic(Node target);
  void addNeighbors();
};

class Coordinates {
 public:
  int x;
  int y;
  Coordinates(){};
  Coordinates(int x, int y) {
    this->x = x;
    this->y = y;
  };
  bool isEqual(Coordinates point);
};

class Support {
 public:
  float manhattanDistance(Coordinates start, Coordinates target);
  bool containsInPriorityQueue(Node node, priority_queue<Node, vector<Node>, Compare> priorityQueue);
  bool containsInVector(Node node, vector<Node> vector);
};

struct Compare {
  bool operator()(const Node& n1, const Node& n2) {
    return n1.f > n2.f;
  }
};

#endif