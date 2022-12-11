#ifndef ASTAR
#define ASTAR

#include "support.hpp"

class AStar {
 public:
  vector<Node> aStar(Node start, Node target, int maxX, int maxY, json map);
  vector<Node> getPath(Node node);
};

#endif
