#include "aStar.hpp"

vector<Node> AStar::aStar(Node start, Node target) {
  Support support;

  if (support.isDestination(start.coordinates, target.coordinates)) {
    return vector<Node>{};
  }

  // verificar implementação do https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h pode ajudar

  return vector<Node>{};
}