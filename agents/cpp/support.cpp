#include "support.hpp"

bool Coordinates::isEqual(Coordinates point) {
  return (this->x == point.x) && (this->y == point.y);
}

int Support::manhattanDistance(Coordinates start, Coordinates target) {
  return abs(start.x - target.x) + abs(start.y - target.y);
}

bool Support::isDestination(Coordinates start, Coordinates target) {
  if (start.x == target.x && start.y == target.y) {
    return true;
  }
  return false;
}