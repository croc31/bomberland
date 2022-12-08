#ifndef ASTAR
#define ASTAR

#include "support.hpp"

class AStar {
 public:
  vector<Node> aStar(Node start, Node target);
  vector<Node> getPath(Node node);
};

#endif
