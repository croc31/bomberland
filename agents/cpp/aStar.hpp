#ifndef ASTAR
#define ASTAR

#include <vector>

#include "support.hpp"
using namespace std;

class AStar {
 public:
  vector<Node> aStar(Node start, Node target);
};

#endif
