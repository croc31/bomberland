#ifndef SUPPORT
#define SUPPORT
#include <stdlib.h>

#include <cmath>

struct Node {
  Coordinates coordinates;
  Coordinates parent;
  float g;
  float h;
  float f;
};

class Coordinates {
 public:
  int x;
  int y;
  Coordinates(int x, int y) {
    this->x = x;
    this->y = y;
  };
  bool isEqual(Coordinates point);
};

class Support {
 public:
  int manhattanDistance(Coordinates start, Coordinates target);
  bool isDestination(Coordinates start, Coordinates target);
};

#endif